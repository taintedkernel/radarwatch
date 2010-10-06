#!/bin/bash

LVM="./radarlvm"
LVMRADAR="$LVM/radar"
LVMWARNINGS="$LVM/warnings"

RADAR="./radar"
WARNINGS="./warnings"

mount $LVM 2>/dev/null

if [ "$?" -ne 1 ]; then
	echo "LVM volume $LVM not mounted, aborting"
	exit 1
fi

if [ ! -d "$LVMRADAR" ]; then
	mkdir $LVMRADAR
fi
if [ ! -d "$LVMWARNINGS" ]; then
	mkdir $LVMWARNINGS
fi

cp -avn $RADAR/* $LVMRADAR/
cp -avn $WARNINGS/* $LVMWARNINGS/
