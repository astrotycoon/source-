set ylabel "Space complexity (bytes)"
plot "find.dat" using 1:3 title "find S(N) => 8.00", \
8.00 notitle, \
"find-min.dat" using 1:3 title "find-min find-max retrieve S(N) => 4.00", \
4.00 notitle        
