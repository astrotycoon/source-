set logscale y
set ylabel "Space complexity (bytes) logscale"
plot "quick-sort-(average-case).dat" using 1:3 title "quick-sort-(average-case) S(N) => 20.99 * logN", \
20.99 * log(x)/log(2) notitle        
