#define _GNU_SOURCE
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "ale/stats.h"
#include "ale/math.h"
#include "ale/error.h"

#define GENERIC_FUNC(SUFFIX,TYPE)					\
  TYPE									\
  stats_mean##SUFFIX(size_t n, const TYPE x[n])				\
  {									\
    TYPE s = 0;								\
									\
    for (size_t i = 0 ; i < n ; i++)					\
      s += x[i];							\
									\
    return s / n;							\
  }									\
  									\
  TYPE									\
  stats_var##SUFFIX(size_t n, const TYPE x[n])				\
  {									\
    TYPE m = stats_mean##SUFFIX(n, x);					\
    TYPE s = 0;								\
									\
    for (size_t i = 0 ; i < n ; i++)					\
      {									\
	TYPE term = x[i] - m;						\
	s += term * term;						\
      }									\
									\
    return s / (n - 1);							\
  }									\
									\
  TYPE									\
  stats_sd##SUFFIX(size_t n, const TYPE x[n])				\
  {									\
    return sqrt##SUFFIX(stats_var##SUFFIX(n, x));			\
  }									\
  									\
  TYPE									\
  stats_cov##SUFFIX(size_t n, const TYPE x[n], const TYPE y[n])		\
  {									\
    TYPE mx = stats_mean##SUFFIX(n, x);					\
    TYPE my = stats_mean##SUFFIX(n, y);					\
    TYPE s = 0;								\
    									\
    for (size_t i = 0 ; i < n ; i++)					\
      s += (x[i] - mx) * (y[i] - my);					\
									\
    return s / (n - 1);							\
  }									\
									\
  TYPE									\
  stats_diff_mean##SUFFIX(size_t n, const TYPE x[n], const TYPE y[n])	\
  {									\
    TYPE s = 0;								\
									\
    for (size_t i = 0 ; i < n ; i++)					\
      s += y[i] - x[i];							\
									\
    return s / n;							\
  }									\
									\
  TYPE									\
  stats_diff_var##SUFFIX(size_t n, const TYPE x[n], const TYPE y[n])	\
  {									\
    TYPE m = stats_diff_mean##SUFFIX(n, x, y);				\
    TYPE s = 0;								\
									\
    for (size_t i = 0 ; i < n ; i++)					\
      {									\
	TYPE term = y[i] - x[i] - m;					\
	s += term * term;						\
      }									\
									\
    return s / (n - 1);							\
  }									\
									\
  TYPE									\
  stats_diff_sd##SUFFIX(size_t n, const TYPE x[n], const TYPE y[n])	\
  {									\
    return sqrt##SUFFIX(stats_diff_var##SUFFIX(n, x, y));		\
  }									\
									\
  int									\
  stats_pearson_corr##SUFFIX(size_t n, const TYPE x[n], const TYPE y[n], \
		     TYPE *rho, TYPE *pvalue)				\
  {									\
    TYPE mx = stats_mean##SUFFIX(n, x);					\
    TYPE my = stats_mean##SUFFIX(n, y);					\
    TYPE cov = 0, sx = 0, sy = 0;					\
									\
    for (size_t i = 0 ; i < n ; i++)					\
      {									\
	TYPE xterm = x[i] - mx;						\
	TYPE yterm = y[i] - my;						\
	cov += xterm * yterm;						\
	sx += xterm * xterm;						\
	sy += yterm * yterm;						\
      }									\
    if ( sx <= 0 ||  sy <= 0)						\
      return -1;							\
    cov /= n - 1;							\
    sx = sqrt##SUFFIX(sx / (n - 1));					\
    sy = sqrt##SUFFIX(sy / (n - 1));					\
									\
    /* Shieh, G. Behavior Research Methods (2010) 42: 906. https://doi.org/10.3758/BRM.42.4.906 */ \
    TYPE r = cov / (sx * sy);						\
    TYPE rs = 1-r*r;							\
    TYPE rho2 = r * (1 + rs / (2 * (n - 2)) + 9 * rs * rs / (8 * n * (n - 2))); \
									\
    if (rho)								\
      *rho = rho2;							\
    if (pvalue)								\
      {									\
	if (1.0 == rho2)						\
	  *pvalue = 0;							\
	else								\
	  {								\
	    TYPE t = rho2 * sqrt##SUFFIX(n-2) / sqrt##SUFFIX(1 - rho2*rho2); \
	    *pvalue = 2 * (1 - stats_student_F##SUFFIX(fabs##SUFFIX(t), n-2)); \
	  }								\
      }									\
									\
    return 0;								\
  }									\
									\
  void									\
  stats_mat_cov##SUFFIX(size_t m, size_t n, TYPE cov[m][m], const TYPE x[m][n]) \
  {									\
    TYPE *means = malloc(sizeof(TYPE) * m);				\
									\
    for (size_t i = 0 ; i < m ; i++)					\
      means[i] = stats_mean##SUFFIX(n, x[i]);				\
									\
    for (size_t i = 0 ; i < m ; i++)					\
      for (size_t j = i ; j < m ; j++)					\
	{								\
	  TYPE s = 0;							\
	  for (size_t k = 0 ; k < n ; k++)				\
	    s += (x[i][k] - means[i] ) * (x[j][k] - means[j]);		\
	  cov[j][i] = cov[i][j] = s / (n - 1);				\
	}								\
    free(means);							\
  }									\
									\
  TYPE									\
  stats_rss##SUFFIX(size_t m, size_t n, const TYPE y[m], const TYPE x[m][n], \
	    TYPE (*predict)(const TYPE x[n], void *cls), void *cls)	\
  {									\
    TYPE rss = 0;							\
    for (size_t i = 0 ; i < m ; i++)					\
      {									\
	TYPE term = y[i] - predict(x[i], cls);				\
	rss += term*term;						\
      }									\
									\
    return rss;								\
  }									\
									\
  int									\
  stats_rsquared##SUFFIX(size_t m, size_t n, const TYPE y[m], const TYPE x[m][n], \
		 TYPE (*predict)(const TYPE x[n], void *cls), void *cls, TYPE *rsquared) \
  {									\
    TYPE rss = 0, ssy = 0;						\
    TYPE my = stats_mean##SUFFIX(m, y);					\
    for (size_t i = 0 ; i < m ; i++)					\
      {									\
	TYPE term = y[i] - predict(x[i], cls);			\
	TYPE ssy_term = my - y[i];					\
	rss += term*term;						\
	ssy += ssy_term*ssy_term;					\
      }									\
									\
    if (0.0 == ssy)							\
      return -1;							\
									\
    /* https://en.wikipedia.org/wiki/Residual_sum_of_squares#Relation_with_Pearson's_product-moment_correlation */ \
    *rsquared = 1 - rss/ssy;						\
									\
    return 0;								\
  }									\
									\
  static int								\
  indirect_compar_double##SUFFIX(const void *p1, const void *p2, void *arg) \
  {									\
    const TYPE *d = arg;						\
    const size_t *a = p1;						\
    const size_t *b = p2;						\
									\
    if (d[*a] < d[*b])							\
      return -1;							\
    else if (d[*a] == d[*b])						\
      return 0;								\
									\
    return 1;								\
  }									\
									\
  /* http://www.biostathandbook.com/multiplecomparisons.html */		\
  int									\
  stats_p_adjust_fwer_bonf##SUFFIX(size_t n,  const TYPE p[n], TYPE padj[n]) \
  {									\
    for (size_t i = 0 ; i < n ; i++)					\
      {									\
	TYPE correction = p[i] * (TYPE ) n;				\
	padj[i] = (correction > 1.0d)? 1.0d : correction;		\
      }									\
									\
    return 0;								\
  }									\
									\
  /* http://www.biostathandbook.com/multiplecomparisons.html */		\
  /* R p.adjust: */							\
  /*   }, BH = { */							\
  /*        i <- lp:1L */						\
  /*        o <- order(p, decreasing = TRUE) */				\
  /*        ro <- order(o) */						\
  /*        pmin(1, cummin(n/i * p[o]))[ro] } */			\
  int									\
  stats_p_adjust_fdr_bh##SUFFIX(size_t n,  const TYPE p[n], TYPE padj[n]) \
  {									\
    size_t *index = malloc(sizeof(size_t) * n);				\
    TYPE min_val = 1.0d;						\
    TYPE last_val = -1;						\
    size_t last_pos = n-1;						\
									\
    ERROR_ERRNO_RET(NULL == index, -1);					\
									\
    for (size_t i = 0 ; i < n ; i++)					\
      index[i] = i;							\
									\
    qsort_r(index, n, sizeof(index[0]),					\
	    indirect_compar_double##SUFFIX, (void*) p);			\
									\
    for (ssize_t i = n-1 ; i >= 0  ; i--)				\
      {									\
	TYPE current_val = p[index[i]];				\
									\
	/* in case of equality (0.5 0.1 0.1) ranking is pessimistic (3 2 2) */ \
	if (last_val != current_val)					\
	  {								\
	    last_val = current_val;					\
	    last_pos = i;						\
	  }								\
									\
	TYPE correction =  current_val * (TYPE ) n / (TYPE) ( last_pos + 1 ); \
	min_val = padj[index[i]] = (correction > min_val)? min_val : correction; \
      }									\
									\
    free(index);							\
    return 0;								\
  }

GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)


void
stats_shuffle(void *vec, size_t nmemb, size_t size)
{
    char (*v)[size] = vec;
    char *tmp = malloc(size);
    for (size_t i = 0 ; i < nmemb ; i++)
      {
	size_t index = (size_t) stats_unif_rand(0, nmemb);
	memcpy(tmp, v+i, size);	
	memcpy(v+i, v+index, size);
	memcpy(v+index, tmp, size);
      }
    free(tmp);
}