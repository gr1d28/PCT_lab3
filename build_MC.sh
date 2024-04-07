gcc -Wall -fopenmp monte_carlo.c -o monte_carlo -lm
./monte_carlo
gnuplot plot_MC.gnu