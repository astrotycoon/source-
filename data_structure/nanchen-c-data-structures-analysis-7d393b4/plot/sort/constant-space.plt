set ylabel "Space complexity (bytes)"
plot "insertion-sort-(average-case).dat" using 1:3 title "insertion-sort-(average-case) shell-sort-(average-case) S(N) => 8.00", \
8.00 notitle, \
"heap-sort-(average-case).dat" using 1:3 title "heap-sort-(average-case) S(N) => 20.00", \
20.00 notitle        
