#!/bin/bash

while [ 1 ];
do
    echo
	echo "radarwatch main menu"
	echo "1. start radarwatchd"
	echo "2. stop radarwatchd"
	echo "3. restart radarwatchd"
	echo "4. radarwatchd status"
	echo "5. run radarwatch DIX"
	echo "6. run gthumb"
	echo "q. quit"

	read -p "Choice? "
	echo

	case "$REPLY" in
	1)
		perl radarwatchd.pl start
		;;
	2)
		perl radarwatchd.pl stop
		;;
	3)
		perl radarwatchd.pl restart
		;;
	4)
		perl radarwatchd.pl status
		;;
	5)
		perl radarwatch.pl DIX
		;;
	6)
		gthumb &
		;;
	q | Q)
		exit
		;;
	*)
		echo "Choice not understood";
		;;
	esac

done
