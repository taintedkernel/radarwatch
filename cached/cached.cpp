
#include <list>
#include <stack>
#include <string>
#include <cassert>

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

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

#include <pthread.h>
#include <zmq.hpp>

#include "zhelpers.hpp"
#include "cached.h"


using namespace std;


// Storm ID matrix used for identifying individual storms
static unsigned int **cellIDMatrix = NULL;

// Our source radar image
//gdImagePtr radarGdImage;

// SQLite database
//sqlite3 *metaDB, *imageDB;
//cl_radarDB radarDB;

// Keep track of how many storm-free radar images were removed
unsigned int removedImagesCount = 0;
unsigned int markedDeletionCount = 0;

// Debugging
bool debug = false;
volatile bool shutdownSignal = false;

// Clocks/timing
clock_t start, end;
double enumPixelsClock, enumCellsClock, radarClock, dbzClock, cellClock, metaClock;


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

/*static int callback(void *NotUsed, int argc, char **argv, char **azColName){
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
}*/


int cl_radarDB::openDBs(const char *metaDBname, const char *imageDBname)
{
	// Open databases
	if (sqlite3_open(metaDBname, &metaDB) != SQLITE_OK) {
		fprintf(stderr, "Can't open database %s\n", metaDBname);
		shutdownApplication(true, 1);
		return -1;
	}
	if (sqlite3_open(imageDBname, &imageDB) != SQLITE_OK) {
		fprintf(stderr, "Can't open database %s\n", imageDBname);
		shutdownApplication(true, 1);
		return -1;
	}

	// Create tables if needed
	RunQueryMeta("CREATE TABLE IF NOT EXISTS radar (id INTEGER PRIMARY KEY, "
			"station TEXT, timestamp TEXT, type TEXT, fileName TEXT UNIQUE, flags INTEGER, "
			"signalPxCount INTEGER, precipPxCount INTEGER, stormPxCount INTEGER, "
			"cellCount INTEGER, radarGroup INTEGER)", true);
	RunQueryMeta("CREATE TABLE IF NOT EXISTS dbzData (radarid INTEGER, "
			"dbzValue INTEGER, pixelCount INTEGER, UNIQUE(radarid, dbzValue))", true);
	RunQueryMeta("CREATE TABLE IF NOT EXISTS celldata (cellid INTEGER, "
			"radarid INTEGER, size INTEGER, dbzSigma INTEGER, dbzAvg FLOAT, dbzStdDev FLOAT, "
			"dbzMax INTEGER, x INTEGER, y INTEGER, xCenter DOUBLE, yCenter DOUBLE, flags INTEGER, "
			"UNIQUE (cellid, radarid))", true);
	RunQueryMeta("CREATE TABLE IF NOT EXISTS radarmeta (radarid INTEGER, "
			"datatype INTEGER, value INTEGER, UNIQUE (radarid, datatype))", true);
	RunQueryImage("CREATE TABLE IF NOT EXISTS imagedata (id INTEGER PRIMARY KEY, "
			"radarid INTEGER, filename TEXT, flags INTEGER, radarimage BLOB)", true);

	// Index dbzData & cellData tables if needed, tables radar/imagedata already indexed through PK
	RunQueryMeta("CREATE INDEX IF NOT EXISTS radarFileIndex ON radar (fileName)", true);
	RunQueryMeta("CREATE INDEX IF NOT EXISTS dbzIndex ON dbzData (radarid, dbzValue)", true);
	RunQueryMeta("CREATE INDEX IF NOT EXISTS cellIndex ON celldata (cellid, radarid)", true);
	RunQueryMeta("CREATE INDEX IF NOT EXISTS metaIndex ON radarmeta (radarid, datatype)", true);
	RunQueryImage("CREATE INDEX IF NOT EXISTS idIndex ON imagedata (id, radarid)", true);

	// Tweak some more performance out of SQLite
	// This can cause db corruption if power is lost but since we keep it in /dev/shm
	// for the time being it will be lost regardless....  AAAANNNDD it's GONE!
	RunQueryMeta("PRAGMA synchronous = OFF");
	RunQueryMeta("PRAGMA journal_mode = MEMORY");

	sqlite3_prepare_v2(metaDB, "INSERT OR REPLACE INTO celldata (cellid, radarid, size, dbzSigma, dbzAvg, dbzMax, dbzStdDev,"
		"x, y, xCenter, yCenter) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", -1, &celldataInsertStmt, 0);
	sqlite3_prepare_v2(metaDB, "INSERT OR REPLACE INTO dbzData (radarid, dbzValue, pixelCount) VALUES (?, ?, ?)", -1,
		&dbzdataInsertStmt, 0);

	init = true;
	return 0;
}

int cl_radarDB::RunQueryMeta(const char *query, bool onAbort = false) {
	return RunQueryGeneric(metaDB, query, onAbort);
}

int cl_radarDB::RunQueryImage(const char *query, bool onAbort = false) {
	return RunQueryGeneric(imageDB, query, onAbort);
}

//int cl_radarDB::RunQueryGeneric(sqlite3 *db, const char *query, bool onAbort = false)
int cl_radarDB::RunQueryGeneric(sqlite3 *db, const char *query, bool onAbort)
{
	char *errMsg;
	fflush(stdout);
	fflush(stderr);

	if (query == NULL) {
		fprintf(stderr, "[err] SQL: unable to create query: %s, aborting\n", query);
		return -1;
	}

	int rc = sqlite3_exec(db, query, NULL, 0, &errMsg);
	if (rc == SQLITE_OK)
		return rc;
	else if (rc == SQLITE_CANTOPEN)
	{
		fprintf(stderr, "[err] SQL error: %s on query %s, rc: %d, retrying\n", errMsg, query, rc);
		int attempt = 1;
		while (attempt++ < 3) {
			fprintf(stderr, "retrying query %s, attempt %d\n", query, attempt);
			rc = sqlite3_exec(db, query, NULL, 0, &errMsg);
			if (rc == SQLITE_OK) return rc;
			fprintf(stderr, "[err] SQL error: %s on query %s, rc: %d, retrying\n", errMsg, query, rc);
		}
	}
	else if (rc != SQLITE_OK)
		fprintf(stderr, "[err] SQL error: %s on query %s, rc: %d\n", errMsg, query, rc);

	if (onAbort) {
		fprintf(stderr, "unable to execute query, fatal error, aborting");
		exit(-1);
	}

	return rc;
}

//int cl_radarDB::GetQueryResultsMeta(char *query)
//void cl_radarDB::GetQueryResultsMeta(char *query, st_clRadarDbRes &result, bool onAbort = false)
// flags:
//		0 - display errors, abort
// 		1 - display errors, continue
//		2 - display errors except non-row, continue

void cl_radarDB::GetQueryResultsMeta(char *query, st_clRadarDbRes &result, unsigned int option = OPT_QUERY_CONTONERR)
{
	int rc;
	static sqlite3_stmt *ppStmt;

	fflush(stdout);
	fflush(stderr);

	result.data = -1;
	result.rc = sqlite3_prepare_v2(metaDB, query, -1, &ppStmt, 0);

	if (result.rc == SQLITE_OK && ppStmt != NULL)
	{
		result.rc = sqlite3_step(ppStmt);
		if (result.rc == SQLITE_ROW)
		{
			result.data = sqlite3_column_int(ppStmt, 0);
			result.rc = sqlite3_finalize(ppStmt);
			return;
		}

		if (option < OPT_QUERY_IGNNOROWERR)
			fprintf(stderr, "[err] sqlite3_step returned non-row data, rc: %d, query: %s", result.rc, query);
	}
	else
	{	
		if (result.rc != SQLITE_OK) {
			fprintf(stderr, "[err] sqlite3_prepare_v2 failed, rc: %d, query: %s\n", result.rc, query);
		}
		if (ppStmt == NULL) {
			fprintf(stderr, "[err] unable to prepare ppStmt on query: %s", query);
		}
	}

	if (option <= OPT_QUERY_ABORTONERR) {
		fprintf(stderr, "unable to execute query, fatal error, aborting");
		exit(-1);
	}
}


int cl_radarDB::getRadarId(const char *filename, int options)
{
	static st_clRadarDbRes result;
	static char *searchQuery = sqlite3_mprintf("SELECT id FROM radar WHERE fileName=%Q", filename);
/*	char *searchQuery = sqlite3_mprintf("SELECT id FROM radar WHERE station='%s'"
			" AND timestamp='%s' AND type='%s'", station, timestamp, type);*/

	GetQueryResultsMeta(searchQuery, result, options);
	sqlite3_free(searchQuery);

	if (result.rc == SQLITE_OK)
		return (int)result.data;
	else
		return -1;
}


// Returns NEXRAD radar signal strength of a pixel coordinate (indexed palette color)
inline int cl_nexradGif::GetFullPixelDBZIndex(unsigned int x, unsigned int y)
{
	register int red = radarGdImage->red[gdImageGetPixel(radarGdImage, x, y)];				// *SLOW*
	register int green = radarGdImage->green[gdImageGetPixel(radarGdImage, x, y)];			// *SLOW*
	register int blue = radarGdImage->blue[gdImageGetPixel(radarGdImage, x, y)];			// *SLOW*

	for (register int i = 1; i <= sizeof(dbZFullColorList) / (3*sizeof(int)); i++)		// 3 = R,G,B  **SLOW**
	{
        if (CheckListColor(i, red, green, blue, dbZFullColorList)) {			// **SLOW**
            return i;
        }
    }

    return 0;
}

