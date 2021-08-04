#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/algebra.h"
#include "ale/stats.h"
#include "ale/math.h"

#define MAX_ITER (10)

int
main(int argc, char *argv[argc])
{
  /* const size_t m = 10, n = 3, p = 3;;
  double A[m][n],  X[n][p], B[m][p];
  struct stats_stastistic stat[n][p];
  double B_copy[m][p];
  double delta;
  int ret;
  const double alpha = 0.01;
  const double eps = 0.0000001;

  ALG_INIT_M(m, n, A, (i == j)?2:1);

  for (size_t i = 0 ; i < m ; i++)
    for (size_t j = 0 ; j < p; j++)
      {
	B[i][j] = 0;
	
	for (size_t k = 0 ; k < n ; k++)
	  B[i][j] += (j+1)*A[i][k];

	B_copy[i][j] = B[i][j];
      }
  
  printf("\nA=\n");
  print_m(m, n, A);
  printf("\nB=\n");
  print_m(m, p, B);
  
  ret = alg_AX_B_OLS_solve_full(m, n, p, A, B, X, stat);
  ERROR_UNDEF_FATAL_FMT(ret < 0, "FAIL: alg_AX_B_OLS_solve_full() ret = %d\n != 0", ret);
  
  printf("\nX=\n");
  print_m(n, p, X);

  alg_mul_m_m(m,n,p, A, X, B);
  printf("\nB=\n");
  print_m(m, p, B);


  for (size_t i = 0 ; i < m ; i++)
    for (size_t j = 0; j < p ; j++)
    {
      
      delta = fabs(B[i][j] - B_copy[i][j]);
      ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: alg_AX_B_OLS_solve_full() delta B[%ld, %ld] != 0 = %f\n", i, j, delta);
    }

  ALG_INIT_M(m, n, A, stats_unif_rand(0, 1000));
  ALG_INIT_M(m, p, B, stats_unif_rand(0, 1000));

  ret = alg_AX_B_OLS_solve_full(m, n, p, A, B, X, stat);
  ERROR_UNDEF_FATAL_FMT(ret < 0, "FAIL: alg_AX_B_OLS_solve_full() ret = %d\n != 0", ret);

  for (size_t i = 0 ; i < n ; i++)
    {
      for (size_t j = 0; j < p ; j++)
	{
	  //  alpha / (m * p) == Bonferroni correction
	  const double bonf_alpha = alpha / (n * p);
	  ERROR_UNDEF_FATAL_FMT(stat[i][j].pvalue <= bonf_alpha,
				"FAIL: alg_AX_B_OLS_solve_full() stat[%ld][%ld].pvalue = %f <=  %f (beta = %f ; score = %e ; mse = %e)\n",
				i, j, stat[i][j].pvalue, bonf_alpha, X[i][j], stat[i][j].score, stat[i][j].mse);
	}
    }
  
  for (int iter = 0 ; iter < MAX_ITER ; iter++)
    {
      size_t m = 100, n = 2, p = 15;
      double b[n][p], b_res[n][p];
      double Y[m][p], X[m][n];
      struct stats_stastistic stat[n][p];


      ALG_INIT_M(n, p, b, stats_unif_rand(0, 1000));
      ALG_INIT_M(m, n, X, stats_unif_rand(0, 1000));

      alg_mul_m_m(m, n, p, X, b, Y);

      ALG_ADD_M(m, p, Y, stats_norm_rand(0, 1));

      ret = alg_AX_B_OLS_solve_full(m, n, p, X, Y, b_res, stat);
      ERROR_UNDEF_FATAL_FMT(ret < 0, "FAIL: alg_AX_B_OLS_solve_full() ret = %d\n != 0", ret);

      for (size_t i = 0 ; i < n ; i++)
	{
	  for (size_t j = 0; j < p ; j++)
	    {
	      const double eps = 0.01;
	      ERROR_UNDEF_FATAL_FMT(stat[i][j].pvalue > 0.05,
				    "FAIL: alg_AX_B_OLS_solve_full() stat[%ld][%ld].pvalue = %f >  0.05 (score = %e ; mse = %e)\n",
				    i, j, stat[i][j].pvalue, stat[i][j].score, stat[i][j].mse);
	      printf("%10.6e ", stat[i][j].score);

	      delta = fabs(b[i][j] - b_res[i][j]);
	      ERROR_UNDEF_FATAL_FMT( 0 != ale_cmp_double(b[i][j], b_res[i][j], eps) ,
	      			     "FAIL: alg_AX_B_OLS_solve_full() b_res[%ld][%ld] = %f !=  %f = b[%ld][%ld] ; delta = %f > %f = eps\n",
	      			     i, j, b_res[i][j], b[i][j], i, j, delta, eps);
	      
	    }
	  printf("\n");
	}
	  
      printf("\n");
    }
  */ 
  return EXIT_SUCCESS;
}
