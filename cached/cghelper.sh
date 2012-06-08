#!/bin/bash

if [ -z $IMAGE ]; then
	IMAGE=$1
fi

if [ -z $IMAGE ]; then
	echo "Specify image as argument or $IMAGE environment variable!"
#	exit 1
fi

valgrind --tool=callgrind ./cached -r ../radar -d /dev/shm -u
#valgrind --tool=callgrind ./cached -r $HOME/git/radarwatch/radar -d /dev/shm/radar -i $IMAGE -u
PROFPID=`ls -lrt callgrind.out.* | tail -1 | cut -d'.' -f3`
callgrind_annotate --auto=yes --threshold=75 callgrind.out.$PROFPID | tee callgrind.out.$PROFPID.log
less callgrind.out.$PROFPID.log
echo "Callgrind log written to callgrind.out.$PROFPID.log"
