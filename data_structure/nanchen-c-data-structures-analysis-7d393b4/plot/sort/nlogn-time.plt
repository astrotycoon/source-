set ylabel "Time complexity (number of execution steps)"
plot "heap-sort-(average-case).dat" using 1:2 title "heap-sort-(average-case) T(N) => 13.65 * NlogN", \
13.65 * x * log(x)/log(2) notitle, \
"merge-sort-(average-case).dat" using 1:2 title "merge-sort-(average-case) T(N) => 23.38 * NlogN", \
23.38 * x * log(x)/log(2) notitle, \
"quick-sort-(average-case).dat" using 1:2 title "quick-sort-(average-case) T(N) => 8.16 * NlogN", \
8.16 * x * log(x)/log(2) notitle        
