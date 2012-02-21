set ylabel "Time complexity (number of execution steps)"
plot "merge-sort-(average-case).dat" using 1:2 title "merge-sort-(average-case) T(N) => 23.38 * NlogN", \
23.38 * x * log(x)/log(2) notitle        
