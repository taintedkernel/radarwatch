#!/bin/bash

if [ "$#" -lt "2" ]; then
	echo "Usage: $0 YEAR MONTH [PATH] (as 4 and 2-digit dates)"
	echo "Tars up files in PATH (defaults to ./radar) matching regex:"
	echo "	'[A-Z]{3}_$YR$MO[0-9]{2}.*' "
fi

YR="$1"
MO="$2"
DIR="./radar"
ARCHIVE="/dev/shm/radar-$YR-$MO-archive.tar"

if [ ! -z "$3" ]; then
	DIR="$3"
fi

echo "Tarring up files in $DIR matching $YR-$MO"

find $DIR -regextype posix-egrep -regex ".*/[A-Z]{3}_$YR$MO[0-9]{2}.*" -print | sort | \
	xargs -t tar -uvf $ARCHIVE


