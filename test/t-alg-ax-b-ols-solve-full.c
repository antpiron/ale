#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/algebra.h"
#include "ale/stats.h"
#include "ale/math.h"

#define MAX_ITER (10)

const double alpha = 0.01;
const double eps = 0.0000001;

enum {
      RANDOM_B,
      PERFECT_B,
      CORRELATED_B
};

void
gen_model(char *name, size_t m, size_t n, size_t p, size_t intercept, int rand_B, double var_e, void *A_param, void *B_param)
{
  int ret;
  struct alg_ols ols;
  double A[m][n],  X[n][p], B[m][p], X_res[n][p];

  printf("\n\n%s\n", name);

  if (A_param)
    memcpy(A, A_param, sizeof(double) * m * n);
  else
    ALG_INIT_M(m, n, A, (intercept && (0 == j))? 1  : stats_unif_rand(0, 1000) );

  // generate B
  if ( B_param )
    memcpy(B, B_param, sizeof(double) * m * p);
  else
  if ( RANDOM_B == rand_B )
    ALG_INIT_M(m, p, B, stats_unif_rand(0, 1000) );
  else
    {
      ALG_INIT_M(n, p, X_res, stats_unif_rand(0, 1000) );
      
      alg_mul_m_m(m, n, p, A, X_res, B);
 
      if ( CORRELATED_B  == rand_B)
	{
	  ALG_ADD_M(m, p, B, stats_norm_rand(0, var_e) );
	}
    }

  printf("\nA=\n");
  print_m(m, n, A);
  printf("\nB=\n");
  print_m(m, p, B);
  if ( RANDOM_B != rand_B)
    {
      printf("\nX_res=\n");
      print_m(n, p, X_res);
    }

  ret = alg_AX_B_OLS_init(&ols, m, n, p, A, B, X);
  ERROR_UNDEF_FATAL_FMT(ret < 0, "FAIL: %s alg_AX_B_OLS_init ret = %d\n != 0", name, ret);

  printf("\nX=\n");
  print_m(n, p, X);

  if ( PERFECT_B  == rand_B )
    {
      for (size_t i = 0 ; i < n ; i++)
	for (size_t j = 0; j < p ; j++)
	  {
	    ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(X[i][j], X_res[i][j], eps),
				  "FAIL: %s alg_AX_B_OLS_solve_full() X[%ld, %ld] = %f != %f = X_res[%ld, %ld]\n",
				  name, i, j, X[i][j], X_res[i][j], i, j);
	  }
    }

  
  ret = alg_AX_B_OLS_statistics(&ols, intercept);
  ERROR_UNDEF_FATAL_FMT(ret < 0, "FAIL: %s alg_AX_B_OLS_statistics() ret = %d\n != 0", name, ret);

  printf("\nAX=\n");
  print_m(m, p, ols.AX);
  printf("\nmeans=\n");
  print_v(p, ols.means);

  printf("\nrss=\n");
  print_v(p, ols.rss);
  printf("\nmss=\n");
  print_v(p, ols.mss);

  printf("\nr²=\n");
  print_v(p, ols.r_squared);

  printf("\nF-stat=\n");
  print_v(p, ols.score);

  printf("\nF-pval=\n");
  print_v(p, ols.pvalue);

  alg_AX_B_OLS_destroy(&ols);
}

int
main(int argc, char *argv[argc])
{
  struct alg_ols ols;
  const size_t m = 30, n = 3, p = 3;;
  double A[m][n],  X[n][p], B[m][p];
  double B_copy[m][p];
  double delta;
  int ret;
  const double alpha = 0.01;
  const double eps = 0.0000001;


  gen_model("random with intercept", 6, 3, 1, 1, RANDOM_B, 0,
	    (double[6][3]) { {1.000000,	751.411799, 53.406825},	
			      {1.000000, 878.793326, 184.466027},
				 {1.000000, 934.970735, 400.979769},
				   {1.000000, 248.113489, 6.991620 },
				     {1.000000, 969.135640, 443.855135},
				     {1.000000, 397.544328, 330.078718 } },
	    (double[6]) {216.396249, 852.721703, 234.933271, 446.330890, 161.112500, 863.669808});

  /* gen_model("random with intercept", 6, 3, 1, 1, RANDOM_B, 0, NULL, NULL);
  gen_model("perfect correlation with intercept", 6, 3, 1, 1, PERFECT_B, 0, NULL, NULL);
  gen_model("correlation with intercept", 6, 3, 1, 1, CORRELATED_B, 10, NULL, NULL); */

  return(0);
  
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
      printf("pvalue = %f ; score = %f ; r² = %f\n", pvalue[i], score[i], r_squared[i]);
    }
  printf("\n");
  
  for (size_t i = 0 ; i < n ; i++)
    {
      for (size_t j = 0; j < p ; j++)
	{
	  //  alpha / (m * p) == Bonferroni correction
	  const double bonf_alpha = alpha / (n * p);
	  double (*beta_pvalue)[p] = ols.beta_pvalue;
	  double (*beta_score)[p] = ols.beta_score;

	  if (0 != i)
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
