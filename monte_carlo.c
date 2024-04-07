#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <stdlib.h>

// const double PI = 3.14159265358979323846;
const int n1 = 10000000;
const int n2 = 100000000;

double getrand()
{ return  (double)rand() / RAND_MAX; }

double getrand_p(unsigned int *seed)
{ return (double)rand_r(seed) / RAND_MAX; }

double func(double x, double y)
{ return exp(x - y); }


void serial(int n)
{
  int in = 0;
  double s = 0;
  for (int i = 0; i < n; i++)
  {
    double x = getrand() * (-1);
    double y = getrand();
    if (y <= 1) {
      in++;
      s += func(x, y);
    }
  }
  double v = in / n;
  double res = v * s / in;
  printf("Result: %.12f, n %d\n", res, n);
}

void parallel(int n, int count_threads)
{
  printf("Numerical integration by Monte Carlo method: n = %d\n", n);
  int in = 0;
  double s = 0;
  #pragma omp parallel num_threads(count_threads)
  {
    double s_loc = 0;
    int in_loc = 0;
    unsigned int seed = omp_get_thread_num();
    #pragma omp for nowait
    for (int i = 0; i < n; i++)
    {
      double x = getrand(&seed) * (-1);
      double y = getrand(&seed);
      if (y <= 1)
      {
        in_loc++;
        s_loc += func(x, y);
      }
    }
    #pragma omp atomic
    s += s_loc;
    #pragma omp atomic
    in += in_loc;
  }
  double v = in / n;
  double res = v * s / in;
  printf("Result of %d threads: %.12f, n %d\n", count_threads, res, n);
}

int main(int argc, char *argv[])
{
  double t_serial = 0, t_parallel = 0, boost = 0;
  char buf[10] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'}, bufn[5] = { ' ', ' ', ' ', ' ', '\0'};
  FILE *fp = NULL;
  printf("Monte Carlo n = %d:\n", n1);
  fp = fopen("MonteCarlo_data1.dat", "wb");
  t_serial = omp_get_wtime();
  serial(n1);
  t_serial = omp_get_wtime() - t_serial;
  bufn[0] = '1';
  buf[0] = '1';
  buf[8] = '\n';
  fputs(bufn, fp);
  fputs(buf, fp);
  printf("Serial time: %.6f\n", t_serial);
  for(int i = 2; i <= 8; i += 2)
  {
    t_parallel = omp_get_wtime();
    parallel(n1, i);
    t_parallel = omp_get_wtime() - t_parallel;
    boost = t_serial / t_parallel;
    bufn[0] = i + '0';
    sprintf(buf, "%.6f", boost);
    buf[8] = '\n';
    fputs(bufn, fp);
    fputs(buf, fp);
    printf("Parallel time of %d threads: %.6f\n", i, t_parallel);
  }
  fclose(fp);
  fp = NULL;

  printf("Monte Carlo n = %d:\n", n2);
  fp = fopen("MonteCarlo_data2.dat", "wb");
  t_serial = omp_get_wtime();
  serial(n2);
  t_serial = omp_get_wtime() - t_serial;
  bufn[0] = '1';
  buf[0] = '1';
  buf[8] = '\n';
  fputs(bufn, fp);
  fputs(buf, fp);
  printf("Serial time: %.6f\n", t_serial);
  for(int i = 2; i <= 8; i += 2)
  {
    t_parallel = omp_get_wtime();
    parallel(n2, i);
    t_parallel = omp_get_wtime() - t_parallel;
    boost = t_serial / t_parallel;
    bufn[0] = i + '0';
    sprintf(buf, "%.6f", boost);
    buf[8] = '\n';
    fputs(bufn, fp);
    fputs(buf, fp);
    printf("Parallel time of %d threads: %.6f\n", i, t_parallel);
  }
  fclose(fp);

  return 0;
}