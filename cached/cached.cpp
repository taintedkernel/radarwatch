
#include <list>
#include <stack>
#include <string>

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <regex.h>
#include <dirent.h>
#include <getopt.h>

#include <gd.h>
#include <gdfontt.h>
#include <gdfonts.h>
#include <gdfontmb.h>
#include <gdfontl.h>
#include <gdfontg.h>

#include <sqlite3.h>

#include "cached.h"


using namespace std;


// Storm ID matrix used for identifying individual storms
static unsigned int **cellIDMatrix = NULL;

// Our source radar image
gdImagePtr radarImage;

// SQLite database
sqlite3 *metaDB, *imageDB;

// Keep track of how many storm-free radar images were removed
unsigned int removedImagesCount = 0;
unsigned int markedDeletionCount = 0;

// Debugging
bool debug = false;
bool shutdownSignal = false;


char *substr(char *dest, const char *src, int position, int length)
{
	char *pointer = dest;

	memset(dest, '\0', 4);
	while(length > 0) {
		*dest = *(src+position);
		dest++;
		src++;
		length--;
	}

	return pointer;
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
  int i;
  for(i=0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}


void DebugStmt(sqlite3_stmt *ppStmt)
{
	printf("column count=%d, type=%d\n", sqlite3_column_count(ppStmt), sqlite3_column_type(ppStmt, 0));

	for (int i=0; i < sqlite3_column_count(ppStmt); i++) {
		printf("%s", sqlite3_column_text(ppStmt, i));
		printf("%d", sqlite3_column_int(ppStmt, i));
	}
}

int RunQuery(sqlite3 *db, const char *query)
{
	char *errMsg;

	if (query == NULL) {
		fprintf(stderr, "SQL error: unable to create query %s, aborting\n", query);
		return -1;
	}

	int rc = sqlite3_exec(db, query, NULL, 0, &errMsg);
	if (rc == SQLITE_CANTOPEN)
	{
		fprintf(stderr, "SQL error: %s, rc: %d, query %s, retrying\n", errMsg, rc, query);
		int attempt = 1;
		while (attempt++ < 3) {
			fprintf(stderr, "retrying query %s, attempt %d\n", query, attempt);
			rc = sqlite3_exec(db, query, NULL, 0, &errMsg);
			if (rc == SQLITE_OK) return rc;
			fprintf(stderr, "SQL error: %s, rc: %d, query %s, retrying\n", errMsg, rc, query);
		}
	}
	else if (rc != SQLITE_OK)
		fprintf(stderr, "SQL error: %s, rc: %d, query %s\n", errMsg, rc, query);

	return rc;
}

// Returns NEXRAD radar signal strength of a pixel coordinate (indexed palette color)
inline int GetFullPixelDBZIndex(unsigned int x, unsigned int y)
{
	register int red = radarImage->red[gdImageGetPixel(radarImage, x, y)];				// *SLOW*
	register int green = radarImage->green[gdImageGetPixel(radarImage, x, y)];			// *SLOW*
	register int blue = radarImage->blue[gdImageGetPixel(radarImage, x, y)];			// *SLOW*

	for (register int i = 1; i <= sizeof(dbZFullColorList) / (3*sizeof(int)); i++)		// 3 = R,G,B  **SLOW**
	{
        if (CheckListColor(i, red, green, blue, dbZFullColorList)) {			// **SLOW**
            return i;
        }
    }

    return 0;
}

// Overloaded function with combined xy pair argument
inline int GetFullPixelDBZIndex(cartesianPair xy)
{
	register int red = radarImage->red[gdImageGetPixel(radarImage, xy.x, xy.y)];
	register int green = radarImage->green[gdImageGetPixel(radarImage, xy.x, xy.y)];
	register int blue = radarImage->blue[gdImageGetPixel(radarImage, xy.x, xy.y)];

	for (register int i = 1; i <= sizeof(dbZFullColorList) / (3*sizeof(int)); i++)     // 3 = R,G,B
	{
        if (CheckListColor(i, red, green, blue, dbZFullColorList)) {
/*		if (CheckListColor(i,
				radarImage->red[gdImageGetPixel(radarImage, xy.x, xy.y)],
				radarImage->green[gdImageGetPixel(radarImage, xy.x, xy.y)],
				radarImage->blue[gdImageGetPixel(radarImage, xy.x, xy.y)],
				dbZFullColorList)) {*/
			return i;
		}
	}

	return 0;
}

unsigned int PaintCell(cartesianPair point, unsigned int id)
{
	unsigned int cellSize = 0;
	bool localDebug = false;
	int dbzValue;
    stack<cartesianPair> pixels;

    // sin/cos first deravitive values circling a central pixel,
	// radius 1, starting at 12 o'clock (90 degrees)
    static int xShift[] = {  0,  1,  0,  0, -1, -1,  0,  0, };
    static int yShift[] = { -1,  0,  1,  1,  0,  0, -1, -1, };

    if (Index2DBZFull(GetFullPixelDBZIndex(point)) < THRESH_DBZ_TSTORM) return 0;

    if (localDebug) printf("painting cell %d ... ", id);
    pixels.push(point);

    while (!pixels.empty())
    {
        point = pixels.top();
        pixels.pop();
		cellSize++;

		dbzValue = GetFullPixelDBZIndex(point);
		if (localDebug) printf("(%d,%d,%d,%d)\n", point.x, point.y, dbzValue, Index2DBZFull(dbzValue));

        if (Index2DBZFull(GetFullPixelDBZIndex(point)) >= THRESH_DBZ_TSTORM && cellIDMatrix[point.y][point.x] == 0)
            cellIDMatrix[point.y][point.x] = id;

        // Take our xy point and check the surrounding 8 pixels
        for (int i = 0; i <= 7; i++)
        {
            point.x += xShift[i];
            point.y += yShift[i];

            if (point.x < 0 || point.y < 0 || point.x >= gdImageSX(radarImage) || point.y >= gdImageSY(radarImage))
                continue;

            if (Index2DBZFull(GetFullPixelDBZIndex(point)) >= THRESH_DBZ_TSTORM && cellIDMatrix[point.y][point.x] == 0) {
                cellIDMatrix[point.y][point.x] = id;
                pixels.push(point);
            }
			if (localDebug) printf(".");
        }
    }

	if (localDebug) printf("\n");
	return cellSize;
}

unsigned int addRadarToDB(const char *radarFilePath, sqlite3 *db)
{
	char *radarData;
	int n, rc, filesize;
	FILE *radarImage;
	sqlite3_stmt *ppStmt;

	// Make a pointer to the filename proper, sans any path
	char *radarFileName = (char *)strrchr(radarFilePath, '/') + 1;

	char *query = sqlite3_mprintf("SELECT id FROM imagedata WHERE filename LIKE '%s'", radarFileName);
	rc = sqlite3_prepare_v2(db, query, -1, &ppStmt, 0);
	if (rc == SQLITE_OK && ppStmt != NULL)
	{
		rc = sqlite3_step(ppStmt);
		if (rc == SQLITE_ROW) {
			int id = sqlite3_column_int(ppStmt, 0);
			rc = sqlite3_finalize(ppStmt);
			printf("existing radar image ID %d found for radar '%s', keeping existing image\n", id, radarFileName);
			if (id > 0)
				return 0;
		}

		rc = sqlite3_finalize(ppStmt);
	}

	// Open our radar image, get filesize, and allocate buffer
	radarImage = fopen(radarFilePath, "rb");
	if (radarImage == NULL) {
		fprintf(stderr, "Unable to open file %s, skipping", radarFilePath);
		return 0;
	}
	fseek(radarImage, 0, SEEK_END);
	filesize = ftell(radarImage);
	fseek(radarImage, 0, SEEK_SET);
	radarData = (char *)malloc(sizeof(char) * filesize);

	// Read in image data
	n = fread(radarData, sizeof(char), filesize, radarImage);
	fclose(radarImage);

	// Insert data into the db
	rc = sqlite3_prepare_v2(db, "INSERT INTO imagedata (filename, radarimage) VALUES (?, ?)", -1, &ppStmt, 0);
	if (rc == SQLITE_OK && ppStmt != NULL)
	{
		sqlite3_bind_text(ppStmt, 1, radarFileName, -1, SQLITE_STATIC);
		sqlite3_bind_blob(ppStmt, 2, radarData, filesize, free);

		rc = sqlite3_step(ppStmt);
		rc = sqlite3_finalize(ppStmt);
		printf("radar image %s (%s) added to database\n", radarFilePath, radarFileName);
	}

	return 1;
}

void compactDB(bool renderHeapMap)
{
	// basic data types
	int signalPxCount, precipPxCount, stormPxCount;
	int oldSignal, oldPrecip, oldStorm, oldFlags;
	unsigned int dbzData[DBZ_COLORLIST_LENGTH];
	unsigned int plotID, imageID, dataPoint;
	unsigned int startRemovalID, endRemovalID;
	bool localDebug = false;

	// sqlite database
	int rc, rc2, rc3, id, flags;
	char *errMsg = 0, *zErrMsg = 0;
	sqlite3_stmt *ppStmt, *ppStmt2, *ppStmt3;
	unsigned int year, month, day, hour, minute;

	// stl
	list<idTimeStruct> radarDictList;
    list<idTimeStruct>::iterator radarDictIterator;
	idTimeStruct radarEntry;
	fixed_stack<idTimeStruct> radarStack(20);

	// regex
	regex_t re;
	size_t nmatch = 6;
	regmatch_t pmatch[6];

	// date/time
	char buffer[5];
	tm timeStruct;
	time_t ourTime, lastTime;

	// data files
	bool writeDataFiles = false;
	char dataFileFN[50], heatMapFN[50];
	FILE *dataFile = NULL;
	FILE *heatMapData = NULL;

	// Initialize values
	startRemovalID = 0;
	oldSignal = oldPrecip = oldStorm = oldFlags = 0;
	plotID = 0, imageID = 0, dataPoint = 0;

	memset(dbzData, 0, sizeof(unsigned int) * DBZ_COLORLIST_LENGTH);	

	// More of an time-domain analysis then compaction at this point, but hey... who's keeping track?
	printf("compacting DB...\n");

	char timestampRegx[] = "([0-9]{4})([0-9]{2})([0-9]{2})_([0-9]{2})([0-9]{2})";
	if (regcomp(&re, timestampRegx, REG_EXTENDED) != 0) {
		printf("Unable to compile regex, aborting...\n");
		exit(1);
	}
	printf("looking for %s\n", timestampRegx);

	char selectQuery[] = "SELECT id, timestamp FROM radar"; // LIMIT 1000";
	rc = sqlite3_prepare_v2(metaDB, selectQuery, -1, &ppStmt, 0);

	// If unable to read data, abort and exit
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);

		regfree(&re);
		sqlite3_close(imageDB);
		sqlite3_close(metaDB);
		exit(0);
	}

	while (rc = sqlite3_step(ppStmt) == SQLITE_ROW && !shutdownSignal)
	{
		id = sqlite3_column_int(ppStmt, 0);
		const unsigned char *ucTimestamp = sqlite3_column_text(ppStmt, 1);
		char *cTimestamp = (char *)ucTimestamp;

		int dbzValue;
		unsigned int pixelCount;

		int status = regexec(&re, cTimestamp, nmatch, pmatch, 0);

		if (status == REG_NOMATCH) {
			printf("no match on regex\n");
			continue;
		}
		if (status != 0) {
			printf("error: %d\n", status);
			continue;
		}

		year	= atoi(substr(buffer, cTimestamp, pmatch[1].rm_so, 4));
		month	= atoi(substr(buffer, cTimestamp, pmatch[2].rm_so, 2));
		day		= atoi(substr(buffer, cTimestamp, pmatch[3].rm_so, 2));
		hour	= atoi(substr(buffer, cTimestamp, pmatch[4].rm_so, 2));
		minute	= atoi(substr(buffer, cTimestamp, pmatch[5].rm_so, 2));

		timeStruct.tm_year	= year - 1900;
		timeStruct.tm_mon	= month - 1;
		timeStruct.tm_mday	= day;
		timeStruct.tm_hour 	= hour - 1;
		timeStruct.tm_min	= minute;
		timeStruct.tm_sec 	= 0;
		timeStruct.tm_isdst	= 0;					// Explicitly required

		lastTime = ourTime;
		ourTime = mktime(&timeStruct);

		char *selectFlagsQuery = sqlite3_mprintf("SELECT signalPxCount, precipPxCount, "
						"stormPxCount, flags FROM radar WHERE id=%d", id);
		rc2 = sqlite3_prepare_v2(metaDB, selectFlagsQuery, -1, &ppStmt2, 0);

		flags = -1;
		if (rc2 == SQLITE_OK && ppStmt2 != NULL)
		{
			rc2 = sqlite3_step(ppStmt2);
			if (rc2 == SQLITE_ROW)
			{
				oldSignal = signalPxCount;
				oldPrecip = precipPxCount;
				oldStorm = stormPxCount;
				oldFlags = flags;
				signalPxCount = sqlite3_column_int(ppStmt2, 0);
				precipPxCount = sqlite3_column_int(ppStmt2, 1);
				stormPxCount = sqlite3_column_int(ppStmt2, 2);
				flags = sqlite3_column_int(ppStmt2, 3);
				rc2 = sqlite3_finalize(ppStmt2);
			}
			else {
				printf("[err] sqlite3_step did not return valid data (%d) on query '%s',"
					"skipping id %d\n", rc2, selectFlagsQuery, id);
				continue;
			}
		}
		else {
			printf("[err] sqlite3_prepare_v2 did not return valid data (%d) on query '%s',"
				"skipping id %d\n", rc2, selectFlagsQuery, id);
			continue;
		}

		char *dbzDataQuery = sqlite3_mprintf("SELECT dbzValue, pixelCount FROM dbzData WHERE radarid=%d", id);
		rc3 = sqlite3_prepare_v2(metaDB, dbzDataQuery, -1, &ppStmt3, 0);

		if (rc3 == SQLITE_OK)
		{
			while (rc3 = sqlite3_step(ppStmt3) == SQLITE_ROW)
			{
				dbzValue = sqlite3_column_int(ppStmt3, 0);
				pixelCount = sqlite3_column_int(ppStmt3, 1);
				if (DBZ_COLORLIST_MIN < dbzValue && dbzValue < DBZ_COLORLIST_MAX)
				{
					dbzData[DBZFull2Index(dbzValue)] = pixelCount;
 					if (localDebug) printf("dbzData[%d]=%d\n", DBZFull2Index(dbzValue), pixelCount);
				}
			}
		}
		else {
			printf("[err] sqlite3_prepare_v2 did not return valid data (%d) on query '%s',"
				"skipping id %d\n", rc2, dbzDataQuery, id);
			continue;
		}

		radarEntry.id = id;
		radarEntry.time = timeStruct;
		radarStack.push_front(radarEntry);

		if (flags > 0)
		{
			// This image marked for deletion?
			if (flags & FLAG_RADAR_DELETE)
			{
				// Is this the first image in the chronological set to be removed?
				// eg: was the previous image NOT marked for removal?
				if (startRemovalID == 0)
				{
					// Start a new 'removal' set
					startRemovalID = id;
// 					printf("start of removal found at ID %d\n");
					printf(CLR_DEF);
					printf(">>>\n", id);
// 					radarEntry.id = id;
// 					radarEntry.time = timeStruct;
// 					radarDictList.push_back(radarEntry);
				}
				// This image is marked for deletion but so was the previous one
				// so we are in a set of images marked for removal
				else if (startRemovalID != 0)
				{
					/*radarEntry.id = id;
					radarEntry.time = timeStruct;
					radarDictList.push_back(radarEntry);*/
				}
			}
		}
		// Image not marked for removal
		else if (flags <= 0)
		{
			// However, previous image was marked
			if (startRemovalID != 0)
			{
				// Remove between startRemovalID and current ID
				// Reset startRemovalID to zero (null/false)
				printf("<<<\n");
// 					printf("removal found between IDs %d and %d\n", startRemovalID, id-1);
				startRemovalID = 0;
			}
		}

		if (stormPxCount >= THRESH_RADAR_PX_STORM)					// 25
			printf(CLR_R);
		else if (precipPxCount >= THRESH_RADAR_PX_PRECIP_STORM)		// 200
			printf(CLR_Y);
		else if (signalPxCount >= THRESH_RADAR_PX_PRECIP_RAW)		// 1000
			printf(CLR_G);
		else if (signalPxCount >= THRESH_RADAR_PX_SIGNAL)			// 100
			printf(CLR_BG);
		else
			printf(CLR_W);

		if (abs(int(ourTime) - int(lastTime)) > 4000)
		{
			printf("\n\n\n");
			imageID = oldSignal = oldPrecip = oldStorm = oldFlags = 0;

			if (writeDataFiles)
			{
				if (dataFile != NULL) fclose(dataFile);
				if (heatMapData != NULL) fclose(heatMapData);

				sprintf(dataFileFN, "gnuplot-%d.dat", plotID);
				sprintf(heatMapFN, "heatmap-%d.dat", plotID++);
				printf("using datafiles %s/%s\n", dataFileFN, heatMapFN);
				dataFile = fopen(dataFileFN, "w");
				heatMapData = fopen(heatMapFN, "w");
			}
		}

		if (dataFile != NULL && writeDataFiles)
			fprintf(dataFile, "%d %d %d %d\n", (int)ourTime, stormPxCount, precipPxCount, signalPxCount);

		for (int dbz=DBZ_COLORLIST_MIN; dbz<=DBZ_COLORLIST_MAX; dbz+=DBZ_COLORLIST_STEP)
		{
			if (heatMapData != NULL && writeDataFiles)
				fprintf(heatMapData, "%d %d %d\n", imageID, dbz, dbzData[DBZFull2Index(dbz)]);
			printf("%d ", dbzData[DBZFull2Index(dbz)]);
		}
		printf("\n");
		if (writeDataFiles) fprintf(heatMapData, "\n");

		if (startRemovalID == 0)
			printf("    ");
		else
			printf("[x] ");

		printf("ID %d ", id);
		printf("[ %04d-%02d-%02d %02d:%02d ] epoch=%d, delta=%d %s(%s%d %s%d %s%d %s%d)",
			year, month, day, hour, minute, (int)ourTime, int(ourTime) - int(lastTime),
			CLR_W, CLR_R, stormPxCount, CLR_Y, precipPxCount, CLR_BG, signalPxCount, CLR_W, flags);
		printf(CLR_W);
		printf(" (%d %d %d)\n", stormPxCount-oldStorm, precipPxCount-oldPrecip, signalPxCount-oldSignal);

		imageID++;
	}

	rc = sqlite3_finalize(ppStmt);		// TODO: finalize other ppStmt's??
	if (writeDataFiles) {
		fclose(dataFile);
		fclose(heatMapData);
	}

	// tidy up
	regfree(&re);
