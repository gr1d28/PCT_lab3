gcc -Wall -fopenmp runge.c -o runge -lm
./runge
gnuplot plot_runge.gnu