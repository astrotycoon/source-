set ylabel "Time complexity (number of execution steps)"
plot "quick-sort-(average-case).dat" using 1:2 title "quick-sort-(average-case) T(N) => 8.16 * NlogN", \
8.16 * x * log(x)/log(2) notitle        