// 	sqlite3_close(imageDB);
// 	sqlite3_close(metaDB);
// 	exit(0);
}

int processImage(const char *radarfile, sqlite3 *metaDB, sqlite3 *radarDB, bool skipExisting, bool renderHeapMap = false)
{
    // basic data types
	register unsigned int x, y;
    unsigned int i, xMax, yMax, size, flags = 0;
	unsigned int stormCellCount, stormPixelCount, precipPixelCount, signalPixelCount;
	unsigned int dbzPixelCount[DBZ_COLORLIST_LENGTH];
	register int pixelGDIndex;
	int pixelIndex, pixelDBZ, pixelFullDBZ, pixelFullIndex;			// Negative DBZ values are permitted!
	int pixelR, pixelG, pixelB, pixelA;
	float xCenter, yCenter;

    // stl
	string cacheOutput, radarOutput, radarPathStr;
	list<cartesianPair> pixelList;
    list<cartesianPair>::iterator pixelIterator;
	list<stormCell> cellList;
    list<stormCell>::iterator cellIterator;

    // radarwatch structs
	cartesianPair xy, xySum;
	stormCell cell;

	// storage: sqlite/files
	FILE *radarFile;
	sqlite3_stmt *ppStmt;
	char station[4], timestamp[14], type[4];
	char *errMsg = 0;
	int rc, id = 0;

	// DIX_20090612_0208_NCR.gif
	// Make a pointer to the filename proper, sans any path
	char *radarFileName = (char *)strrchr(radarfile, '/') + 1;
	string sRadarWildcard = radarfile;
	sRadarWildcard.replace(sRadarWildcard.size()-7, 3, "???");

	// Extract station and timestamp from filename
	strncpy(station, radarFileName, 3);
	strncpy(timestamp, radarFileName+4, 13);
	strncpy(type, radarFileName+18, 3);
	timestamp[13] = '\0';
	station[3] = '\0';
	type[3] = '\0';


	// Search our radar table for an existing match
	char *searchQuery = sqlite3_mprintf("SELECT id FROM radar WHERE station='%s'"
			" AND timestamp='%s' AND type='%s'", station, timestamp, type);
	rc = sqlite3_prepare_v2(metaDB, searchQuery, -1, &ppStmt, 0);
	if (rc == SQLITE_OK && ppStmt != NULL)
	{
		rc = sqlite3_step(ppStmt);
		if (rc == SQLITE_ROW)
		{
			id = sqlite3_column_int(ppStmt, 0);
			rc = sqlite3_finalize(ppStmt);

			if (skipExisting)
			{
				printf("Radar image %s exists in meta db (id: %d) and flag set, skipping\n", radarFileName, id);
				return ERROR_SKIP_IMAGE;
			}
		}
	}

	radarFile = fopen(radarfile, "rb");
	if (!radarFile) {
		fprintf(stderr, "Can't open radar source image '%s', aborting...\n", radarfile);
        return ERROR_CANT_OPEN;
    }

    // Load our image and prepare to analyze
    radarImage = gdImageCreateFromGif(radarFile);
	if (!radarImage) {
		fprintf(stderr, "Radar source image not GIF compliant, aborting...\n");
		fclose(radarFile);
		return ERROR_NON_GIF;
	}

	stormCellCount = stormPixelCount = precipPixelCount = signalPixelCount = 0;
	memset(dbzPixelCount, 0, sizeof(unsigned int) * DBZ_COLORLIST_LENGTH);
    xMax = gdImageSX(radarImage);
    yMax = gdImageSY(radarImage);

    // Allocate memory for our ID array
	// This is a static variable, heap buffer will be reused instead of reallocation
    if (cellIDMatrix == NULL)
	{
		cellIDMatrix = (unsigned int **) malloc(xMax * sizeof(unsigned int *));
		if (!cellIDMatrix) {
			fprintf(stderr, "Unable to allocate memory for ID matrix, aborting...\n");
			free(cellIDMatrix);
			exit(-1);
		}
		for (i = 0; i < yMax; i++) {
			cellIDMatrix[i] = (unsigned int *) malloc(yMax * sizeof(unsigned int));
			if (!cellIDMatrix[i]) {
				fprintf(stderr, "Unable to allocate memory for ID matrix, aborting...\n");
				free(cellIDMatrix[i]);
				exit(-1);
			}
			memset(cellIDMatrix[i], 0, yMax * sizeof(unsigned int));
		}
	}
	else
	{
		for (i = 0; i < yMax; i++)
		{
			if (cellIDMatrix[i] != NULL)
				memset(cellIDMatrix[i], 0, yMax * sizeof(unsigned int));
			else
			{
				cellIDMatrix[i] = (unsigned int *) malloc(yMax * sizeof(unsigned int));
				if (!cellIDMatrix[i]) {
					fprintf(stderr, "Unable to allocate memory for ID matrix, aborting...\n");
					free(cellIDMatrix[i]);
					exit(-1);
				}
				memset(cellIDMatrix[i], 0, yMax * sizeof(unsigned int));
			}
		}
	}

	/** Get transparent color.  If not found, default to last color in indexed image
		NEXRAD radar images typically uses this for transparency **/
	int transparent = gdImageGetTransparent(radarImage);
	if (!transparent) transparent = gdImageColorsTotal(radarImage);

    // Enumerate storm pixels
    for (y = 0;	y < yMax; y++)
    {
        for (x = 0; x < xMax; x++)
        {
			// These next 6 lines consume the majority of CPU time
            pixelGDIndex = gdImageGetPixel(radarImage, x, y);
            pixelR = radarImage->red[pixelGDIndex];
            pixelG = radarImage->green[pixelGDIndex];
            pixelB = radarImage->blue[pixelGDIndex];
            pixelA = gdImageAlpha(radarImage, pixelGDIndex);

            // Skip empty data (transparent pixels)
            if (pixelR == 255 && pixelG == 255 && pixelB == 255 && pixelA == 127)
                continue;

// 			pixelIndex = GetPixelDBZIndex(x, y);
// 			pixelDBZ = Index2DBZ(pixelIndex);
			pixelFullIndex = GetFullPixelDBZIndex(x, y);
			pixelFullDBZ = Index2DBZFull(pixelFullIndex);

			/*if ((pixelFullDBZ != pixelDBZ) && (pixelFullDBZ >= 20)) {
				printf("[err] %d != %d [%d] @ (%d, %d)\n", pixelFullDBZ, pixelFullIndex, x, y);
			}*/

			if (debug)
                printf("%d/%d (%d,%d,%d)\n", pixelFullIndex, pixelFullDBZ, pixelR, pixelG, pixelB);

			// Remove weak signals / noise from radar image
			if (pixelFullDBZ < THRESH_DBZ_LIGHT_PRECIP) {
				gdImageSetPixel(radarImage, x, y, transparent);
			}
            /*for (i = 1; i <= sizeof(weakSignals)/(3*sizeof(int)); i++) {
                if (CheckListColor(i, r, g, b, weakSignals)) {
                    gdImageSetPixel(radarImage, x, y, transparent);
                }
            }*/

			dbzPixelCount[pixelFullIndex]++;

			if (pixelFullDBZ >= THRESH_DBZ_LIGHT_PRECIP)
				signalPixelCount++;
			if (pixelFullDBZ >= THRESH_DBZ_MODERATE_PRECIP)
				precipPixelCount++;
            if (pixelFullDBZ >= THRESH_DBZ_TSTORM) {
                xy.x = x;			xy.y = y;
                xySum.x += x;		xySum.y += y;
                stormPixelCount++;
                pixelList.push_back(xy);
            }
		}
	}

	// #define THRESH_RADAR_PX_PRECIP_RAW				1000
	// #define THRESH_RADAR_PX_PRECIP_STORM				200
	// #define THRESH_RADAR_PX_STORM					25
	// TODO: Replace this with a general heuristic rather then strict thresholds
	// Create a probability function, maybe fuzzy logic??
	if ((( precipPixelCount < THRESH_RADAR_PX_PRECIP_RAW && stormPixelCount == 0 ) ||
		( precipPixelCount < THRESH_RADAR_PX_PRECIP_STORM && stormPixelCount < THRESH_RADAR_PX_STORM ))
		&& FLAG_REMOVE_EMPTY_IMAGES)
	{
		char *cpRadarWildcard = (char *)sRadarWildcard.c_str();
		printf("\nradar analysis revealed no storms (%d signal, %d precip, %d storm), marking image for removal\n",
			   signalPixelCount, precipPixelCount, stormPixelCount);
		printf("removing radar images matching wildcard '%s'\n", cpRadarWildcard);

		flags |= FLAG_RADAR_DELETE;
		if (stormPixelCount < THRESH_RADAR_PX_NOSTORM)
			flags |= FLAG_RADAR_NO_STORMS;
		if (signalPixelCount < THRESH_RADAR_PX_SIGNAL)
			flags |= FLAG_RADAR_NO_PRECIP;

		markedDeletionCount++;
// 		removedImagesCount++;
// 		unlink(radarWildcard);
// 		unlink(radarfile);

		// Remove any cached radar image data
 		/*char *deleteRadarQuery = sqlite3_mprintf("DELETE FROM imagedata WHERE filename='%s'", radarFileName);
 		rc = RunQuery(imageDB, deleteRadarQuery);
 		fclose(radarFile);
 		gdImageDestroy(radarImage);
 		return 0;*/
	}

	// Enumerate our storm cell list
    for (pixelIterator = pixelList.begin(); pixelIterator != pixelList.end(); pixelIterator++)
    {
        xy = *pixelIterator;
        x = xy.x; 		  y = xy.y;
		/*printf("P %X...", *pixelIterator);
		printf("C (%d,%d)...", x, y);*/

        if (cellIDMatrix[y][x] == 0)
        {
            cell.size = PaintCell(xy, ++stormCellCount);
            cell.id = stormCellCount;
            cell.xy = xy;
            cellList.push_back(cell);
        }
    }

	// Search our radar table for an existing match
 	char *selectQuery = sqlite3_mprintf("SELECT id FROM radar WHERE station='%s'"
			" AND timestamp='%s' AND type='%s'", station, timestamp, type);
	rc = sqlite3_prepare_v2(metaDB, selectQuery, -1, &ppStmt, 0);
	if (rc == SQLITE_OK && ppStmt != NULL)
	{
		rc = sqlite3_step(ppStmt);
		if (rc == SQLITE_ROW)
		{
			// We found existing entry, update it
			id = sqlite3_column_int(ppStmt, 0);
			rc = sqlite3_finalize(ppStmt);

			printf("updating radar id %d entry with latest\n", id);

			char *updateQuery = sqlite3_mprintf("UPDATE radar SET station=%Q, timestamp=%Q, type=%Q, "
				"fileName=%Q, flags=%d, signalPxCount=%d, precipPxCount=%d, stormPxCount=%d, cellCount=%d "
				"WHERE id = %d", station, timestamp, type, radarFileName, flags, signalPixelCount, precipPixelCount,
				stormPixelCount, stormCellCount, id);
			rc = RunQuery(metaDB, updateQuery);
			sqlite3_free(updateQuery);

			char *dbzClearQuery = sqlite3_mprintf("DELETE FROM dbzData WHERE radarid=%d", id);
			rc = RunQuery(metaDB, dbzClearQuery);
			sqlite3_free(dbzClearQuery);
		}
		else
		{
			printf("inserting new radar data entry\n");
			rc = sqlite3_finalize(ppStmt);

			// No entry found, insert new
			char *insertQuery = sqlite3_mprintf("INSERT INTO radar (station, timestamp, type, fileName,"
				" flags, signalPxCount, precipPxCount, stormPxCount, cellCount) VALUES (%Q, %Q, %Q, %Q, %d, %d, %d, %d, %d)",
				station, timestamp, type, radarFileName, flags, signalPixelCount, precipPixelCount, stormPixelCount, stormCellCount);
			rc = RunQuery(metaDB, insertQuery);
			sqlite3_free(insertQuery);

			// Get the id of our newly inserted entry
			char *selectNewQuery = sqlite3_mprintf("SELECT id FROM radar WHERE station='%s'"
						" AND timestamp='%s' AND type='%s'", station, timestamp, type);
			rc = sqlite3_prepare_v2(metaDB, selectNewQuery, -1, &ppStmt, 0);

			if (rc == SQLITE_OK && ppStmt != NULL)
			{
				rc = sqlite3_step(ppStmt);
				if (rc == SQLITE_ROW)
				{
					id = sqlite3_column_int(ppStmt, 0);
					rc = sqlite3_finalize(ppStmt);
				}
				else {
					id = 0;
					fprintf(stderr, "SQL error: on SELECT (%s) new inserted data sqlite3_step returned %d\n", selectNewQuery, rc);
				}
			}
			else {
				id = 0;
				fprintf(stderr, "SQL error: on SELECT (%s) new inserted data sqlite3_prepare returned %d\n", selectNewQuery, rc);
			}

			sqlite3_free(selectNewQuery);
		}
	}
	else {
		fprintf(stderr, "SQL error, on SELECT (%s) search sqlite_prepare returned %d. ppStmt=0x%p\n", selectQuery, rc, ppStmt);
		sqlite3_finalize(ppStmt);
	}
	sqlite3_free(selectQuery);

	// Update/insert DBZ data for radar image
	printf("inserting DBZ data: ");

	// Loop through DBZ data and add to db
	RunQuery(metaDB, "BEGIN");
	for (int i=1; i<DBZ_COLORLIST_LENGTH, Index2DBZFull(i)<=THRESH_DBZ_MAX_SIGNAL; i++)
	{
		char *dbzQuery = sqlite3_mprintf("INSERT INTO dbzData (radarid, dbzValue, pixelCount) VALUES (%d, %d, %d)",
			id, Index2DBZFull(i), dbzPixelCount[i]);
		printf("%d=%d ", Index2DBZFull(i), dbzPixelCount[i]);
		rc = RunQuery(metaDB, dbzQuery);
		sqlite3_free(dbzQuery);
	}
	RunQuery(metaDB, "COMMIT");
	printf("\n");

	// If no storms (marked for deletion), close file and abort
	if (flags & FLAG_RADAR_DELETE) {
		fclose(radarFile);
		gdImageDestroy(radarImage);
		return 0;
	}

	printf("cell data:");

	// Assuming we have a valid radarid, update the celldata table with our information
	if (id != 0)
	{
		// Clear out any existing storm cell data for this radar file
		char *deleteQuery = sqlite3_mprintf("DELETE FROM celldata WHERE radarid=%d", id);
		rc = RunQuery(metaDB, deleteQuery);
		sqlite3_free(deleteQuery);

		// Loop through our cell list and insert into the db
		RunQuery(metaDB, "BEGIN");
		for (cellIterator = cellList.begin(); cellIterator != cellList.end(); cellIterator++)
		{
	        cell = *cellIterator;
	        xy = cell.xy;

 			char *cellInsertQuery = sqlite3_mprintf("INSERT INTO celldata (cellid, radarid, size, x, y)"
				"VALUES (%d, %d, %d, %d, %d)", cell.id, id, cell.size, xy.x, xy.y);
			rc = RunQuery(metaDB, cellInsertQuery);
			sqlite3_free(cellInsertQuery);

			printf("\ncell ID %d @ (%d,%d), size = %d", cell.id, xy.x, xy.y, cell.size);
	    }
		RunQuery(metaDB, "COMMIT");
	}
	else {
		fprintf(stderr, "SQL error: INSERT/UPDATE radar table did not return an id, unable to add/update cell table\n");
	}

	// Release our memory
	cellList.clear();
	pixelList.clear();
    fclose(radarFile);
	gdImageDestroy(radarImage);

	printf("\nsignal count: %d\nprecip count: %d\nstorm pixel count: %d\nstorm cell count: %d\n",
		   signalPixelCount, precipPixelCount, stormPixelCount, stormCellCount);
	return stormCellCount;
}