// Overloaded function with combined xy pair argument
inline int cl_nexradGif::GetFullPixelDBZIndex(cartesianPair xy)
{
	register int red = radarGdImage->red[gdImageGetPixel(radarGdImage, xy.x, xy.y)];
	register int green = radarGdImage->green[gdImageGetPixel(radarGdImage, xy.x, xy.y)];
	register int blue = radarGdImage->blue[gdImageGetPixel(radarGdImage, xy.x, xy.y)];

	for (register int i = 1; i <= sizeof(dbZFullColorList) / (3*sizeof(int)); i++) {    // 3 = R,G,B
		if (CheckListColor(i, red, green, blue, dbZFullColorList))
			return i;
	}

	return 0;
}

/*unsigned int classifyImage(unsigned int id)
{
	unsigned int flags = 0;

	return flags;
}*/

/*if (stormCount < THRESH_RADAR_PX_STORM)
	flags |= FLAG_RADAR_NO_STORMS;

if (precipCount < THRESH_RADAR_PX_PRECIP_STORM)
	flags |= FLAG_RADAR_NO_PRECIP;*/

/*if (signalCount < THRESH_RADAR_
		if (stormPxCount >= THRESH_RADAR_PX_STORM)					// 25
		printf(CLR_R);
	else if (precipPxCount >= THRESH_RADAR_PX_PRECIP_STORM)		// 200
		printf(CLR_Y);
	else if (signalPxCount >= THRESH_RADAR_PX_PRECIP_RAW)		// 1000
		printf(CLR_BG);
	else if (signalPxCount >= THRESH_RADAR_PX_SIGNAL)			// 100
		printf(CLR_G);
	else
		printf(CLR_W);*/

// unsigned int paintCell(cartesianPair point, unsigned int id)
void cl_nexradGif::paintCell(stormCell &cell)
{
	unsigned int cellSize = 0, dbzIndex;
	bool localDebug = false;
	int dbzValue, dbzSum = 0, dbzMax = 0;
	stack<stormPixel> cellComponents;
	stormPixel cellPixel;
	stack<cartesianPair> pixels;
	cartesianPair point;
	stack<int> stdDevStack;

    // sin/cos first deravitive values circling a central pixel,
	// radius 1, starting at 12 o'clock (90 degrees)
    static int xShift[] = {  0,  1,  0,  0, -1, -1,  0,  0, };
    static int yShift[] = { -1,  0,  1,  1,  0,  0, -1, -1, };

    if (Index2DBZFull(GetFullPixelDBZIndex(cell.xy)) < THRESH_DBZ_PRE_TSTORM) return;

    if (localDebug) printf("painting cell %d ... ", cell.id);
    pixels.push(cell.xy);
	//stdDevStack.push(Index2DBZFull(GetFullPixelDBZIndex(cell.xy)));

    while (!pixels.empty())
    {
        point = pixels.top();
        pixels.pop();

		dbzIndex = GetFullPixelDBZIndex(point);
		dbzValue = Index2DBZFull(dbzIndex);

		if (localDebug) printf("(%d,%d,%d,%d)\n", point.x, point.y, dbzIndex, dbzValue);

		if (dbzValue >= THRESH_DBZ_PRE_TSTORM)
		{
			cellSize++;
			dbzSum += dbzValue;
			cellPixel.xy = point;
			cellPixel.dbz = dbzValue;
			cellComponents.push(cellPixel);
			stdDevStack.push(dbzValue);

			if (dbzValue > dbzMax)
				dbzMax = dbzValue;
			if (cellIDMatrix[point.y][point.x] == 0)
				cellIDMatrix[point.y][point.x] = cell.id;
		}

        // Take our xy point and check the surrounding 8 pixels
        for (int i = 0; i <= 7; i++)
        {
            point.x += xShift[i];
            point.y += yShift[i];

            if (point.x < 0 || point.y < 0 || point.x >= gdImageSX(radarGdImage) || point.y >= gdImageSY(radarGdImage))
                continue;

            if (dbzValue >= THRESH_DBZ_PRE_TSTORM && cellIDMatrix[point.y][point.x] == 0) {
                cellIDMatrix[point.y][point.x] = cell.id;
                pixels.push(point);
            }
			if (localDebug) printf(".");
        }
    }

	cell.size = cellSize;
	cell.dbzSigma = dbzSum;
	cell.dbzAvg = dbzSum / cellSize;
	cell.dbzMax = dbzMax;
	cell.dbzStdDev = 0.0;

	if (cellSize > 1)
	{
		float totalDev = 0;
		while (!stdDevStack.empty()) {
			//thisDev = stdDevStack.top() - cell.dbzAvg;
			totalDev += pow(stdDevStack.top() - cell.dbzAvg, 2);
			stdDevStack.pop();
		}
		//totalDev = sqrt(totalDev/(cellSize - 1));
		cell.dbzStdDev = sqrt(totalDev/(cellSize - 1));
	}

	float powXWeightSum = 0, powYWeightSum = 0, powerSum = 0;
	while (!cellComponents.empty()) {
		cellPixel =  cellComponents.top();
		cellComponents.pop();
		powXWeightSum += cellPixel.xy.x * dbz2Power(cellPixel.dbz);
		powYWeightSum += cellPixel.xy.y * dbz2Power(cellPixel.dbz);
		powerSum += dbz2Power(cellPixel.dbz);
	}

	cell.xCenter = powXWeightSum / powerSum;
	cell.yCenter = powYWeightSum / powerSum;

	if (localDebug) printf("\n");
}

