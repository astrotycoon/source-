set ylabel "Time complexity (number of execution steps)"
plot "List_makeEmpty.dat" using 1:2 title "List_makeEmpty T(N) => 4.10 * N", \
4.10 * x notitle, \
"List_find-(worst-case).dat" using 1:2 title "List_find-(worst-case) T(N) => 2.05 * N", \
2.05 * x notitle        