int main(int argc, char *argv[])
{
	// files/directory
	unsigned int totalFileCount, fileMatchCount, fileCheckedCount, processedFileCount, addedFileCount;
	unsigned int skippedImages, erroredImages, removedErroredImages;
	string radarOutput, radarPathStr, radarfile, fullradarfile;
	FILE *radarFile, *dataCache, *imageCache;
	DIR *radarDir;
	struct dirent *entry;
	list<unsigned int> epoch, epochPrime;
	list<string> radarFileList;

	// regex
	regex_t re;
	size_t nmatch = 6;
	regmatch_t pmatch[6];

	// date/time
	char buffer[5];
	tm timeStruct;
	time_t ourTime, lastTime;
	clock_t beginAnalysisClock, endAnalysisClock, beginImageClock, endImageClock, skippedTime = 0;

	// sqlite3 database
	int rc;
	char *errMsg = 0;

	// getopt
	int c;

	// assign default values
	bool dryRun = false;
	bool renderHeapMap = false;
	bool flagCompactDB = false;
	bool removedErrored = false;
	bool skipExisting = SKIP_EXISTING_RADAR;
	char *radarPath = (char *)RADAR_PATH;
	char *radarImage = 0;
	char *station = (char *)RADAR_STATION;
	string dbPath = SQLITE_DB_PATH;

	// Catch SIGINT to shutdown app gracefully - nice feature IMHO!
	signal(SIGINT, signalHandler);

	// verify arguments
	if (argc < 2) {
		fprintf(stderr, "Usage: %s [-r radar path | [-i radar image] [-d database path] [-s radar station] [-u] [-y] [-h]\n\n", argv[0]);
		fprintf(stderr, "radar path: path to radar image directory\n");
		fprintf(stderr, "radar image: single radar image to process\n");
		fprintf(stderr, "database path: path to sqlite database\n");
	    fprintf(stderr, "radar station : radar station/id to search for in radar path\n");
		fprintf(stderr, "-u: update existing radar entires (default: skip)\n");
		fprintf(stderr, "-e: remove errored files (non-GIF compliant)\n");
		fprintf(stderr, "-c: compact database, analyze storms in time domain and flag for removal\n");
		fprintf(stderr, "-h: heat map.  generate heatmap via gnuplot of DBZ data from radar images\n");
		fprintf(stderr, "-y: drY run, display values and quit\n");
	    exit(1);
	}

	// parse args
	while ((c = getopt(argc, argv, "r:d:s:i:uyechD")) != -1)
	{
		switch (c) {
		case 'r':
			radarPath = optarg;
			break;
		case 'i':
			radarImage = optarg;
			break;
		case 'd':
			dbPath = optarg;
			break;
		case 's':
			station = optarg;
			break;
		case 'u':
			skipExisting = false;
			break;
		case 'e':
			removedErrored = true;
			break;
		case 'y':
			dryRun = true;
			break;
		case 'c':
			flagCompactDB = true;
			break;
		case 'h':
			renderHeapMap = true;
			break;
		case 'D':
			debug = true;
			break;
		case '?':
			fprintf(stderr, "Unrecognized option: -%c\n", optopt);
		}
	}

	// Prep our paths
	radarPathStr = radarPath;
 	if (radarPathStr.substr(radarPathStr.length(), 1) != "/")
 		radarPathStr += "/";

	string metaDbPath = dbPath + "/" + SQLITE_META_DB;
	string radarDbPath = dbPath + "/" + SQLITE_IMAGE_DB;

	printf("using radar path: %s\n", radarPath);
	printf("using radar station: %s\n", station);
	printf("using database path: %s\n", dbPath.c_str());
	printf("skip existing images: %d\n\n", skipExisting);
	if (dryRun) { exit(0); }

	// Start the clock
	beginAnalysisClock = clock();

	// Open our databases
	if (sqlite3_open(metaDbPath.c_str(), &metaDB) != SQLITE_OK) {
		fprintf(stderr, "Can't open database\n");
		sqlite3_close(metaDB);
		exit(1);
	}
	if (sqlite3_open(radarDbPath.c_str(), &imageDB) != SQLITE_OK) {
		fprintf(stderr, "Can't open database\n");
		sqlite3_close(imageDB);
		exit(1);
	}

	// Create tables if needed
	rc = RunQuery(metaDB, "CREATE TABLE IF NOT EXISTS radar (id INTEGER PRIMARY KEY, "
			"station TEXT, timestamp TEXT, type TEXT, fileName TEXT, flags INTEGER, "
			"signalPxCount INTEGER, precipPxCount INTEGER, stormPxCount INTEGER, cellCount INTEGER)");
	rc = RunQuery(metaDB, "CREATE TABLE IF NOT EXISTS dbzData (radarid INTEGER, "
			"dbzValue INTEGER, pixelCount INTEGER)");
	rc = RunQuery(metaDB, "CREATE TABLE IF NOT EXISTS celldata (cellid INTEGER, "
			"radarid INTEGER, size INTEGER, x INTEGER, y INTEGER, strength DOUBLE, "
			"xCenter DOUBLE, yCenter DOUBLE, flags INTEGER)");
	rc = RunQuery(imageDB, "CREATE TABLE IF NOT EXISTS imagedata (id INTEGER PRIMARY KEY, "
			"filename TEXT, flags INTEGER, radarimage BLOB)");

	if (radarImage) {
		processImage(radarImage, metaDB, imageDB, skipExisting, false);
		exit(0);
	}

	if (flagCompactDB) {
		compactDB(renderHeapMap);
		sqlite3_close(imageDB);
		sqlite3_close(metaDB);
		exit(0);
	}

	// Prep our regex's
	string radarFileRegx = station;
	radarFileRegx += "_([0-9]{4})([0-9]{2})([0-9]{2})_([0-9]{2})([0-9]{2})_";
	radarFileRegx += RADAR_TYPE;
	radarFileRegx += ".gif";
	if (regcomp(&re, radarFileRegx.c_str(), REG_EXTENDED) != 0) {
		printf("Unable to compile regex, aborting...\n");
		exit(1);
	}

	// Open radar path and read in file list
	radarDir = opendir(radarPath);
    if (radarDir == NULL) {
        fprintf(stderr, "Unable to open directory %s, aborting\n", radarPathStr.c_str());
        exit(1);
    }

	// Loop through file list, if file name matches regex add to list
	totalFileCount = fileMatchCount = fileCheckedCount = processedFileCount = addedFileCount = 0;
	skippedImages = erroredImages = removedErroredImages = 0;
    while (entry = readdir(radarDir))
	{
		totalFileCount++;
		int status = regexec(&re, entry->d_name, nmatch, pmatch, 0);
		if (status == 0 && fileMatchCount < MAX_FILES)
		{
			fileMatchCount++;
			radarfile = entry->d_name;
			radarFileList.push_back(radarfile);
		}
	}

	// tidy up
	regfree(&re);
    closedir(radarDir);

	/* This is absolutely vital! */
	radarFileList.sort();

	printf("read in files, %d/%d found\n", fileMatchCount, totalFileCount);

	// Iterate through radar files and process them
	while(!radarFileList.empty() && !shutdownSignal)
	{
		beginImageClock = clock();

		radarfile = radarFileList.front();
		radarFileList.pop_front();

		unsigned int year	= atoi(substr(buffer, radarfile.c_str(), pmatch[1].rm_so, 4));
		unsigned int month	= atoi(substr(buffer, radarfile.c_str(), pmatch[2].rm_so, 2));
		unsigned int day	= atoi(substr(buffer, radarfile.c_str(), pmatch[3].rm_so, 2));
		unsigned int hour	= atoi(substr(buffer, radarfile.c_str(), pmatch[4].rm_so, 2));
		unsigned int minute	= atoi(substr(buffer, radarfile.c_str(), pmatch[5].rm_so, 2));

		timeStruct.tm_year	= year - 1900;
		timeStruct.tm_mon	= month;
		timeStruct.tm_mday	= day;
		timeStruct.tm_hour 	= hour;
		timeStruct.tm_min	= minute;
		timeStruct.tm_sec 	= 0;

		lastTime = ourTime;
		ourTime = mktime(&timeStruct);
		fullradarfile = radarPathStr + radarfile;
		epoch.push_back(ourTime);

		printf("\n\nradar image : %d/%d\n", ++fileCheckedCount, fileMatchCount);
		printf("source image : %s\n", fullradarfile.c_str());
		printf("timestamp : %04d-%02d-%02d %02d:%02d, epoch = %d\n", year, month, day, hour, minute, (int)ourTime);

		int ret = processImage(fullradarfile.c_str(), metaDB, imageDB, skipExisting, renderHeapMap);

		// Error with image, do not count processing time
		if (ret <= ERROR_CANT_OPEN)
		{
			endImageClock = clock();
			skippedTime += (endImageClock - beginImageClock);
			erroredImages++;

			// If the file is non GIF-compliant and set for removal-on-error then do so
			if (ret == ERROR_NON_GIF && removedErrored)
			{
				printf("removing non GIF-compliant file %s\n", fullradarfile.c_str());		// TODO: logging - to file?
				unlink(fullradarfile.c_str());
				removedErroredImages++;
			}
		}
		// Image already in db & set to skip, again do not count processing time
		else if (ret == ERROR_SKIP_IMAGE) {
			endImageClock = clock();
			skippedTime += (endImageClock - beginImageClock);
			skippedImages++;
		}
		else if (ret == 0) {
			// What do we do here?
			// This means processing was OK, just no storm activity detected in image
			// Probably should just add a new counter and increment
		}
		// Processed OK!
		else if (ret > 0) {
			processedFileCount++;
 			if (addRadarToDB(fullradarfile.c_str(), imageDB) > 0) addedFileCount++;
		}
    }

	endAnalysisClock = clock();
	double totalTimeElapsed = ((double) (endAnalysisClock - beginAnalysisClock - skippedTime)) / CLOCKS_PER_SEC;

	sqlite3_close(imageDB);
	sqlite3_close(metaDB);

	printf("\nanalysis complete");
	printf("\n%d radar images analyzed", fileCheckedCount);
	printf("\n%d images processed (%d added to database)", processedFileCount, addedFileCount);
	printf("\n%d existing radar images skipped", skippedImages);
	printf("\n%d non-storm radar images removed (%d marked for removal)", removedImagesCount, markedDeletionCount);
	printf("\n%d radar images encountered errors (%d removed)", erroredImages, removedErroredImages);
	printf("\n%.02f total seconds elapsed (avg processed %.02f images/sec)\n", totalTimeElapsed, ((fileCheckedCount-skippedImages)/totalTimeElapsed));
}

void signalHandler(int signum)
{
	fprintf(stderr, "Caught SIGINT, cleaning up and exiting...\n");
	shutdownSignal = true;
}