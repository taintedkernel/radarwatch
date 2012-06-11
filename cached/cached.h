#ifndef CACHED_H
#define CACHED_H

#define RGB_R									0
#define RGB_G									1
#define RGB_B									2

// errors
#define ERROR_NON_GIF							-3
#define ERROR_CANT_OPEN							-2
#define ERROR_SKIP_IMAGE						-1

// flags
#define FLAG_RADAR_NO_PRECIP					1 << 0
#define FLAG_RADAR_NO_STORMS					1 << 1
#define FLAG_RADAR_NO_DATA						1 << 2
#define FLAG_RADAR_BLANK						1 << 3
//#define FLAG_RADAR_NO_DATA						1 << 4
//#define FLAG_RADAR_NO_DATA						1 << 5
//#define FLAG_RADAR_NO_DATA						1 << 6
#define FLAG_RADAR_KEEP_BEFORE					1 << 7
#define FLAG_RADAR_KEEP_AFTER					1 << 8
#define FLAG_RADAR_DELETE						1 << 9


#define OPT_QUERY_ABORTONERR					1 << 0
#define OPT_QUERY_CONTONERR						1 << 1
#define OPT_QUERY_IGNRETRY						1 << 2
#define OPT_QUERY_IGNNOROWERR					1 << 3


// enums
#define DATA_AVG_CELL_SIZE						1
#define DATA_AVG_CELL_SIGMA						2
#define DATA_MAX_CELL_SIZE						3
#define DATA_MAX_CELL_SIGMA						4
#define DATA_MAX_DBZ_VALUE						5
#define DATA_STORMGROUP_ID						6



/*** Various thresholds ***/
// DBZ thresholds
#define THRESH_DBZ_MAX_SIGNAL					75
#define THRESH_DBZ_TSTORM						50
#define THRESH_DBZ_PRE_TSTORM					45
#define THRESH_DBZ_MODERATE_PRECIP				35
#define THRESH_DBZ_LIGHT_PRECIP					20

#define T_STORM_STRENGTH     					7			// Red
#define MED_PRECIP_STRENGTH						4			// Yellow
#define PRECIP_THRESHOLD_STRENGTH				2			// Green

/*** Radar pixel count thresholds ***/
/*
	Two types of precip thresholds for determining to keep radar image:
	1. Only precip without storm activity (THRESH_RADAR_PX_PRECIP_RAW)
	2. Precip with some minimal storm activity (precip: THRESH_RADAR_PX_PRECIP_STORM + storm: THRESH_RADAR_PX_STORM)
*/
#define THRESH_RADAR_PX_SIGNAL					100
#define THRESH_RADAR_PX_PRECIP_RAW				1000
#define THRESH_RADAR_PX_PRECIP_STORM			200
#define THRESH_RADAR_PX_STORM					10
#define THRESH_RADAR_PX_NOSTORM					10			// Radar image defined as having "no storm" activity below this
//#define THRESH_RADAR_PX_CELL					100

/*#define THRESH_RADAR_PX_STORM					15
#define THRESH_RADAR_PX_PRECIP					200
#define THRESH_RADAR_PX_SIGNAL					100
#define THRESH_RADAR_PX_PRECIP_STORM			200*/

#define MAGIC									0xAD80
#define VERSION									"2.0\0"
#define DEFAULT_RADAR_OUT						"radarout.gif"
#define RADAR_PATH								"radar/"
#define CACHE_PATH								"cache/"
#define RADAR_STATION							"DIX"
#define RADAR_TYPE								"NCR"

#define MAX_FILES								25000
#define NUM_PROCESS_WORKERS						2	

#define SKIP_EXISTING_RADAR						true
#define FLAG_REMOVE_EMPTY_IMAGES				1

#define DATAFILE_PATH							"gnuplot"
#define SQLITE_DB_PATH							"/dev/shm/radar"
#define SQLITE_META_DB							"meta.db"
#define SQLITE_IMAGE_DB							"radar.db"
// #define SQLITE_META_DB							"/dev/shm/radar/meta.db"
// #define SQLITE_IMAGE_DB							"/dev/shm/radar/radar.db"


/*** General DBZ/index helper functions ***/

// We'll start full DBZ color list at 5 DBZ and increases by 5 DBZ each step
#define DBZ_COLORLIST_MIN						5
#define DBZ_COLORLIST_MAX						75
#define DBZ_COLORLIST_STEP 						5
#define DBZ_COLORLIST_LENGTH					16