//unsigned int cl_radarDB::addRadarToDB(unsigned int id, const char *radarFilePath, sqlite3 *db)
unsigned int cl_radarDB::addRadarToDB(unsigned int id, const char *radarFilePath)
{
	char *radarData;
	int n, rc, filesize;
	FILE *radarImage;
	sqlite3_stmt *ppStmt;

	// Make a pointer to the filename proper, sans any path
	char *radarFileName = (char *)strrchr(radarFilePath, '/') + 1;

	// TODO: Use standard helper functions for radarDB class
	char *query = sqlite3_mprintf("SELECT id FROM imagedata WHERE filename LIKE '%s'", radarFileName);
	rc = sqlite3_prepare_v2(imageDB, query, -1, &ppStmt, 0);
	if (rc == SQLITE_OK && ppStmt != NULL)
	{
		rc = sqlite3_step(ppStmt);
		if (rc == SQLITE_ROW)
		{
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

	// Read in image data
	fseek(radarImage, 0, SEEK_END);
	filesize = ftell(radarImage);
	fseek(radarImage, 0, SEEK_SET);
	radarData = (char *)malloc(sizeof(char) * filesize);
	n = fread(radarData, sizeof(char), filesize, radarImage);
	fclose(radarImage);

	// Insert data into the imgDB
	rc = sqlite3_prepare_v2(imageDB, "INSERT INTO imagedata (radarid, filename, radarimage) VALUES (?, ?, ?)", -1, &ppStmt, 0);
	if (rc == SQLITE_OK && ppStmt != NULL)
	{
		sqlite3_bind_int(ppStmt, 1, id);
		sqlite3_bind_text(ppStmt, 2, radarFileName, -1, SQLITE_STATIC);
		sqlite3_bind_blob(ppStmt, 3, radarData, filesize, free);

		rc = sqlite3_step(ppStmt);
		rc = sqlite3_finalize(ppStmt);
		printf("radar image %d (%s/%s) added to database\n", id, radarFilePath, radarFileName);
	}
	else {
		fprintf(stderr, "[err] SQL error rc: %d, skipping\n", rc);
	}

	return 1;
}



/*int analyzeGroups(bool sortOrder, unsigned int setFlag)
{
	// basic data types
 	int signalPxCount, precipPxCount, stormPxCount;
// 	unsigned int dbzData[DBZ_COLORLIST_LENGTH];
// 	unsigned int plotID, imageID, dataPoint, startRemovalID, endRemovalID;
	unsigned int keepCount, radarGroupID;
	bool localDebug = false;

	// sqlite database
	int rc, rc2, rc3, id, flags;
	char *errMsg = 0, *zErrMsg = 0;
	sqlite3_stmt *ppStmt;
// 	sqlite3_stmt *ppStmt2, *ppStmt3;
// 	unsigned int year, month, day, hour, minute;

	// radar data structs
	idFlagsTimeStruct radarEntry;
	fixed_queue<idFlagsTimeStruct> radarQueue(10);
	fixed_queue<idFlagsTimeStruct>::reverse_iterator rit;
// 	list<idTimeStruct> radarList;
// 	list<idTimeStruct>::iterator radarListIterator;

// 	// regex
// 	regex_t re;
// 	int status;
// 	size_t nmatch = 6;
// 	regmatch_t pmatch[6];
// 
// 	// date/time
// 	char buffer[5];
// 	tm timeStruct;
// 	time_t thisTime, lastTime;
// 	int timeDelta;

	// data files
// 	bool writeDataFiles = false;
// 	char dataFileFN[50], heatMapFN[50];
// 	FILE *dataFile = NULL;
// 	FILE *heatMapData = NULL;

	// Initialize values
// 	thisTime = lastTime = 0;
// 	startRemovalID = keepCount = 0;
// 	plotID = imageID = dataPoint = 0;
 	signalPxCount = precipPxCount = stormPxCount = 0;
	radarGroupID = 0;

	unsigned int stormSum, precipSum;
	float stormFloat, precipFloat;

// 	const unsigned char *ucTimestamp;
// 	char *cTimestamp
	char *updateQuery;


	if (sortOrder)
	{
		char selectQuery[] = "SELECT id, timestamp, signalPxCount, precipPxCount, stormPxCount, "
			" flags FROM radar ORDER BY timestamp";
		rc = sqlite3_prepare_v2(metaDB, selectQuery, -1, &ppStmt, 0);
	}
	else
	{
		char selectQuery[] = "SELECT id, timestamp, signalPxCount, precipPxCount, stormPxCount, "
			" flags FROM radar ORDER BY timestamp DESC";
		rc = sqlite3_prepare_v2(metaDB, selectQuery, -1, &ppStmt, 0);
	}

	// If unable to read data, abort and exit
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return -1;
	}

	while (rc = sqlite3_step(ppStmt) == SQLITE_ROW && !shutdownSignal)
	{
		id = sqlite3_column_int(ppStmt, 0);
// 		ucTimestamp = sqlite3_column_text(ppStmt, 1);
		signalPxCount = sqlite3_column_int(ppStmt, 2);
		precipPxCount = sqlite3_column_int(ppStmt, 3);
		stormPxCount = sqlite3_column_int(ppStmt, 4);
		flags = sqlite3_column_int(ppStmt, 5);

// 		cTimestamp = (char *)ucTimestamp;
// 		status = regexec(&re, cTimestamp, nmatch, pmatch, 0);
// 
// 		if (status == REG_NOMATCH) {
// 			printf("no match on regex\n");
// 			continue;
// 		}
// 		if (status != 0) {
// 			printf("error: %d\n", status);
// 			continue;
// 		}
// 
// 		year	= atoi(substr(buffer, cTimestamp, pmatch[1].rm_so, 4));
// 		month	= atoi(substr(buffer, cTimestamp, pmatch[2].rm_so, 2));
// 		day		= atoi(substr(buffer, cTimestamp, pmatch[3].rm_so, 2));
// 		hour	= atoi(substr(buffer, cTimestamp, pmatch[4].rm_so, 2));
// 		minute	= atoi(substr(buffer, cTimestamp, pmatch[5].rm_so, 2));
// 
// // 		timeStruct.tm_year	= year - 1900;
// // 		timeStruct.tm_mon	= month - 1;
// // 		timeStruct.tm_mday	= day;
// // 		timeStruct.tm_hour 	= hour;
// // 		timeStruct.tm_min	= minute;
// // 		timeStruct.tm_sec 	= 0;
// // 		timeStruct.tm_isdst	= 0;					// Explicitly required
// 
// // 		lastTime = thisTime;
// // 		thisTime = timegm(&timeStruct);
// // 		timeDelta = int(thisTime) - int(lastTime);

		radarEntry.id = id;
		radarEntry.flags = flags;
// 		radarEntry.time = timeStruct;
		radarEntry.stormPixels = stormPxCount;
		radarEntry.precipPixels = precipPxCount;
		radarQueue.push_fixed_back(radarEntry);

		if (flags & FLAG_RADAR_DELETE && radarQueue.size() > 2)
		{
			if (~radarQueue[radarQueue.size()-2].flags & FLAG_RADAR_DELETE)
			{
// 				printf("starting new removal: ");
				rit = radarQueue.rbegin() + 1;
				stormSum = precipSum = 1;
				for (int j=0; j<5; j++, rit++)
				{
					if (rit >= radarQueue.rend()) {
// 						printf("break!");
						break;
					}
// 					printf("%d ", rit->id);
					stormSum += rit->stormPixels;
					precipSum += rit->precipPixels;
// 					rit++;
				}

				stormFloat = log(stormSum) / log(2);
				precipFloat = log10(precipSum);
				keepCount = int(stormFloat + precipFloat/2);
// 				printf("   s:%d p:%d; %f + %f = %d\n", stormSum, precipSum, stormFloat, precipFloat, keepCount);
			}
		}
		else {}


//		updateQuery = sqlite3_mprintf("INSERT OR REPLACE INTO radarmeta (radarid, datatype, value) VALUES (%d, %d, %d)",
			id, DATA_STORMGROUP_ID, radarGroupID);
// 		updateQuery = sqlite3_mprintf("UPDATE radar SET radarGroup = %d WHERE id = %d", radarGroupID, id);
// 		rc = RunQuery(metaDB, updateQuery);
// 		sqlite3_free(updateQuery);

		if (flags & FLAG_RADAR_DELETE)
		{
// 			if (flags & FLAG_RADAR_KEEP_AFTER) {
// 				printf("> ");
// 			}
			if (keepCount > 0)
			{
// 				printf("o ");
				keepCount--;
				flags |= setFlag;
				updateQuery = sqlite3_mprintf("UPDATE radar SET flags = %d WHERE id = %d", flags, id);
				rc = RunQuery(metaDB, updateQuery);
			}
// 			else
// 				printf("X ");
		}
// 		else
// 			printf("+ ");

// 		printf("id=%05d %04d-%02d-%02d %02d:%02d d=%02dm [%s %4d %s/%s %5d %s/%s %6d %s/ 0x%05d ] [ group=%d ]\n",
// 			id, year, month, day, hour, minute, timeDelta/60, CLR_R, stormPxCount, CLR_W, CLR_Y, precipPxCount, CLR_W,
// 			CLR_BG, signalPxCount, CLR_W, flags, radarGroupID);
	}
}*/



/*int groupStorms(void)
{
	// basic data types
 	int signalPxCount, precipPxCount, stormPxCount;
// 	unsigned int dbzData[DBZ_COLORLIST_LENGTH];
// 	unsigned int plotID, imageID, dataPoint;
	unsigned int radarGroupID;
// 	unsigned int startRemovalID, endRemovalID
	unsigned int keepCount;
	bool localDebug = false;

	// sqlite database
	int rc, rc2, rc3, id, flags;
	char *errMsg = 0, *zErrMsg = 0;
	sqlite3_stmt *ppStmt;
// 	sqlite3_stmt *ppStmt2, *ppStmt3;
	unsigned int year, month, day, hour, minute;

	// radar data structs
	idFlagsTimeStruct radarEntry;
	fixed_queue<idFlagsTimeStruct> radarQueue(10);
	fixed_queue<idFlagsTimeStruct>::reverse_iterator rit;
// 	list<idTimeStruct> radarList;
// 	list<idTimeStruct>::iterator radarListIterator;

	// regex
	regex_t re;
	int status;
	size_t nmatch = 6;
	regmatch_t pmatch[6];

	// date/time
	char buffer[5];
	tm timeStruct;
	time_t thisTime, lastTime;
	int timeDelta;

	// data files
// 	bool writeDataFiles = false;
// 	char dataFileFN[50], heatMapFN[50];
// 	FILE *dataFile = NULL;
// 	FILE *heatMapData = NULL;

	// Initialize values
// 	thisTime = lastTime = 0;
// 	startRemovalID = keepCount = 0;
// 	plotID = imageID = dataPoint = 0;
 	signalPxCount = precipPxCount = stormPxCount = 0;
	radarGroupID = 0;

// 	int maxSize, maxDbzSigma, maxDbzValue;
// 	int status, dbzValue;
// 	unsigned int pixelCount;
// 	int datatype;
// 	float value;
	unsigned int stormSum, precipSum;
	float stormFloat, precipFloat;

	const unsigned char *ucTimestamp;
	char *cTimestamp, *updateQuery;
// 	char *selectFlagsQuery, *maxCellQuery, *dbzDataQuery;


	// More of an time-domain analysis then compaction at this point, but hey... who's keeping track?
	printf("grouping images...");
	fflush(stdout);

	char timestampRegx[] = "([0-9]{4})([0-9]{2})([0-9]{2})_([0-9]{2})([0-9]{2})";
	if (regcomp(&re, timestampRegx, REG_EXTENDED) != 0) {
		printf("Unable to compile regex, aborting...\n");
// 		shutdownApplication(true, 1);
		return -1;
	}

	//RunQuery(metaDB, "BEGIN;");
	char selectQuery[] = "SELECT id, timestamp FROM radar ORDER BY timestamp"; // LIMIT 1000";
	rc = sqlite3_prepare_v2(metaDB, selectQuery, -1, &ppStmt, 0);

	// If unable to read data, abort and exit
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		regfree(&re);
// 		shutdownApplication(true, 1);
		return -1;
	}

	// Iterate through the radar images
	while (rc = sqlite3_step(ppStmt) == SQLITE_ROW && !shutdownSignal)
	{
		id = sqlite3_column_int(ppStmt, 0);
		ucTimestamp = sqlite3_column_text(ppStmt, 1);
// 		signalPxCount = sqlite3_column_int(ppStmt, 2);
// 		precipPxCount = sqlite3_column_int(ppStmt, 3);
// 		stormPxCount = sqlite3_column_int(ppStmt, 4);
// 		flags = sqlite3_column_int(ppStmt, 5);

		cTimestamp = (char *)ucTimestamp;
		status = regexec(&re, cTimestamp, nmatch, pmatch, 0);

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
		timeStruct.tm_hour 	= hour;
		timeStruct.tm_min	= minute;
		timeStruct.tm_sec 	= 0;
		timeStruct.tm_isdst	= 0;					// Explicitly required

		lastTime = thisTime;
		thisTime = timegm(&timeStruct);
		timeDelta = int(thisTime) - int(lastTime);

		// Search for gaps in time domain
		if (abs(timeDelta) > 4000 && int(lastTime) > 0)
		{
			radarGroupID++;
			printf("%d ", radarGroupID);
		}

		updateQuery = sqlite3_mprintf("UPDATE radar SET radarGroup = %d WHERE id = %d", radarGroupID, id);
		rc = RunQuery(metaDB, updateQuery);
		sqlite3_free(updateQuery);
	}

	printf("\n\nanalyzing groups...");
	fflush(stdout);

	char selectQuery2[] = "SELECT id, timestamp, signalPxCount, precipPxCount, stormPxCount, "
		" flags, radarGroup FROM radar ORDER BY timestamp"; // LIMIT 1000";
	rc = sqlite3_prepare_v2(metaDB, selectQuery2, -1, &ppStmt, 0);

	// If unable to read data, abort and exit
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		regfree(&re);
		return -1;
	}

	printf("asc...");
	analyzeGroups(true, FLAG_RADAR_KEEP_AFTER);
	printf("desc...");
	analyzeGroups(false, FLAG_RADAR_KEEP_BEFORE);
	printf("done\n\n");

	while (rc = sqlite3_step(ppStmt) == SQLITE_ROW && !shutdownSignal)
	{
		id = sqlite3_column_int(ppStmt, 0);
		ucTimestamp = sqlite3_column_text(ppStmt, 1);
		signalPxCount = sqlite3_column_int(ppStmt, 2);
		precipPxCount = sqlite3_column_int(ppStmt, 3);
		stormPxCount = sqlite3_column_int(ppStmt, 4);
		flags = sqlite3_column_int(ppStmt, 5);
		radarGroupID = sqlite3_column_int(ppStmt, 6);

		cTimestamp = (char *)ucTimestamp;
		status = regexec(&re, cTimestamp, nmatch, pmatch, 0);

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

// 		timeStruct.tm_year	= year - 1900;
// 		timeStruct.tm_mon	= month - 1;
// 		timeStruct.tm_mday	= day;
// 		timeStruct.tm_hour 	= hour;
// 		timeStruct.tm_min	= minute;
// 		timeStruct.tm_sec 	= 0;
// 		timeStruct.tm_isdst	= 0;					// Explicitly required

// 		lastTime = thisTime;
// 		thisTime = timegm(&timeStruct);
// 		timeDelta = int(thisTime) - int(lastTime);


// 		radarEntry.id = id;
// 		radarEntry.flags = flags;
// // 		radarEntry.time = timeStruct;
// 		radarEntry.stormPixels = stormPxCount;
// 		radarEntry.precipPixels = precipPxCount;
// 		radarQueue.push_fixed_back(radarEntry);
// 
// 		if (flags & FLAG_RADAR_DELETE)
// 		{
// 			if (~radarQueue[radarQueue.size()-2].flags & FLAG_RADAR_DELETE)
// 			{
// // 				printf("starting new removal: ");
// 				rit = radarQueue.rbegin() + 1;
// 				stormSum = precipSum = 1;
// 				for (int j=0; j<5; j++, rit++)
// 				{
// 					if (rit >= radarQueue.rend()) {
// // 						printf("break!");
// 						break;
// 					}
// // 					printf("%d ", rit->id);
// 					stormSum += rit->stormPixels;
// 					precipSum += rit->precipPixels;
// // 					rit++;
// 				}
// 
// 				stormFloat = log(stormSum) / log(2);
// 				precipFloat = log10(precipSum);
// 				keepCount = int(stormFloat + precipFloat/2);
// // 				printf("   s:%d p:%d; %f + %f = %d\n", stormSum, precipSum, stormFloat, precipFloat, keepCount);
// 			}
// 		}
// 		else 
// 		{
// 		}

//		updateQuery = sqlite3_mprintf("INSERT OR REPLACE INTO radarmeta (radarid, datatype, value) VALUES (%d, %d, %d)",
			id, DATA_STORMGROUP_ID, radarGroupID);
// 		updateQuery = sqlite3_mprintf("UPDATE radar SET radarGroup = %d WHERE id = %d", radarGroupID, id);
// 		rc = RunQuery(metaDB, updateQuery);
// 		sqlite3_free(updateQuery);

		if (flags & FLAG_RADAR_DELETE)
		{
			if (flags & FLAG_RADAR_KEEP_AFTER) {
				printf("> ");
			}
			else if (flags & FLAG_RADAR_KEEP_BEFORE) {
				printf("< ");
			}
// 			if (keepCount > 0)
// 			{
// 				printf("o ");
// 				keepCount--;
// 				flags |= FLAG_RADAR_KEEP_AFTER;
// 				updateQuery = sqlite3_mprintf("UPDATE radar SET flags = %d WHERE id = %d", flags, id);
// 				rc = RunQuery(metaDB, updateQuery);
// 			}
			else
				printf("%sX%s ", CLR_R, CLR_DEF);
		}
		else
			printf("+ ");

		printf("id=%05d %04d-%02d-%02d %02d:%02d d=%02dm [%s %4d %s/%s %5d %s/%s %6d %s/ 0x%05d ] [ group=%d ]\n",
			id, year, month, day, hour, minute, timeDelta/60, CLR_R, stormPxCount, CLR_W, CLR_Y, precipPxCount, CLR_W,
			CLR_BG, signalPxCount, CLR_W, flags, radarGroupID);
	}
}*/



/*void analyzeDB(bool flagCompactDB, bool writeDataFiles)
{
	// basic data types
	int signalPxCount, precipPxCount, stormPxCount;
// 	int oldSignal, oldPrecip, oldStorm, oldFlags;
	unsigned int dbzData[DBZ_COLORLIST_LENGTH];
	unsigned int plotID, imageID, dataPoint, radarGroupID;
	unsigned int startRemovalID, endRemovalID, keepCount;
	bool localDebug = false;

	// sqlite database
	int rc, rc2, rc3, id, flags;
	char *errMsg = 0, *zErrMsg = 0;
	sqlite3_stmt *ppStmt, *ppStmt2, *ppStmt3;
	unsigned int year, month, day, hour, minute;

	// stl
// 	list<idTimeStruct> radarList;
//     list<idTimeStruct>::iterator radarListIterator;
	idFlagsTimeStruct radarEntry;
	fixed_queue<idFlagsTimeStruct> radarQueue(10);
	fixed_queue<idFlagsTimeStruct>::reverse_iterator rit;

	// regex
	regex_t re;
	size_t nmatch = 6;
	regmatch_t pmatch[6];

	// /DATA_STORMGROUP_ID/ date/time
	char buffer[5];
	tm timeStruct;
	time_t thisTime, lastTime;

	// data files
// 	bool writeDataFiles = false;
	char dataFileFN[50], heatMapFN[50];
	FILE *dataFile = NULL;
	FILE *heatMapData = NULL;

	// Initialize values
	thisTime = lastTime = 0;
	startRemovalID = keepCount = 0;
	plotID = imageID = dataPoint = radarGroupID = 0;
// 	oldSignal = oldPrecip = oldStorm = oldFlags = 0;
	signalPxCount = precipPxCount = stormPxCount = 0;

	int maxSize, maxDbzSigma, maxDbzValue;
	int status, dbzValue;
	unsigned int pixelCount;

	int datatype;
	float value;
	unsigned int stormSum, precipSum;
	float stormFloat, precipFloat;

	const unsigned char *ucTimestamp;
	char *cTimestamp, *updateQuery, *selectFlagsQuery, *maxCellQuery, *dbzDataQuery;


	// More of an time-domain analysis then compaction at this point, but hey... who's keeping track?
	printf("analyzing DB...\n\n");

// 	printf("grouping storms...");
	groupStorms();
// 	printf("done\n");
	return;

	char timestampRegx[] = "([0-9]{4})([0-9]{2})([0-9]{2})_([0-9]{2})([0-9]{2})";
	if (regcomp(&re, timestampRegx, REG_EXTENDED) != 0) {
		printf("Unable to compile regex, aborting...\n");
		shutdownApplication(true, 1);
	}
// 	printf("looking for %s\n\n", timestampRegx);

	/**
	BASIC ALGORITHM

	1. Iterate through images
	2. Find all storm groups (consecutive collections of radar images with storm activity)
	3. Extend collection X images in each direction (before/after) (dynamic based on strength/trend)
		If small/weak collection extend short amount and vice vercia
	4. If storm collection is of minimum size (image count) or strength (cell size/strength) then keep
		otherwise mark for removal

	--== [ OR ] ==--

	1. Iterage through images
	2. Find large blocks of non-storm groups
	3. Shrink group by a certain amount in each direction (based upon activity?)
		Amount calculated by weighted amount of storm + precip values
		Logarithmic? log2(sigma(last 5 stormPixelCounts)) + log10(sigma(last 5 precipPixelCounts))
	4. Mark group for removal

	Perhaps both methods should be investigated and sample run performed to compare results?
	**/

	/**RunQuery(metaDB, "BEGIN;");
	char selectQuery[] = "SELECT id, timestamp FROM radar ORDER BY timestamp"; // LIMIT 1000";
	rc = sqlite3_prepare_v2(metaDB, selectQuery, -1, &ppStmt, 0);

	// If unable to read data, abort and exit
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);

		regfree(&re);
		shutdownApplication(true, 1);
	}

	// Iterate through the radar images
	while (rc = sqlite3_step(ppStmt) == SQLITE_ROW && !shutdownSignal)
	{
		id = sqlite3_column_int(ppStmt, 0);
		ucTimestamp = sqlite3_column_text(ppStmt, 1);
		cTimestamp = (char *)ucTimestamp;

		status = regexec(&re, cTimestamp, nmatch, pmatch, 0);

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
		timeStruct.tm_hour 	= hour;
		timeStruct.tm_min	= minute;
		timeStruct.tm_sec 	= 0;
		timeStruct.tm_isdst	= 0;					// Explicitly required

		lastTime = thisTime;
		thisTime = timegm(&timeStruct);

		// Grab metadata from radar table about this image
		selectFlagsQuery = sqlite3_mprintf("SELECT signalPxCount, precipPxCount, "
						"stormPxCount, flags FROM radar WHERE id=%d", id);
		rc2 = sqlite3_prepare_v2(metaDB, selectFlagsQuery, -1, &ppStmt2, 0);

		flags = -1;
		if (rc2 == SQLITE_OK && ppStmt2 != NULL)
		{
			rc2 = sqlite3_step(ppStmt2);
			if (rc2 == SQLITE_ROW)
			{
// 				oldSignal = signalPxCount;
// 				oldPrecip = precipPxCount;
// 				oldStorm = stormPxCount;
// 				oldFlags = flags;
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


		radarEntry.id = id;
		radarEntry.flags = flags;
		radarEntry.time = timeStruct;
		radarEntry.stormPixels = stormPxCount;
		radarEntry.precipPixels = precipPxCount;


		// Find our maximum size/dbzvalues for storm cells in this image
		// We originally did a SELECT MAX() on the celldata table, however this
		// was horifically slow.  Now we have implemented a radarmeta cache which
		// is populated/updated when processImage() is run.
		// We can re-use rc2/ppStmt2 from previous query here
		//maxCellQuery = sqlite3_mprintf("SELECT datatype,value FROM radarmeta WHERE radarid=%d AND datatype "
			"IN (%d, %d, %d)", id, DATA_MAX_CELL_SIZE, DATA_MAX_CELL_SIGMA, DATA_MAX_DBZ_VALUE);
		rc2 = sqlite3_prepare_v2(metaDB, maxCellQuery, -1, &ppStmt2, 0);

		maxSize = maxDbzSigma = maxDbzValue = 0;
		if (rc2 == SQLITE_OK && ppStmt2 != NULL)
		{
			while (rc2 = sqlite3_step(ppStmt2) == SQLITE_ROW)
			{
				datatype = sqlite3_column_int(ppStmt2, 0);
				value = sqlite3_column_double(ppStmt2, 1);

				if (datatype == DATA_MAX_CELL_SIZE) maxSize = value;
				if (datatype == DATA_MAX_CELL_SIGMA) maxDbzSigma = value;
				if (datatype == DATA_MAX_DBZ_VALUE) maxDbzValue = value;
			}
			rc2 = sqlite3_finalize(ppStmt2);
		}
		else {
			printf("[err] sqlite3_prepare_v2 did not return valid data (%d) on query '%s',"
				"skipping id %d\n", rc2, maxCellQuery, id);
			continue;
		}*/

		// data update!
		// Search for gaps in time domain
		/*if (abs(int(thisTime) - int(lastTime)) > 4000 && int(lastTime) > 0)
		{
			if (startRemovalID != 0)
			{
				// Remove between startRemovalID and current ID
				// Reset startRemovalID to zero (null/false)
				printf("]>-: EORM\n\n");
				startRemovalID = keepCount = 0;
			}

			// output only / final pass
			if (writeDataFiles)
			{
				if (dataFile != NULL) fclose(dataFile);
				if (heatMapData != NULL) fclose(heatMapData);

				sprintf(dataFileFN, "%s/gnuplot-%d.dat", DATAFILE_PATH, plotID);
				sprintf(heatMapFN, "%s/heatmap-%d.dat", DATAFILE_PATH, plotID);
				printf("using datafiles %s/%s\n", dataFileFN, heatMapFN);
				dataFile = fopen(dataFileFN, "w");
				heatMapData = fopen(heatMapFN, "w");
			}

			plotID++;
			radarGroupID++;

			// Reset everything
			radarQueue.clear();
			printf("\n\n\n\nstorm group ID: %d\n", radarGroupID);
			imageID = 0;
// 			oldSignal = oldPrecip = oldStorm = oldFlags = 0;
		}

		// data update!
		radarQueue.push_fixed_back(radarEntry);

		// This image marked for deletion?
		if (flags & FLAG_RADAR_DELETE)
		{
			// Is this the first image in the chronological set to be removed?
			// eg: was the previous image NOT marked for removal?
			if (startRemovalID == 0)
			{
				// Start a new 'removal' set
				startRemovalID = id;
				printf(CLR_DEF);

				stormSum = precipSum = 1;
				rit = radarQueue.rbegin() + 1;
				printf("\n");
				for (int j=0; j<5; j++)
				{
					if (rit >= radarQueue.rend()) { printf("break!"); break; }
					printf("%d ", rit->id);
					stormSum += rit->stormPixels;
					precipSum += rit->precipPixels;
					rit++;
				}

				stormFloat = log(stormSum) / log(2);
				precipFloat = log10(precipSum);
				keepCount = int(stormFloat + precipFloat/2);
				printf("\nRM: -<[ ");
				printf("s: %d, p: %d;\t%f + %f = %d\n", stormSum, precipSum, stormFloat, precipFloat, keepCount);
			}
			// This image is marked for deletion but so was the previous one
			// so we are in a set of images marked for removal
			else if (startRemovalID != 0) {}
		}
		// Image not marked for removal
		else if (!(flags & FLAG_RADAR_DELETE))
		{
			// However, previous image was marked
			if (startRemovalID != 0)
			{
				// Remove between startRemovalID and current ID
				// Reset startRemovalID to zero (null/false)
				printf("]>-: EORM\n\n");
				startRemovalID = 0;
			}
		}

		// output only / final pass
		if (stormPxCount >= THRESH_RADAR_PX_STORM)					// 25
			printf(CLR_R);
		else if (precipPxCount >= THRESH_RADAR_PX_PRECIP_STORM)		// 200
			printf(CLR_Y);
		else if (signalPxCount >= THRESH_RADAR_PX_PRECIP_RAW)		// 1000
			printf(CLR_BG);
		else if (signalPxCount >= THRESH_RADAR_PX_SIGNAL)			// 100
			printf(CLR_G);
		else
			printf(CLR_W);

		// output only / final pass
		if (writeDataFiles)
		{
			dbzDataQuery = sqlite3_mprintf("SELECT dbzValue, pixelCount FROM dbzData WHERE radarid=%d", id);
			rc3 = sqlite3_prepare_v2(metaDB, dbzDataQuery, -1, &ppStmt3, 0);

			if (rc3 == SQLITE_OK)
			{
				while (rc3 = sqlite3_step(ppStmt3) == SQLITE_ROW)
				{
					dbzValue = sqlite3_column_int(ppStmt3, 0);
					pixelCount = sqlite3_column_int(ppStmt3, 1);
					if (DBZ_COLORLIST_MIN < dbzValue && dbzValue < DBZ_COLORLIST_MAX) {
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

			if (writeDataFiles && dataFile != NULL)
				fprintf(dataFile, "%d %d %d %d\n", (int)thisTime, stormPxCount, precipPxCount, signalPxCount);

			for (int dbz=DBZ_COLORLIST_MIN; dbz<=DBZ_COLORLIST_MAX; dbz+=DBZ_COLORLIST_STEP)
			{
				if (writeDataFiles && heatMapData != NULL)
					fprintf(heatMapData, "%d %d %d\n", imageID, dbz, dbzData[DBZFull2Index(dbz)]);
// 				printf("%04d ", dbzData[DBZFull2Index(dbz)]);
			}

// 			printf("\n");
			if (writeDataFiles && heatMapData != NULL) fprintf(heatMapData, "\n");
			imageID++;
		}

		// data update!
		if (startRemovalID == 0)
			printf(". ");
		else
		{
			if (keepCount > 0)
			{
				printf("o ");
				keepCount--;
				flags |= FLAG_RADAR_KEEP_BEFORE;
				updateQuery = sqlite3_mprintf("UPDATE radar SET flags=%d WHERE id = %d", flags, id);
				rc = RunQuery(metaDB, updateQuery);
				sqlite3_free(updateQuery);
			}
			else {
				printf("X ");
				keepCount = 0;
			}
		}

// 		updateQuery = sqlite3_mprintf("UPDATE radar SET ");

		// output only / final pass
		printf("%05d %04d-%02d-%02d %02d:%02d d=%02dm %s[%s %4d %s/%s %5d %s/%s %6d %s/ %5d ] ",
			id, year, month, day, hour, minute, (int(thisTime)-int(lastTime))/60,
			CLR_W, CLR_R, stormPxCount, CLR_W, CLR_Y, precipPxCount, CLR_W, CLR_BG, signalPxCount, CLR_W, flags);
		printf(" %d", stormPxCount*stormPxCount*60 + precipPxCount*5 + signalPxCount);
// 		printf(" max_size/sigma/avg/maxSig: %d %d ", maxSize, maxDbzSigma);
// 		if (maxSize > 0) printf("%d", maxDbzSigma/maxSize);
// 		printf(" %d", maxDbzValue);
		printf("\n");

	}

	//RunQuery(metaDB, "END;");
	rc = sqlite3_finalize(ppStmt);		// TODO: finalize other ppStmt's??


	if (writeDataFiles) {
		fclose(dataFile);
		fclose(heatMapData);
	}

	// tidy up
	regfree(&re);
}*/



//int processImage(unsigned int &newId, const char *radarfile, sqlite3 *metaDB, bool skipExisting)
//int cl_nexradGif::processImage(unsigned int &newId, const char *radarfile, bool skipExisting)
int cl_nexradGif::processImage(unsigned int &newId, bool skipExisting)
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
	stormCell cell, avgCell, maxCell;

	// storage: sqlite/files
	FILE *radarFile;
//	sqlite3_stmt *ppStmt, *ppStmt2
	char station[4], timestamp[14], type[4];
	char *errMsg = 0;
	int rc, radarId = 0;
	st_clRadarDbRes result;



	// /path/to/radar/DIX_20090612_0208_NCR.gif
	// Make a pointer to the filename proper, sans any path
	char *radarFileName = (char *)strrchr(radarFilename, '/') + 1;
	string sRadarWildcard = radarFileName;
	//string sRadarWildcard = (char *)strrchr(radarFilename, '/') + 1;
	sRadarWildcard.replace(sRadarWildcard.size()-7, 3, "???");

	// Extract station and timestamp from filename
	strncpy(station, radarFileName, 3);
	strncpy(timestamp, radarFileName+4, 13);
	strncpy(type, radarFileName+18, 3);
	timestamp[13] = '\0';
	station[3] = '\0';
	type[3] = '\0';


	float thisAvgDbz, avgSize = 0, avgSigma = 0;
	int dbzMaxSignal = 0;

	// Search our radar table for an existing match
	// TODO: move to Python code (make this C code dumb and migrate error-checking elsewhere?)
/*	char *searchQuery = sqlite3_mprintf("SELECT id FROM radar WHERE station='%s'"
			" AND timestamp='%s' AND type='%s'", station, timestamp, type);*/
/*	char *searchQuery = sqlite3_mprintf("SELECT id FROM radar WHERE fileName=%Q", radarFileName);

	radarDB->GetQueryResultsMeta(searchQuery, result, OPT_QUERY_IGNNOROWERR);
	if (result.rc == SQLITE_OK) radarId = (int)result.data;
	sqlite3_free(searchQuery);*/
	radarId = radarDB->getRadarId(radarFileName, OPT_QUERY_IGNNOROWERR); 

/*	rc = sqlite3_prepare_v2(metaDB, searchQuery, -1, &ppStmt, 0);

	if (rc == SQLITE_OK && ppStmt != NULL)
	{
		rc = sqlite3_step(ppStmt);
		if (rc == SQLITE_ROW)
		{
			radarId = sqlite3_column_int(ppStmt, 0);
			rc = sqlite3_finalize(ppStmt);*/

			if (radarId > 0 && skipExisting)
			{
				printf("Radar image %s exists in meta db (id: %d) and flag set, skipping\n", radarFileName, radarId);
				return ERROR_SKIP_IMAGE;
			}
/*		}
	}*/

	radarFile = fopen(radarFilename, "rb");
	if (!radarFile) {
		fprintf(stderr, "Can't open radar source image '%s', aborting...\n", radarFilename);
        return ERROR_CANT_OPEN;
    }

    // Load our image and prepare to analyze
    radarGdImage = gdImageCreateFromGif(radarFile);
	if (!radarGdImage) {
		fprintf(stderr, "Radar source image not GIF compliant, aborting...\n");
		fclose(radarFile);
		return ERROR_NON_GIF;
	}

	stormCellCount = stormPixelCount = precipPixelCount = signalPixelCount = 0;
	memset(dbzPixelCount, 0, sizeof(unsigned int) * DBZ_COLORLIST_LENGTH);
	memset(&avgCell, 0, sizeof(stormCell));
	memset(&maxCell, 0, sizeof(stormCell));
	xMax = gdImageSX(radarGdImage);
    yMax = gdImageSY(radarGdImage);

    // Allocate memory for our ID array
	// This is a static variable, heap buffer will be reused instead of reallocation
	// TODO: moved to thread-level data structure
	// ie: each thread implements data structure and single nexradGif instance
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

	// Get transparent color.  If not found, default to last color in indexed image
	// NEXRAD radar images typically uses this for transparency
	int transparent = gdImageGetTransparent(radarGdImage);
	if (!transparent) transparent = gdImageColorsTotal(radarGdImage);

	start = clock();

    /** Enumerate storm pixels **/
	// TODO: Can we do some cache optimization here by breaking into subblocks??
    for (y = 0;	y < yMax; y++)
    {
        for (x = 0; x < xMax; x++)
        {
			// Optimize here and grab our alpha first and skip if blank
            pixelGDIndex = gdImageGetPixel(radarGdImage, x, y);
            pixelA = gdImageAlpha(radarGdImage, pixelGDIndex);

			// Skip empty data (transparent pixels)
			if (pixelA > 0)
                continue;

			// If regular pixel, get RGB
			pixelR = radarGdImage->red[pixelGDIndex];
            pixelG = radarGdImage->green[pixelGDIndex];
            pixelB = radarGdImage->blue[pixelGDIndex];

			pixelFullIndex = GetFullPixelDBZIndex(x, y);
			pixelFullDBZ = Index2DBZFull(pixelFullIndex);

			if (debug)
                printf("%d/%d (%d,%d,%d,%d)\n", pixelFullIndex, pixelFullDBZ, pixelR, pixelG, pixelB, pixelA);


			// Remove weak signals / noise from radar image
			// This is kind of useless as we don't write the image back out
			// It would be nice to write this updated image to db directly bypassing writing to disk
			/*if (pixelFullDBZ < THRESH_DBZ_LIGHT_PRECIP) {
				gdImageSetPixel(radarGdImage, x, y, transparent);
			}*/

			dbzPixelCount[pixelFullIndex]++;

			if (pixelFullDBZ >= THRESH_DBZ_LIGHT_PRECIP)
				signalPixelCount++;
			if (pixelFullDBZ >= THRESH_DBZ_MODERATE_PRECIP)
				precipPixelCount++;
            if (pixelFullDBZ >= THRESH_DBZ_TSTORM)
			{
                stormPixelCount++;
                xy.x = x;			xy.y = y;
                xySum.x += x;		xySum.y += y;
                pixelList.push_back(xy);
            }
		}
	}

	end = clock();
	enumPixelsClock += (end - start);


	/** Perform some very basic classification **/
	if (stormPixelCount == 0)
		flags |= FLAG_RADAR_NO_STORMS;
	if (signalPixelCount < THRESH_RADAR_PX_PRECIP_RAW)
		flags |= FLAG_RADAR_NO_PRECIP;
	if (signalPixelCount < THRESH_RADAR_PX_SIGNAL && stormPixelCount == 0)
		flags |= FLAG_RADAR_NO_DATA;
	if (stormPixelCount == 0 && signalPixelCount == 0 && precipPixelCount == 0)
		flags |= FLAG_RADAR_BLANK;


	// TODO: Replace this with a general heuristic rather then strict thresholds
	// Create a probability function, maybe fuzzy logic??
	/*if ((( stormPixelCount == 0 && precipPixelCount < THRESH_RADAR_PX_PRECIP_RAW ) ||
		( stormPixelCount < THRESH_RADAR_PX_STORM && precipPixelCount < THRESH_RADAR_PX_PRECIP_STORM ))
		&& FLAG_REMOVE_EMPTY_IMAGES)*/
	if ( flags & FLAG_RADAR_NO_STORMS && FLAG_REMOVE_EMPTY_IMAGES )
	{
		printf("\nradar analysis revealed no storms, marking image for removal");

		flags |= FLAG_RADAR_DELETE;
		markedDeletionCount++;
// 		removedImagesCount++;

// 		char *cpRadarWildcard = (char *)sRadarWildcard.c_str();
// 		unlink(radarWildcard);
// 		unlink(radarfile);

		// Remove any cached radar image data
 		/*char *deleteRadarQuery = sqlite3_mprintf("DELETE FROM imagedata WHERE filename='%s'", radarFileName);
 		rc = RunQuery(imageDB, deleteRadarQuery);
 		fclose(radarFile);
 		gdImageDestroy(radarGdImage);
 		return 0;*/
	}

	printf("\nradar information: (%d signal, %d precip, %d storm, %d flags)\n",
		   signalPixelCount, precipPixelCount, stormPixelCount, flags);


	/** Enumerate our storm cells **/
	start = clock();

    for (pixelIterator = pixelList.begin(); stormPixelCount > 0, pixelIterator != pixelList.end(); pixelIterator++)
    {
        xy = *pixelIterator;
        x = xy.x; 		  y = xy.y;
		/*printf("P %X...", *pixelIterator);
		printf("C (%d,%d)...", x, y);*/

        if (cellIDMatrix[y][x] == 0)
        {
            cell.id = ++stormCellCount;
			cell.xy = xy;
            paintCell(cell);
            cellList.push_back(cell);
        }
    }

	end = clock();
	enumCellsClock += (end - start);


	/** Search our radar table for an existing match **/
	// Do not perform another query, just check to see if ID is nonzero (we check this already above)
	start = clock();
// 	char *selectQuery = sqlite3_mprintf("SELECT id FROM radar WHERE station='%s'"
// 			" AND timestamp='%s' AND type='%s'", station, timestamp, type);
// 	rc = sqlite3_prepare_v2(metaDB, selectQuery, -1, &ppStmt, 0);
//
// 	if (rc == SQLITE_OK && ppStmt != NULL)
// 	{
// 		rc = sqlite3_step(ppStmt);
// 		if (rc == SQLITE_ROW)

	if (radarId != 0)
	{
		printf("updating radar id %d entry with latest\n", radarId);

		// We found existing entry, update it
	// 			radarId = sqlite3_column_int(ppStmt, 0);
	// 			rc = sqlite3_finalize(ppStmt);

		char *updateQuery = sqlite3_mprintf("UPDATE radar SET station=%Q, timestamp=%Q, type=%Q, "
			"fileName=%Q, flags=%d, signalPxCount=%d, precipPxCount=%d, stormPxCount=%d, cellCount=%d "
			"WHERE id = %d", station, timestamp, type, radarFileName, flags, signalPixelCount, precipPixelCount,
			stormPixelCount, stormCellCount, radarId);
		rc = radarDB->RunQueryMeta(updateQuery);
	//			radarDB->GetMetaQueryResults(updateQuery, &result);
		sqlite3_free(updateQuery);

	// 			char *dbzClearQuery = sqlite3_mprintf("DELETE FROM dbzData WHERE radarid=%d", radarId);
	// 			rc = radarDB->RunQueryMeta(dbzClearQuery);
	// 			sqlite3_free(dbzClearQuery);
	}
	else
	{
		printf("inserting new radar entry\n");

	// 			rc = sqlite3_finalize(ppStmt);

		// No entry found, insert new
		char *insertQuery = sqlite3_mprintf("INSERT INTO radar (station, timestamp, type, fileName,"
			" flags, signalPxCount, precipPxCount, stormPxCount, cellCount) VALUES (%Q, %Q, %Q, %Q, %d, %d, %d, %d, %d)",
			station, timestamp, type, radarFileName, flags, signalPixelCount, precipPixelCount, stormPixelCount, stormCellCount);
		rc = radarDB->RunQueryMeta(insertQuery);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "SQL error: on INSERT query %s, returned %d\n", insertQuery, rc);
		}
		sqlite3_free(insertQuery);

		// Get the id of our newly inserted entry
		// There probably is a better way to do it but this works
		char *selectNewQuery = sqlite3_mprintf("SELECT id FROM radar WHERE station='%s'"
					" AND timestamp='%s' AND type='%s'", station, timestamp, type);
		radarDB->GetQueryResultsMeta(selectNewQuery, result);
		if (result.rc != SQLITE_OK) {
				radarId = 0;
				fprintf(stderr, "SQL error: on SELECT (%s) new inserted data sqlite3_step returned %d\n", selectNewQuery, rc);
		}
		else
			radarId = (int)result.data;

		sqlite3_free(selectNewQuery);

	/*			rc = sqlite3_prepare_v2(metaDB, selectNewQuery, -1, &ppStmt, 0);

		if (rc == SQLITE_OK && ppStmt != NULL)
		{
			rc = sqlite3_step(ppStmt);
			if (rc == SQLITE_ROW)
			{
				radarId = sqlite3_column_int(ppStmt, 0);
				rc = sqlite3_finalize(ppStmt);
			}
			else {
				// abort here?
				radarId = 0;
				fprintf(stderr, "SQL error: on SELECT (%s) new inserted data sqlite3_step returned %d\n", selectNewQuery, rc);
			}
		}
		else {
			// abort here?
			radarId = 0;
			fprintf(stderr, "SQL error: on SELECT (%s) new inserted data sqlite3_prepare returned %d\n", selectNewQuery, rc);
		}*/

	}
