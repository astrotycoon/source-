set ylabel "Space complexity (bytes)"
plot "List_isEmpty.dat" using 1:3 title "List_isEmpty List_header List_first ... S(N) => 4.00", \
4.00 notitle, \
"List_isLast.dat" using 1:3 title "List_isLast List_find-(worst-case) List_findPrevious-(worst-case) S(N) => 8.00", \
8.00 notitle, \
"List_delete.dat" using 1:3 title "List_delete S(N) => -8.00", \
-8.00 notitle, \
"List_insert-(onto-first).dat" using 1:3 title "List_insert-(onto-first) S(N) => 20.00", \
20.00 notitle        
