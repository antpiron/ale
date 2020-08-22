#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"
#include "ale/math.h"

int
main(int argc, char *argv[argc])
{
  double eps = 1e-14, res, delta;
  double exp;


  
  res = stats_hyper_f(1, 5, 5, 10);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_hyper_f(1, 5, 5, 10) == NaN\n");
  exp = 9.92063492063491314532086562394397333264350891113281e-02d;
  ERROR_UNDEF_FATAL_FMT(0 != ale_doublecmp(exp, res, eps),
			"FAIL: stats_hyper_f(1, 5, 5, 10) == %.20f != %.20f\n", res, exp);

  res = stats_hyper_f(2, 5, 5, 10);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_hyper_f(2, 5, 5, 10) == NaN\n");
  exp = 3.96825396825396692346288318731239996850490570068359e-01d;
  ERROR_UNDEF_FATAL_FMT(0 != ale_doublecmp(exp, res, eps),
			"FAIL: stats_hyper_f(2, 5, 5, 10) == %.20f != %.20f\n", res, exp);

  res = stats_hyper_f(4, 5, 5, 10);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_hyper_f(2, 5, 5, 10) == NaN\n");
  exp = 9.92063492063491314532086562394397333264350891113281e-02d;
  ERROR_UNDEF_FATAL_FMT(0 != ale_doublecmp(exp, res, eps),
			"FAIL: stats_hyper_f(4, 5, 5, 10) == %.20f != %.20f\n", res, exp);

  res = stats_hyper_f(9, 9, 9, 1000);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_hyper_f(9, 9, 9, 1000) == NaN\n");
  exp = 3.76220209419118293110987080887119801628657705095627e-22d;
  ERROR_UNDEF_FATAL_FMT(0.0d == res, "FAIL: stats_hyper_f(9, 9, 9, 1000) == %.20f != %.20f\n", res, exp);
  ERROR_UNDEF_FATAL_FMT(0 != ale_doublecmp(exp, res, eps),
			"FAIL: stats_hyper_f(9, 9, 9, 1000) == %.20e != %.20e\n", res, exp);

  res = 0;
  for (long k = 0 ; k < 11 ; k++)
    {
      res += stats_hyper_f(k, 5, 5, 10);
    }
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: sum(stats_hyper_f(k, 5, 5, 10)) == NaN\n");
  ERROR_UNDEF_FATAL_FMT(0 != ale_doublecmp(exp, res, eps), "FAIL: sum(stats_hyper_f(k, 5, 5, 10)) == %.20f != 1\n", res);

  return EXIT_SUCCESS;
}
