#!/bin/bash

# Check if the filename is provided
if [ $# -lt 1 ]; then
    echo "Usage: $0 <filename>"
    exit 1
fi

# Assign the first argument as the filename
FILENAME=$1

# Check if the file exists
if [ ! -f "$FILENAME" ]; then
    echo "Error: File '$FILENAME' not found!"
    exit 1
fi

# Plot the points with color coding based on the number of clients
gnuplot -persist <<EOF
set xlabel "X"
set ylabel "Y"

set xrange [0:*]
set yrange [0:*]

set size ratio -1

# Use a color palette (rainbow or other)
set palette defined ( \
    0 "blue", \
    1 "cyan", \
    2 "green", \
    3 "yellow", \
    4 "orange", \
    5 "red", \
    6 "purple", \
    7 "brown", \
    8 "black" )

# Color bar for number of clients
set colorbox
set cblabel "Number of Clients"

set datafile separator ","

# Plot points with color and annotate them with costs and number of clients
plot "$FILENAME" using 2:3:5 with points pointtype 7 pointsize 1 lc palette title "Points"
EOF