#define CheckListColor(i,r,g,b,list)	( r == list[i][RGB_R] && g == list[i][RGB_G] && b == list[i][RGB_B] )
#define Index2DBZFull(index)			( (index * DBZ_COLORLIST_STEP) + DBZ_COLORLIST_MIN - 5 )		// Index starts at null
#define DBZFull2Index(dbz)				( (dbz - DBZ_COLORLIST_MIN + 5) / DBZ_COLORLIST_STEP )
#define dbz2Power(dbz)					( pow(10, dbz/10) )
#define dbz2Rainfall(dbz)				( pow(pow(10,dbz/10)/200, 0.625) )

/*
This is a highly performance-impacting data structure
Every pixel on the radar image is checked against this color list
The shorter, the better, so we are removing values less then 5 DBZ
*/
static unsigned int dbZFullColorList[][3] = {
	{0,0,0},		    // null
	{4,233,231},	    // 5DBZ
	{1,159,244},	    // 10DBZ
	{3,0,244},	    	// 15DBZ
	{2,253,2},		    // lightGreen 20DBZ
	{1,197,1},	    	// mediumGreen 25DBZ
	{0,142,0},		    // darkGreen 30DBZ
	{253,248,2},    	// lightYellow 35DBZ
	{229,188,0},	    // mediumYellow 40DBZ
	{253,139,0},	    // darkYellow 45DBZ (orange)
	{253,0,0},	    	// lightRed 50DBZ
	{212,0,0},		    // mediumRed 55DBZ
	{188,0,0},		    // darkRed 60DBZ
	{248,0,253},	    // lightPurple 65DBZ
	{152,84,198},	    // mediumPurple 70DBZ
	{253,253,253}	    // we're not in kansas anymore 75DBZ
};


/** Low strength signals (mostly noise) returned in NEXRAD 88D clean/free-air mode
We want these removed from our radar images **/
/*static unsigned int weakSignals[][3] = {
	{0,0,0},	    	// null
	{3,0,244},	    	// 15DBZ
	{1,159,244},	    // 10DBZ
	{4,233,231},	    // 5DBZ
	{100,100,100},	    // 0DBZ
	{153,153,102},  	// -5DBZ
	{204,204,153},	    // -10DBZ
	{102,51,102},	    // -15DBZ
	{153,102,153},	    // -20DBZ
};*/


// ANSI colors
#define CLR_R					"\033[0;31m"
#define CLR_Y					"\033[0;33m"
#define CLR_BG					"\033[1;32m"
#define CLR_G					"\033[0;32m"
#define CLR_GR					"\033[1;30m"
#define CLR_W					"\033[0;37m"
#define CLR_DEF					"\033[0m"


// custom data types


class cl_nexradGifProcessor;


// typedef structs
typedef struct {
	int ret;
	unsigned int id;
} returnIdCode;

typedef struct {
	unsigned short x,y;
} cartesianPair;

typedef struct {
	cartesianPair xy;
	int dbz;
} stormPixel;

typedef struct {
    cartesianPair xy;
    unsigned short id, size; //, dbzMax;
	unsigned int dbzSigma;
	stormPixel dbzMax;
	float dbzAvg, dbzStdDev, xCenter, yCenter;
} stormCell;

typedef struct {
	unsigned int id;
	unsigned int flags;
	unsigned int stormPixels, precipPixels;
	tm time;
} idFlagsTimeStruct;

typedef struct {
	void *zmqContext;
	cl_nexradGifProcessor *cppClassContext;
} pthreadFnArgs;


template<typename T>
class fixed_queue : public std::deque<T>
{
public:
	fixed_queue(size_t ms) : std::deque<T>(), max_size(ms) {}

	void push_fixed_back(const T &v)
	{
		if (std::deque<T>::size() == max_size)
			std::deque<T>::pop_front();

		std::deque<T>::push_back(v);
	}

// 	template<typename Tt>
// 	friend std::ostream &operator<<(std::ostream &os, const fixed_stack<Tt> &fs);

private:
	size_t max_size;
};




// template<typename Tt>
// std::ostream &operator<<(std::ostream & os, const fixed_stack<Tt> & fs)

// 	for(typename fixed_stack<Tt>::const_iterator it = fs.begin(); it != fs.end(); ++it // 	os << *it << " ";
// 	return os;
// }

