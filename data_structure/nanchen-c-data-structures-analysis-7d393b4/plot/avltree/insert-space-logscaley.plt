set logscale y
set yrange [70:200]
set ylabel "Space complexity (bytes) logscale"
plot "insert.dat" using 1:3 title "insert S(N) => 12.28 * logN", \
12.28 * log(x)/log(2) notitle        
