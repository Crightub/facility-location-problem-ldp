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

gnuplot -persist <<EOF
# Labels and title
set title "Out"
set xrange [0:*]
set yrange [0:*]
set xlabel "X"
set ylabel "Y"

set size ratio -1
unset colorbox

# Define a color palette for the c column
set palette rgbformulae 33,13,10
#set palette defined (0 "blue", 1 "green", 2 "yellow", 3 "red", 4 "purple")

# Function to draw filled circles
set style fill transparent solid 0.2 noborder

set datafile separator ","

# Plot points and circles
plot "$FILENAME" using 2:3:(int(\$5) * 17 % 100) with points pt 7 ps 1 palette notitle, \
     "$FILENAME" using 2:3:(\$6) with circles lc rgb "light-blue" fs transparent solid 0.5 notitle
EOF
