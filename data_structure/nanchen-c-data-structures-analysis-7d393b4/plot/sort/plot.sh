#!/bin/sh

axis="../setAxisTitle.plt"
if [ $2 = time ]; then
  title="Time complexity of operations of sorting"
else
  title="Space complexity of operations of sorting"
fi


gnuplot << EOF

set terminal gif
set output "$1.gif"

set grid

load "$axis"
set title "$title"

load "$1"

EOF
