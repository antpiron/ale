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
  struct alg_ols ols;
  const size_t m = 100, n = 3, p = 3;;
  double A[m][n],  X[n][p], B[m][p];
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

  ret = alg_AX_B_OLS_init(&ols, m, n, p, A, B, X);
  ERROR_UNDEF_FATAL_FMT(ret < 0, "FAIL: alg_AX_B_OLS_init ret = %d\n != 0", ret);
  
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
  
  alg_AX_B_OLS_destroy(&ols);

  ALG_INIT_M(m, n, A, (j == 0)?1:stats_unif_rand(0, 1000));
  ALG_INIT_M(m, p, B, stats_unif_rand(0, 1000));
  printf("\nA=\n");
  print_m(m, n, A);
  printf("\nB=\n");
  print_m(m, p, B);
  

  ret = alg_AX_B_OLS_init(&ols, m, n, p, A, B, X);
  ERROR_UNDEF_FATAL_FMT(ret < 0, "FAIL: alg_AX_B_OLS_solve_full() ret = %d\n != 0", ret);

  printf("\nX=\n");
  print_m(n, p, X);

  ret = alg_AX_B_OLS_statistics(&ols, 1);
  ERROR_UNDEF_FATAL_FMT(ret < 0, "FAIL: alg_AX_B_OLS_statistics() ret = %d\n != 0", ret);
  
  for (size_t i = 0; i < p ; i++)
    {
      double *r_squared = ols.r_squared;
      double *pvalue = ols.pvalue;
      double *score = ols.score;
      const double bonf_alpha = alpha / p;

      ERROR_UNDEF_FATAL_FMT(0 == ale_cmp_double(r_squared[i], 1, eps),
			    "FAIL: RANDOM alg_AX_B_OLS_statistics() r_squared[%zu] = %f\n == 1.0\n",
			    i, r_squared[i]);
      ERROR_UNDEF_FATAL_FMT(pvalue[i] <= bonf_alpha ,
			    "FAIL: RANDOM alg_AX_B_OLS_statistics() pvalue[%zu] = %f <= %f (r_squared[%zu] = %f, score[i] = %f)\n",
			    i, pvalue[i], bonf_alpha, i, r_squared[i], i, score[i]);
      printf("%f\t", pvalue[i]);
    }
  
  for (size_t i = 0 ; i < n ; i++)
    {
      for (size_t j = 0; j < p ; j++)
	{
	  //  alpha / (m * p) == Bonferroni correction
	  const double bonf_alpha = alpha / (n * p);
	  double (*beta_pvalue)[p] = ols.beta_pvalue;
	  double (*beta_score)[p] = ols.beta_score;
	  
	  ERROR_UNDEF_FATAL_FMT(beta_pvalue[i][j] <= bonf_alpha,
				"FAIL: RANDOM alg_AX_B_OLS_statistics() beta_pvalue[%ld][%ld] = %f <=  %f (beta = %f ; score = %e)\n",
				i, j, beta_pvalue[i][j], bonf_alpha, X[i][j], beta_score[i][j]);
	}
    }
  alg_AX_B_OLS_destroy(&ols);
  
  for (int iter = 0 ; iter < MAX_ITER ; iter++)
    {
      size_t m = 100, n = 2, p = 15;
      double b[n][p], b_res[n][p];
      double Y[m][p], X[m][n];


      ALG_INIT_M(n, p, b, stats_unif_rand(0, 1000));
      ALG_INIT_M(m, n, X, stats_unif_rand(0, 1000));

      alg_mul_m_m(m, n, p, X, b, Y);

      ALG_ADD_M(m, p, Y, stats_norm_rand(0, 1));

      ret = alg_AX_B_OLS_init(&ols, m, n, p, X, Y, b_res);
      ERROR_UNDEF_FATAL_FMT(ret < 0, "FAIL: alg_AX_B_OLS_init() ret = %d\n != 0", ret);

      ret = alg_AX_B_OLS_statistics(&ols, 0);
      ERROR_UNDEF_FATAL_FMT(ret < 0, "FAIL: alg_AX_B_OLS_statistics() ret = %d\n != 0", ret);

      for (size_t i = 0; i < p ; i++)
	{
	   double *r_squared = ols.r_squared;
	   ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(r_squared[i], 1, eps), "FAIL: alg_AX_B_OLS_statistics() r_squared[%zu] = %f\n != 1.0", i, r_squared[i]);
	}
      
      for (size_t i = 0 ; i < n ; i++)
	{
	  for (size_t j = 0; j < p ; j++)
	    {
	      const double eps = 0.01;
	      double (*beta_pvalue)[p] = ols.beta_pvalue;
	      double (*beta_score)[p] = ols.beta_score;
	      ERROR_UNDEF_FATAL_FMT(beta_pvalue[i][j] > 0.05,
				    "FAIL: alg_AX_B_OLS_statistics() beta_pvalue[%ld][%ld] = %f >  0.05 (score = %e)\n",
				    i, j, beta_pvalue[i][j], beta_score[i][j]);
	      printf("%10.6e ", beta_score[i][j]);

	      delta = fabs(b[i][j] - b_res[i][j]);
	      ERROR_UNDEF_FATAL_FMT( 0 != ale_cmp_double(b[i][j], b_res[i][j], eps) ,
	      			     "FAIL: alg_AX_B_OLS_init() b_res[%ld][%ld] = %f !=  %f = b[%ld][%ld] ; delta = %f > %f = eps\n",
	      			     i, j, b_res[i][j], b[i][j], i, j, delta, eps);
	      
	    }
	  printf("\n");
	}
	  
      printf("\n");
      alg_AX_B_OLS_destroy(&ols);	
    }
   
  return EXIT_SUCCESS;
}