// 	}
// 	else {
// 		fprintf(stderr, "SQL error, on SELECT (%s) search sqlite_prepare returned %d. ppStmt=0x%p\n", selectQuery, rc, ppStmt);
// 		sqlite3_finalize(ppStmt);
// 	}

//	sqlite3_free(selectQuery);
	newId = radarId;
	end = clock();
	radarClock += (end - start);

	// If no storms, close file and abort
	// We used to check if marked for deletion but now only if zero storms
	if (flags & FLAG_RADAR_NO_STORMS) {
		fclose(radarFile);
		gdImageDestroy(radarGdImage);
		return 0;
	}


	/** Update/insert DBZ data for radar image **/
	printf("inserting DBZ data: ");

	start = clock();
	radarDB->RunQueryMeta("BEGIN");
	// 	sqlite3_prepare_v2(metaDB, "INSERT INTO dbzData (radarid, dbzValue, pixelCount) VALUES (?, ?, ?)", -1, &ppStmt2, 0);

	// Loop through DBZ data and add to db
	for (int i=1; i<DBZ_COLORLIST_LENGTH, Index2DBZFull(i)<=THRESH_DBZ_MAX_SIGNAL; i++)
	{
		printf("%d=%d ", Index2DBZFull(i), dbzPixelCount[i]);

		// Use prepared statement instead
		/*char *dbzQuery = sqlite3_mprintf("INSERT INTO dbzData (radarid, dbzValue, pixelCount) VALUES (%d, %d, %d)",
			radarId, Index2DBZFull(i), dbzPixelCount[i]);*/
		/*rc = radarDB->RunQueryMeta(dbzQuery);
		sqlite3_free(dbzQuery);*/

		sqlite3_bind_int(radarDB->dbzdataInsertStmt, 1, radarId);
		sqlite3_bind_int(radarDB->dbzdataInsertStmt, 2, Index2DBZFull(i));
		sqlite3_bind_int(radarDB->dbzdataInsertStmt, 3, dbzPixelCount[i]);
		sqlite3_step(radarDB->dbzdataInsertStmt);
		sqlite3_clear_bindings(radarDB->dbzdataInsertStmt);
		sqlite3_reset(radarDB->dbzdataInsertStmt);
	}

	radarDB->RunQueryMeta("COMMIT");
	printf("\n");
	end = clock();
	dbzClock += (end - start);


	/** Update the celldata table with our information **/
	printf("\ncell data:\n");

	// Clear out any existing storm cell data for this radar file
	start = clock();
	radarDB->RunQueryMeta("BEGIN");

