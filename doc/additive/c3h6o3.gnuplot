# Spalten
# 0: Gew. %
# 1: Dichte g/ml (20°C)

set term png
set grid
set key top left
set title 'Dichte von Milchsäure in abhängig der Konzentration'
set xlabel 'Konzentration [Gew. %]'
set ylabel 'Dichte [g/ml]'
set xrange [0:100]
set yrange [0.99:1.25]
# settings for csv
set datafile separator ','

# Annäherung
t=0
m=0
a=0
b=0
c=0
f(x) = t + m*x + a*x**2 + b*x**3 + c*x**4
# fit f(x) "c3h6o3.csv" using 1:2 via t, m, a, b, c
fit f(x) "c3h6o3.csv" using 1:2 via t, m

set output "c3h6o3.png"
plot "c3h6o3.csv" using 1:2 title 'Measurement', f(x) title 'Annäherung'