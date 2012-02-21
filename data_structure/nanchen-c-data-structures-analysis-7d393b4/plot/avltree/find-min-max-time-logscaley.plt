set logscale y
set ylabel "Time complexity (number of execution steps) logscale"
plot "find-min.dat" using 1:2 title "find-min T(N) => 6.24 * logN", \
6.24 * log(x)/log(2) notitle, \
"find-max.dat" using 1:2 title "find-max T(N) => 2.69 * logN", \
2.69 * log(x)/log(2) notitle        
