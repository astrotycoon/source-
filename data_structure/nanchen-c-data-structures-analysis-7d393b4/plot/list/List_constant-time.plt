set ylabel "Time complexity (number of execution steps)"
plot "List_isEmpty.dat" using 1:2 title "List_isEmpty List_isLast T(N) => 4.00", \
4.00 notitle, \
"List_delete.dat" using 1:2 title "List_delete T(N) => 27.00", \
27.00 notitle, \
"List_findPrevious-(worst-case).dat" using 1:2 title "List_findPrevious-(worst-case) T(N) => 10.00", \
10.00 notitle, \
"List_insert-(onto-first).dat" using 1:2 title "List_insert-(onto-first) T(N) => 14.00", \
14.00 notitle, \
"List_header.dat" using 1:2 title "List_header T(N) => 2.00", \
2.00 notitle, \
"List_first.dat" using 1:2 title "List_first List_advance List_retrieve T(N) => 3.00", \
3.00 notitle        
