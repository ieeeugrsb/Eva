


set title "Roll Obtained from IMU"
set grid
set key 
#set yrange [-100:100]
set xlabel "Time"
set ylabel "Angle (Deg)"
#plot "Roll.txt" using 0:1 with lines title columnheader, "Roll.txt" using 0:2 with lines title columnheader
plot for [col=1:3] "Roll.txt" using 0:col with lines title columnheader lw 1.2
# plot for [col=1:3] "Roll.txt" using 0:col with lines title columnheader lw 3
pause mouse any
# pause 1
reread
