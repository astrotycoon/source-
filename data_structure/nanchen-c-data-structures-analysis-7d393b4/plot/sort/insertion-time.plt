set ylabel "Time complexity (number of execution steps)"
plot "insertion-sort-(average-case).dat" using 1:2 title "insertion-sort-(average-case) T(N) => 2.80 * N^2", \
2.80 * x * x notitle, \
"insertion-sort-(worst-case).dat" using 1:2 title "insertion-sort-(worst-case) T(N) => 5.51 * N^2", \
5.51 * x * x notitle        
