set terminal png
set output 'u.png'
set lmargin at screen 0.15
set rmargin at screen 0.85
set bmargin at screen 0.15
set tmargin at screen 0.85
set xr[-5:5] #0.025
set yr[-5:5]  #0.15
set size ratio 1
set xlabel "x"
set ylabel "y"
set title "n_t = 25000"
#set cbrange [-0.25:0.25]
set palette defined ( 0 "#000090",\
                      1 "#000fff",\
                      2 "#0090ff",\
                      3 "#0fffee",\
                      4 "#90ff70",\
                      5 "#ffee00",\
                      6 "#ff7000",\
                      7 "#ee0000",\
                      8 "#7f0000")
#set grid lw 2 lt 6
set xtics out
set ytics out
set pm3d map
#set pm3d interpolate 0,0
set size 0.707,1
splot 'velocity.sol' u 1:2:3
reset

set terminal png
set output 'v.png'
set lmargin at screen 0.15
set rmargin at screen 0.85
set bmargin at screen 0.15
set tmargin at screen 0.85
set xr[-5:5] #0.025
set yr[-5:5]  #0.15
set size ratio 1
set xlabel "x"
set ylabel "y"
set title "n_t = 25000"
#set cbrange [-0.25:0.25]
set palette defined ( 0 "#000090",\
                      1 "#000fff",\
                      2 "#0090ff",\
                      3 "#0fffee",\
                      4 "#90ff70",\
                      5 "#ffee00",\
                      6 "#ff7000",\
                      7 "#ee0000",\
                      8 "#7f0000")
#set grid lw 2 lt 6
set xtics out
set ytics out
set pm3d map
#set pm3d interpolate 0,0
set size 0.707,1
splot 'velocity.sol' u 1:2:4
reset