//	We're doing INSERT OR REPLACE so no need for expensive DELETE query
// 	char *deleteQuery = sqlite3_mprintf("DELETE FROM celldata WHERE radarid=%d", radarId);
// 	rc = radarDB->RunQuery(metaDB, deleteQuery);
// 	sqlite3_free(deleteQuery);

	// Loop through our cell list and insert into the db
	for (cellIterator = cellList.begin(); cellIterator != cellList.end(); cellIterator++)
	{
		cell = *cellIterator;
		xy = cell.xy;

		avgSize += cell.size;
		avgSigma += cell.dbzSigma;

		if (cell.size > maxCell.size) maxCell.size = cell.size;
		if (cell.dbzSigma > maxCell.dbzSigma) maxCell.dbzSigma = cell.dbzSigma;
		if (cell.dbzMax > dbzMaxSignal) dbzMaxSignal = cell.dbzMax;

//		We're using prepared statements now instead
// 		char *cellInsertQuery = sqlite3_mprintf("INSERT INTO celldata (cellid, radarid, size, dbzSigma, "
// 			" dbzAvg, dbzMax, dbzStdDev, x, y, xCenter, yCenter) VALUES (%d, %d, %d, %d, %f, %d, %f, %d, %d, %f, %f)",
// 			cell.id, id, cell.size,	cell.dbzSigma, cell.dbzAvg, cell.dbzMax, cell.dbzStdDev, xy.x, xy.y, cell.xCenter,
// 			cell.yCenter);
// 		rc = radarDB->RunQueryMeta(cellInsertQuery);
// 		sqlite3_free(cellInsertQuery);


		printf("cell ID %2d @ (%3d,%3d) (%6.2f,%6.2f), size/sigma/avg/max/stddev = %4d / %6d / %5.3f / %2d / %5.3f\n",
				cell.id, xy.x, xy.y, cell.xCenter, cell.yCenter, cell.size, cell.dbzSigma, cell.dbzAvg, cell.dbzMax,
				cell.dbzStdDev);
		sqlite3_bind_int(radarDB->celldataInsertStmt, 1, cell.id);
		sqlite3_bind_int(radarDB->celldataInsertStmt, 2, radarId);
		sqlite3_bind_int(radarDB->celldataInsertStmt, 3, cell.size);
		sqlite3_bind_int(radarDB->celldataInsertStmt, 4, cell.dbzSigma);
		sqlite3_bind_double(radarDB->celldataInsertStmt, 5, cell.dbzAvg);
		sqlite3_bind_int(radarDB->celldataInsertStmt, 6, cell.dbzMax);
		sqlite3_bind_double(radarDB->celldataInsertStmt, 7, cell.dbzStdDev);
		sqlite3_bind_int(radarDB->celldataInsertStmt, 8, xy.x);
		sqlite3_bind_int(radarDB->celldataInsertStmt, 9, xy.y);
		sqlite3_bind_double(radarDB->celldataInsertStmt, 10, cell.xCenter);
		sqlite3_bind_double(radarDB->celldataInsertStmt, 11, cell.yCenter);
		sqlite3_step(radarDB->celldataInsertStmt);
		sqlite3_clear_bindings(radarDB->celldataInsertStmt);
		sqlite3_reset(radarDB->celldataInsertStmt);
	}

	radarDB->RunQueryMeta("COMMIT");

	avgSize /= stormCellCount;
	avgSigma /= stormCellCount;
	end = clock();
	cellClock += (end - start);


	/** Update/populate radar metadata **/
	// TODO: Can probably be combined into one query
	start = clock();
	char *metaQuery1 = sqlite3_mprintf("INSERT OR REPLACE INTO radarmeta (radarid, datatype, value) VALUES (%d, %d, %d)",
		id, DATA_AVG_CELL_SIZE, (int)avgSize);
	char *metaQuery2 = sqlite3_mprintf("INSERT OR REPLACE INTO radarmeta (radarid, datatype, value) VALUES (%d, %d, %d)",
		id, DATA_AVG_CELL_SIGMA, (int)avgSigma);
	char *metaQuery3 = sqlite3_mprintf("INSERT OR REPLACE INTO radarmeta (radarid, datatype, value) VALUES (%d, %d, %d)",
		id, DATA_MAX_CELL_SIZE, maxCell.size);
	char *metaQuery4 = sqlite3_mprintf("INSERT OR REPLACE INTO radarmeta (radarid, datatype, value) VALUES (%d, %d, %d)",
		id, DATA_MAX_CELL_SIGMA, maxCell.dbzSigma);
	char *metaQuery5 = sqlite3_mprintf("INSERT OR REPLACE INTO radarmeta (radarid, datatype, value) VALUES (%d, %d, %d)",
		id, DATA_MAX_DBZ_VALUE, dbzMaxSignal);

	radarDB->RunQueryMeta("BEGIN");
	rc = radarDB->RunQueryMeta(metaQuery1);
	rc = radarDB->RunQueryMeta(metaQuery2);
	rc = radarDB->RunQueryMeta(metaQuery3);
	rc = radarDB->RunQueryMeta(metaQuery4);
	rc = radarDB->RunQueryMeta(metaQuery5);
	radarDB->RunQueryMeta("COMMIT");

	sqlite3_free(metaQuery1);
	sqlite3_free(metaQuery2);
	sqlite3_free(metaQuery3);
	sqlite3_free(metaQuery4);
	sqlite3_free(metaQuery5);
	end = clock();
	metaClock += (end - start);


	/** Release our memory **/
	cellList.clear();
	pixelList.clear();
    fclose(radarFile);
	gdImageDestroy(radarGdImage);


	/** Display results **/
	printf("\nsignal count: %d\nprecip count: %d\nstorm pixel count: %d\nstorm cell count: %d\n",
			signalPixelCount, precipPixelCount, stormPixelCount, stormCellCount);
	printf("avg cell size/sigma: %.2f / %.2f\nmax cell size/sigma: %d / %d\n", avgSize, avgSigma,
		maxCell.size, maxCell.dbzSigma);


	return stormCellCount;
}

