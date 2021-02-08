#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "ale/stats.h"
#include "ale/math.h"
#include "ale/error.h"
#include "ale/portability.h"
#include "ale/algebra.h"

/* 
   An Improved Evaluation of Kolmogorov’s Distribution. Luis Carvalho 
   https://www.jstatsoft.org/article/view/v065c03/v65c03.pdf
*/
#define GENERIC_FUNC(SUFFIX,TYPE)					\
  TYPE									\
  stats_kolmo_F##SUFFIX(TYPE d, unsigned long n)			\
  {									\
    TYPE nd = n*d;							\
    TYPE k_dbl = ceil##SUFFIX(nd);					\
    size_t k = k_dbl;							\
    TYPE h = k_dbl - nd;						\
    size_t m = 2*k - 1;							\
    TYPE *v = malloc(sizeof(TYPE) * m);					\
    TYPE *w = malloc(sizeof(TYPE) * (m - 2));				\
    TYPE *q = malloc(sizeof(TYPE) * m);					\
    TYPE qk = -1;							\
									\
    if ( d <= 0.0##SUFFIX )						\
      return 0.0##SUFFIX;						\
    if ( d >= 1.0##SUFFIX )						\
      return 1.0##SUFFIX;						\
									\
    for (size_t i = 0 ; i < m ; i++)					\
      q[i] = 0;								\
    q[k - 1] = 1.0;							\
									\
    TYPE hj = 1, lsj = 0;						\
    for (size_t j = 0 ; j < m - 1 ; j++)				\
      {									\
	hj *= h;							\
	lsj += log##SUFFIX(j+1);					\
	v[j] = exp##SUFFIX( log1p##SUFFIX(- hj) - lsj );		\
	if (j < m - 2)							\
	  w[j] = exp##SUFFIX(-lsj);					\
      }									\
    hj *= h;								\
    lsj += log##SUFFIX(m);						\
    TYPE sub = (h > 0.5) ? pow##SUFFIX(2 * h - 1, m) : 0;		\
    v[m - 1] = exp##SUFFIX( log##SUFFIX(1 - 2 * hj + sub) - lsj );	\
    									\
    for (size_t i = 1 ; i <= n ; i++)					\
      {									\
	TYPE u = q[0];							\
	TYPE s = ( (TYPE) i ) / (TYPE) n;				\
									\
	q[0] = alg_dot##SUFFIX(m, v, q) * s;				\
									\
	for (size_t j = 2 ; j < m ; j++)				\
	  {								\
	    TYPE a = u;							\
	    u = q[j - 1];						\
	    a += v[m - j] * q[m - 1] + alg_dot##SUFFIX(m - j, w, q + j - 1); \
	    q[j - 1] = a * s;						\
	  }								\
	q[m - 1] = (v[0] * q[m - 1] + u) * s;				\
      }									\
    qk = q[k - 1];							\
									\
    free(v);								\
    free(w);								\
    free(q);								\
									\
    return(qk);								\
  }

GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)
