#!/usr/bin/gnuplot -persist

set term x11 title "WSR-88D Reflectivity Spectrogram" size 1000,400

#set grid
set title "WSR-88D Reflectivity Spectrogram"
#set timestamp "Data from: %B %d %Y (GMT)" top
set key off

#set xdata time
#set timefmt "%s"
#set format x "%H:%M"

set xlabel "Image #"
set ylabel "Signal Strength (DBZ)"
set cblabel "Pixel Count"

set palette defined (0 "black", 10 "white", 2500 "blue", 10000 "red")

datafile="`echo $GNUPLOTFILE`"

plot datafile using 1:2:3 with image

set xrange[-10:*]
replot
#pause -1