typedef struct {
	int rc;
	int data;
} st_clRadarDbRes;


class cl_radarDB
{
private:
	bool init;
	sqlite3 *metaDB, *imageDB;

	int RunQueryGeneric(sqlite3 *db, const char *query, bool onAbort);

public:
	sqlite3_stmt *celldataInsertStmt, *dbzdataInsertStmt;

	cl_radarDB(void) { 
		init = false;
	}
	~cl_radarDB(void) {
		sqlite3_close(metaDB);
		sqlite3_close(imageDB);
	}

	// Low-level query functions
	// Eventually to be deprecated for a query API to abstract away the SQL
	int openDBs(const char *metaDBname, const char *imageDBname);
	unsigned int addRadarToDB(unsigned int id, const char *radarFilePath);
	int RunQueryMeta(const char *query, bool onAbort = false);
	//int RunQueryMeta(const char *query, bool onAbort);
	//void GetQueryResultsMeta(char *query, st_clRadarDbRes &result, bool onAbort = false);
	void GetQueryResultsMeta(char *query, st_clRadarDbRes &result, unsigned int option = OPT_QUERY_CONTONERR);
	int RunQueryImage(const char *query, bool onAbort = false);

	// Query API
	int getRadarId(const char *filename, int options);
};



//class nexradImage

//class nexradGif : public nexradImage
class cl_nexradGifProcessor
{
private:
	cl_radarDB *radarDB;
	gdImagePtr radarGdImage;
	char *radarFilename;
	void *receiver;
	pthread_t thread;
//	void *zmqContext;

public:
	cl_nexradGifProcessor(void) {
		radarFilename = NULL;
	}
	cl_nexradGifProcessor(cl_radarDB *db, void *zmqContext)
	{
		pthreadFnArgs *threadArgs = (pthreadFnArgs *)malloc(sizeof(pthreadFnArgs));
 		threadArgs->zmqContext = zmqContext;
		threadArgs->cppClassContext = this;
//		setContext(context);
//		setDB(db);
		radarDB = db;
		radarFilename = NULL;
		//pthread_create(&thread, NULL, (void *)(threadWorker), context);
		//pthread_create(&thread, NULL, &cl_nexradGifProcessor::threadWorker, context);
//		pthread_create(&thread, NULL, &cl_nexradGifProcessor::_threadWorker, (void *)(void*)threadArgs);
	}
	~cl_nexradGifProcessor(void)
	{
		// kill thread
//		pthread_join(thread, NULL);
	}

	// Static thread wrapper function
	static void* _threadWorker(pthreadFnArgs *context) {
		context->cppClassContext->threadWorker((void *)context->zmqContext);
		return NULL;
	}
	void setDB(cl_radarDB *db) { radarDB = db; }
//	void setContext(void *context) {
//		receiver = zmq_socket(context, ZMQ_REP);
//	}
	void setFile(char *filename)
	{ 
		if (radarFilename != NULL) 
			free(radarFilename);
		radarFilename = (char *)malloc(strlen(filename));
		memcpy(radarFilename,filename,strlen(filename));
	}

	inline int GetFullPixelDBZIndex(unsigned int x, unsigned int y);
	inline int GetFullPixelDBZIndex(cartesianPair xy);
	void labelCell(stormCell &cell);
	//int processImage(unsigned int &newId, const char *radarfile, sqlite3 *metaDB, bool skipExisting);
	//int processImage(unsigned int &newId, const char *radarfile, bool skipExisting);
	//int processImage(unsigned int &newId, const char *radarfile, bool skipExisting);
	int processImage(unsigned int &newId, bool skipExisting);
	void* threadWorker(void *threadZmqContext);

};


void signalHandler(int);
void shutdownApplication(bool, int);
//int RunQuery(sqlite3 *db, const char *query, bool onAbort);
//inline int GetFullPixelDBZIndex(unsigned int x, unsigned int y);
//inline int GetFullPixelDBZIndex(cartesianPair xy);
//void paintCell(stormCell &cell);
//unsigned int addRadarToDB(unsigned int id, const char *radarFilePath, sqlite3 *db);
//int analyzeGroups(bool sortOrder, unsigned int setFlag);
//int groupStorms(void);
//void analyzeDB(bool flagCompactDB, bool writeDataFiles);
//int processImage(unsigned int &newId, const char *radarfile, sqlite3 *metaDB, bool skipExisting);



#endif
