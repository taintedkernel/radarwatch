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
#define FLAG_RADAR_NO_PRECIP					1
#define FLAG_RADAR_NO_STORMS					2
#define FLAG_RADAR_DELETE						4
#define FLAG_RADAR_NOREMOVE_CONTINUITY			8

/*** Various thresholds ***/
// DBZ thresholds
#define THRESH_DBZ_MAX_SIGNAL					75
#define THRESH_DBZ_TSTORM						50
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
#define THRESH_RADAR_PX_STORM					25
#define THRESH_RADAR_PX_NOSTORM					10			// Radar image defined as having "no storm" activity below this
//#define THRESH_RADAR_PX_CELL					100

#define MAGIC									0xAD80
#define VERSION									"2.0\0"
#define DEFAULT_RADAR_OUT						"radarout.gif"
#define RADAR_PATH								"radar/"
#define CACHE_PATH								"cache/"
#define RADAR_STATION							"DIX"
#define RADAR_TYPE								"NCR"

#define MAX_FILES								15000

#define SKIP_EXISTING_RADAR						true
#define FLAG_REMOVE_EMPTY_IMAGES				1

#define SQLITE_DB_PATH							"."
#define SQLITE_META_DB							"meta.db"
#define SQLITE_IMAGE_DB							"radar.db"
// #define SQLITE_META_DB						"/dev/shm/radar/meta.db"
// #define SQLITE_IMAGE_DB						"/dev/shm/radar/radar.db"


/*** General DBZ/index helper functions ***/

// We'll start full DBZ color list at 5 DBZ and increases by 5 DBZ each step
#define DBZ_COLORLIST_MIN						5
#define DBZ_COLORLIST_MAX						75
#define DBZ_COLORLIST_STEP 						5
#define DBZ_COLORLIST_LENGTH					16

#define CheckListColor(i,r,g,b,list)	(r == list[i][RGB_R] && g == list[i][RGB_G] && b == list[i][RGB_B])
#define Index2DBZFull(index)			((index * DBZ_COLORLIST_STEP) + DBZ_COLORLIST_MIN - 5)		// Index starts at null
#define DBZFull2Index(dbz)				((dbz - DBZ_COLORLIST_MIN + 5) / DBZ_COLORLIST_STEP)


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
#define CLR_G					"\033[1;32m"
#define CLR_BG					"\033[0;32m"
#define CLR_GR					"\033[1;30m"
#define CLR_W					"\033[0;37m"
#define CLR_DEF					"\033[0m"


// custom data types

// typedef structs
typedef struct {
	unsigned short x,y;
} cartesianPair;

typedef struct {
    cartesianPair xy;
    unsigned short id, size;
} stormCell;

typedef struct {
	unsigned int id;
	tm time;
} idTimeStruct;


template<typename T>
class fixed_stack : public std::deque<T>
{
public:
	fixed_stack(size_t ms) : std::deque<T>(), max_size(ms) {}

	void push_front(const T &v)
	{
		if (std::deque<T>::size() == max_size)
			std::deque<T>::pop_back();

		std::deque<T>::push_front(v);
	}

// 	template<typename Tt>
// 	friend std::ostream &operator<<(std::ostream &os, const fixed_stack<Tt> &fs);

private:
	size_t max_size;
};

// template<typename Tt>
// std::ostream &operator<<(std::ostream & os, const fixed_stack<Tt> & fs)
// {
// 	for(typename fixed_stack<Tt>::const_iterator it = fs.begin(); it != fs.end(); ++it )
// 	os << *it << " ";
// 	return os;
// }


void signalHandler(int);

#endif