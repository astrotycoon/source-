set logscale y
set yrange [100:1000]
set ylabel "Time complexity (number of execution steps) logscale"
plot "insert.dat" using 1:2 title "insert T(N) => 49.35 * logN", \
49.35 * log(x)/log(2) notitle        