int main(int argc, char *argv[])
{
	// files/directory
	unsigned int totalFileCount, fileMatchCount, fileCheckedCount, processedFileCount, addedFileCount;
	unsigned int skippedImages, erroredImages, removedErroredImages, id = 0;
	string radarOutput, radarPathStr, radarfile, fullradarfile;
	FILE *radarFile, *dataCache, *imageCache;
	DIR *radarDir;
	struct dirent *entry;
	list<unsigned int> epoch, epochPrime;
	list<string> radarFileList;

	// return code
	returnIdCode retIdCode;

	// regex
	regex_t re;
	size_t nmatch = 6;
	regmatch_t pmatch[6];

	// date/time
	char buffer[5];
	tm timeStruct;
	time_t thisTime, lastTime;
	clock_t beginAnalysisClock, endAnalysisClock, beginImageClock, endImageClock, skippedTime = 0;

	radarClock = dbzClock = cellClock = metaClock = 0;

	// classes
	cl_radarDB radarDB;
	cl_nexradGif radarImage;

	// sqlite3 database
	int rc;
	char *errMsg = 0;

	// getopt
	int c;

	// assign default values
	bool dryRun = false;
	bool flagCompactDB = false;
	bool flagAnalyzeDB = false;
	bool writeDataFiles = false;
	bool removedErrored = false;
	bool skipExisting = SKIP_EXISTING_RADAR;
	char *radarPath = (char *)RADAR_PATH;
	char *radarFilename = 0;
	char *station = (char *)RADAR_STATION;
	string dbPath = SQLITE_DB_PATH;

	// Catch SIGINT to shutdown app gracefully - nice feature IMHO!
	signal(SIGINT, signalHandler);

	// verify arguments
	if (argc < 2) {
		fprintf(stderr, "Usage: %s [-r radar path | -i radar image] [-d database path] [-s radar station] [-u] [-y]\n\n", argv[0]);
		fprintf(stderr, "radar path: path to radar image directory\n");
		fprintf(stderr, "radar image: single radar image to process\n");
		fprintf(stderr, "database path: path to sqlite database\n");
	    fprintf(stderr, "radar station : radar station/id to search for in radar path\n");
		fprintf(stderr, "-u: update existing radar entires (default: skip)\n");
		fprintf(stderr, "-e: remove errored files (non-GIF compliant)\n");
		fprintf(stderr, "-a: analyze database, analyze and gather consecutive images into \"storm groups\"\n");
		fprintf(stderr, "-c: compact database, remove non-storm images not associated with a storm group (implies -c)\n");
		fprintf(stderr, "-g: write gnuplot data files.  only useful during db analysis (with -a)\n");
		fprintf(stderr, "-y: drY run, display values and quit\n");
	    exit(1);
	}

	// parse args
	while ((c = getopt(argc, argv, "r:d:s:i:ueacgyD")) != -1)
	{
		switch (c) {
		case 'r':
			radarPath = optarg;
			break;
		case 'i':
			radarFilename = optarg;
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
		case 'a':
			flagAnalyzeDB = true;
			break;
		case 'c':
			flagCompactDB = true;
			break;
		case 'g':
			writeDataFiles = true;
			break;
		case 'y':
			dryRun = true;
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
	printf("opening databases...");
	radarDB.openDBs(metaDbPath.c_str(), radarDbPath.c_str());
	radarImage.setDB(&radarDB);
	printf("done\n");
	fflush(stdout);


	// Start processing our arguments
	if (radarFilename) {
//		processImage(id, radarFilename, metaDB, skipExisting);
		radarImage.setFile(radarFilename);
		radarImage.processImage(id, skipExisting);
// 		sqlite3_close(metaDB);
// 		sqlite3_close(imageDB);
		shutdownApplication(true, 0);
	}
	if (flagAnalyzeDB || flagCompactDB) {
		//analyzeDB(flagCompactDB, writeDataFiles);
		shutdownApplication(true, 0);
	}

	// Prep our regex's
	string radarFileRegx = station;
	radarFileRegx += "_([0-9]{4})([0-9]{2})([0-9]{2})_([0-9]{2})([0-9]{2})_";
	radarFileRegx += RADAR_TYPE;
	radarFileRegx += ".gif";
	if (regcomp(&re, radarFileRegx.c_str(), REG_EXTENDED) != 0) {
		printf("Unable to compile regex, aborting...\n");
		shutdownApplication(true, 1);
	}

	printf("reading in files...");
	fflush(stdout);

	// Open radar path and read in file list
	radarDir = opendir(radarPath);
    if (radarDir == NULL) {
        fprintf(stderr, "Unable to open directory %s, aborting\n", radarPathStr.c_str());
		shutdownApplication(true, 1);
    }

	// Loop through file list, if file name matches regex add to list
	totalFileCount = fileMatchCount = fileCheckedCount = processedFileCount = addedFileCount = 0;
	skippedImages = erroredImages = removedErroredImages = 0;
    while (entry = readdir(radarDir))
	{
		totalFileCount++;
		int status = regexec(&re, entry->d_name, nmatch, pmatch, 0);
		if (status == 0)
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
	// TODO: this will break when including multiple stations
	radarFileList.sort();

	printf("done, %d/%d found\n", fileMatchCount, totalFileCount);

	// Iterate through radar files and process them
	while(!radarFileList.empty() && fileCheckedCount < MAX_FILES && !shutdownSignal)
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

		lastTime = thisTime;
		thisTime = mktime(&timeStruct);
		fullradarfile = radarPathStr + radarfile;
		epoch.push_back(thisTime);

		printf("\n\nradar image : %d/%d\n", ++fileCheckedCount, fileMatchCount);
		printf("source image : %s\n", fullradarfile.c_str());
		printf("timestamp : %04d-%02d-%02d %02d:%02d, epoch = %d\n", year, month, day, hour, minute, (int)thisTime);

		radarImage.setFile(fullradarfile.c_str());
		int ret = radarImage.processImage(id, skipExisting);

		// Error with image - did not analyze, do not count processing time
		// to avoid artificially skewing processing metric higher
		if (ret <= ERROR_CANT_OPEN)
		{
			endImageClock = clock();
			skippedTime += (endImageClock - beginImageClock);
			erroredImages++;

			// If the file is non GIF-compliant and set for removal-on-error then do so
			if (ret == ERROR_NON_GIF && removedErrored)			// default is to not remove because we do not log this yet
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
		// Processing was OK, just no storm activity detected in image
		else if (ret == 0) {
			processedFileCount++;
		}
		// Processed OK!
		else if (ret > 0) {
			processedFileCount++;
 			if (radarDB.addRadarToDB(id, fullradarfile.c_str()) > 0) addedFileCount++;
 			//if (addRadarToDB(id, fullradarfile.c_str(), imageDB) > 0) addedFileCount++;
		}
    }

	endAnalysisClock = clock();
	double totalTimeElapsed = ((double) (endAnalysisClock - beginAnalysisClock - skippedTime)) / CLOCKS_PER_SEC;

	shutdownApplication(false, 0);

	printf("\nanalysis complete");
	printf("\n%d radar images analyzed", fileCheckedCount);
	printf("\n%d images processed (%d added to database)", processedFileCount, addedFileCount);
	printf("\n%d existing radar images skipped", skippedImages);
	printf("\n%d non-storm radar images removed (%d marked for removal)", removedImagesCount, markedDeletionCount);
	printf("\n%d radar images encountered errors (%d removed)", erroredImages, removedErroredImages);
	printf("\n%.02f total seconds elapsed (avg processed %.02f images/sec)\n", totalTimeElapsed,
		   ((fileCheckedCount-skippedImages)/totalTimeElapsed));

	float sum = enumPixelsClock + enumCellsClock + radarClock + dbzClock + cellClock + metaClock;
	printf("pixels:   %f / %f = %f\n", enumPixelsClock, sum, enumPixelsClock/sum);
	printf("cells:    %f / %f = %f\n", enumCellsClock, sum, enumCellsClock/sum);
	printf("radarIns: %f / %f = %f\n", radarClock, sum, radarClock/sum);
	printf("dbzIns:   %f / %f = %f\n", dbzClock, sum, dbzClock/sum);
	printf("cellIns:  %f / %f = %f\n", cellClock, sum, cellClock/sum);
	printf("metaIns:  %f / %f = %f\n", metaClock, sum, metaClock/sum);

	return EXIT_SUCCESS;
}

void signalHandler(int signum)
{
	shutdownSignal = true;
	fprintf(stderr, "Caught SIGINT, cleaning up and exiting...\n");
	signal(signum, signalHandler);
}

void shutdownApplication(bool exitOnCompletion = false, int returnCode = 0)
{
	//delete radarDB;

	if (exitOnCompletion) exit(returnCode);
}
