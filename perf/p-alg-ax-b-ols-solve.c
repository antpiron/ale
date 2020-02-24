#include <stdlib.h>
#include <math.h>
#include <sys/timeb.h>

#include "ale/error.h"
#include "ale/algebra.h"
#include "ale/stats.h"



int
main(int argc, char *argv[argc])
{
  size_t m = 8, n = 10, p = 5;;
  // double A[m][n],  X[n][p], B[m][p];
  double delta;
  int ret;
  double diff, rate;
  struct timeb st, et;
  const double eps = 0.0000001;

  printf("%s:\n", argv[0]);

  for (size_t m = (1l << 16)  ; m < (1l << 20) ; m <<= 1)
    {
      double (*A)[m][n] = malloc(m*n*sizeof(double));
      double (*X)[n][p] = malloc(n*p*sizeof(double));
      double (*B)[m][p] = malloc(m*p*sizeof(double));

      
      for (size_t i = 0 ; i < m ; i++)
	for (size_t j = 0 ; j < n ; j++)
	  {
	    (*A)[i][j] = (i == j)?2:1;
	  }
      for (size_t i = 0 ; i < m ; i++)
	for (size_t j = 0 ; j < p; j++)
	  {
	    double x = 0;
	    for (size_t k = 0 ; k < n ; k++)
	      (*B)[i][j] += (j+1) * (*A)[i][k];
	  }
      
      ftime(&st);
      
      ret = alg_AX_B_OLS_solve(m, n, p, *A, *B, *X);
      ERROR_UNDEF_FATAL_FMT(ret < 0, "FAIL: alg_AX_B_OLS_solve() ret = %d\n != 0", ret);
      
      ftime(&et);
      diff = (et.time - st.time) + (et.millitm - st.millitm) / 1000.0;
  
      printf("Computed A[%6zu, %3zu] X[%6zu, %3zu] = B[%6zu, %3zu] in %6.2f seconds\n", m, n, n, p, m, p, diff);

      free(A);
      free(X);
      free(B);
    }
  
  return EXIT_SUCCESS;
}
