#!/usr/bin/gnuplot -persist
set term wxt

datafile="`echo $GNUPLOTFILE`"

set grid
set title "DBZ pixel counts vs time (GMT)"
#set timestamp "Data from: %B %d %Y (GMT)" datafile top
set key left top

set xdata time
set timefmt "%s"				# Use date/times as epoch seconds
#set format x "%d/%H:%M"
set xtics rotate format "%m-%d_%H:%M"
set xlabel "timestamp"

set ylabel "stormPixels"
set y2label "precip/signalPixels"
set yrange [0:2000]
set y2range [0:100000]
set y2tics nomirror

plot datafile using 1:2 with linespoints lt 1 lw 2 ti "storm (50+ DBZ)" axis x1y1,\
	 datafile using 1:3 with linespoints lt 8 lw 1 ti "precip (35+ DBZ)" axis x1y2,\
	 datafile using 1:4 with linespoints lt 2 lw 1 ti "signal (20+ DBZ)" axis x1y2,\
	 datafile using 1:($2*$2*30+$3*5+$4) with linespoints lw 1 ti "strength" axis x1y1

#	 1000 lt 2 lw 1 axis x1y2,\
#	 200 lt 8 lw 1 axis x1y2,\
#	 25 lt 1 lw 2 axis x1y1

set autoscale y2max
set autoscale ymax
replot
pause -1




