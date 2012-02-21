set ylabel "Time complexity (number of execution steps)"
plot "heap-sort-(average-case).dat" using 1:2 title "heap-sort-(average-case) T(N) => 13.65 * NlogN", \
13.65 * x * log(x)/log(2) notitle        
