set ylabel "Time complexity (number of execution steps)"
plot "shell-sort-(average-case).dat" using 1:2 title "shell-sort-(average-case) T(N) => 13.63 * NlogN", \
13.63 * x * log(x)/log(2) notitle, \
x*x title "N^2"        
