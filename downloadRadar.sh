#!/bin/sh

RADARLOG="radardownload.log"

# function downloadRadar(<station> <type> <directory> <log>)
downloadRadar()
{
	STATION=$1
	TYPE=$2
	DIR=$3
 	LOG=$4
	TMPFILE=`mktemp`

	echo "Starting wget $TYPE/$STATION on `date`:" | tee -a $RADARLOG
	wget -r -nc -l1 --no-parent --no-directories --no-host-directories -P$DIR -A.gif http://radar.weather.gov/ridge/RadarImg/$TYPE/$STATION 2>&1 | tee $TMPFILE
	cat $TMPFILE | egrep 'FINISHED|Downloaded|Saving|Removing' | grep -v 'index.html' >> $LOG && rm $TMPFILE
}


if [ "$#" -lt 2 ]; then
    echo $#
	echo "Usage: $0 <radar station> <radar type> <radar directory> <warning directory>"
	echo "where <radar station> is a 3-letter NEXRAD site ID"
	echo "<radar type> is of"
	echo "	N0R - base reflectivity"
	echo "	NCR - composite reflectivity"
	echo "	N0S - storm relative motion"
	echo "	ALL - both NCR and N0S"
	echo "<radar directory> - directory to store radar files"
	echo "<warning directory> - directory to store warning overlay files"
	exit
fi


RADARSTATION=$1
RADARTYPE=$2
RADARDIR=$3
WARNINGS=$4

if [ -z "$3" ]; then
    RADARDIR="."
fi

echo | tee -a $RADARLOG
echo -n "$0 $@ called on " | tee -a $RADARLOG
date | tee -a $RADARLOG

# Loop through RADARSTATIONS and download all files of types IMAGETYPE
for STATION in $RADARSTATION
do {
	echo | tee -a $RADARLOG
	echo " --== Processing station $STATION ==--" | tee -a $RADARLOG

	for IMAGETYPE in $RADARTYPE
	do {
		echo | tee -a $RADARLOG
		echo "Downloading <{ $IMAGETYPE }> images for station <{ $STATION }>" | tee -a $RADARLOG
		downloadRadar $STATION $IMAGETYPE $RADARDIR $RADARLOG
	} done

	# Download warning overlays
	if [ ! -z "$WARNINGS" ]; then
		WARNINGTMP=`mktemp`
		echo | tee -a $RADARLOG
		echo "Updating NWS severe weather warnings for station <{ $STATION }>" | tee -a $RADARLOG
 		wget -r -nc -l1 --no-parent --no-directories --no-host-directories -P$WARNINGS -A.gif -R "*_Short_Warnings*" http://radar.weather.gov/ridge/Warnings/Short/$RADARSTATION 2>&1 | tee -a $WARNINGTMP
 		find $WARNINGS -type f -name *Short_Warnings.gif -print | xargs /bin/rm 2>/dev/null
		cat $WARNINGTMP | egrep 'FINISHED|Downloaded|Saving|Removing' | grep -v 'index.html' >> $LOG && rm $WARNINGTMP
	fi
} done

exit
