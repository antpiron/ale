#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "ale/stats.h"
#include "ale/math.h"

#define GENERIC_FUNC(SUFFIX,TYPE)					\
  /* https://en.wikipedia.org/wiki/Beta_distribution#Generating_beta-distributed_random_variates */ \
  TYPE									\
  stats_beta_rand##SUFFIX(TYPE alpha, TYPE beta)			\
  {									\
    TYPE x = stats_gamma_rand##SUFFIX(alpha, 1);			\
    TYPE y = stats_gamma_rand##SUFFIX(beta, 1);				\
    									\
    return x / (x + y) ;						\
    }									\
  									\
  TYPE									\
  stats_beta_F##SUFFIX(TYPE x, TYPE alpha, TYPE beta)			\
  {									\
    return ale_ibeta##SUFFIX(x, alpha, beta);				\
  }									\
  									\
  void									\
  stats_beta_fit_mm##SUFFIX(size_t n, const TYPE x[n], TYPE *alpha, TYPE *beta) \
  {									\
    TYPE m = stats_mean##SUFFIX(n, x);					\
    TYPE v = stats_var##SUFFIX(n, x);					\
    TYPE t1_m = 1-m;							\
    TYPE tfrac = m * t1_m / v - 1;					\
									\
    *alpha = m*tfrac;							\
    *beta =  t1_m *tfrac;						\
  }									\
									\
  void									\
  stats_beta_fit##SUFFIX(size_t n, const TYPE x[n], TYPE *alpha, TYPE *beta) \
  {									\
    stats_beta_fit_mm##SUFFIX(n, x, alpha, beta);			\
  }

GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)

/* struct stats_em */
/* { */
/*   size_t n; */
/*   double *x; */
/*   unsigned int *z; */
/*   int ncat; */
/* }; */

/* struct stats_beta_par */
/* { */
/*   double alpha; */
/*   double beta; */
/* }; */

/* void */
/* stats_beta_expectation(struct stats_em *em, void *cls) */
/* { */
/*   struct stats_beta_par *par = cls; */
/* } */
    
/* void */
/* stats_beta_maximisation(struct stats_em *em, void *cls) */
/* { */
/*    struct stats_beta_par *par = cls; */

/*    for (size_t i = 0 ; i <  em->n ; i++) */
/*      { */
/*        double minp = 1; */
/*        int minc = -1; */
       
/*        for (int c = 0 ; c <  em->ncat ; c++) */
/* 	 { */
/* 	   // P[Z = c | alpha,beta ] =   */
/* 	   double p = 2 * fabs(0.5 - stats_beta_F(em->x[i], par[c].alpha, par[c].beta)); */
/* 	   if (p < minp) */
/* 	     { */
/* 	       minp = p; */
/* 	       minc = c; */
/* 	     } */
/* 	 } */
/*        em->z[i] = minc; */
/*      } */
/* } */


/* // EM explanation https://www.youtube.com/watch?v=qMTuMa86NzU */
/* int */
/* stats_em(struct stats_em *em, */
/* 	 void expectation(struct stats_em *em, void *cls), */
/* 	 void maximisation(struct stats_em *em, void *cls), */
/* 	 void *cls) */
/* { */
/*   const size_t max_iter = em->n; */
/*   unsigned int oldz[em->n]; */
/*   int equal = 1; */
/*   unsigned int a; */

/*   for (size_t i = 0 ; i <  em->n ; i++) */
/*     { */
/*       ERROR_ERRNO_MSG(-1 == portability_getrandom(&a, sizeof(a), 0), */
/* 		      "stats_unif_rand_std(): failed"); */
/*       em->z[i] = a % em->ncat; */
/*     } */

/*   for (size_t iter = 0 ; iter < max_iter && equal ; iter++) */
/*     { */
/*       expectation(em, cls); */
/*       memcpy(oldz, em->z, em->n * sizeof(int)); */
/*       maximisation(em, cls); */

/*       equal = 1; */
/*       for (size_t i = 0 ; i < em->n && equal; i++) */
/* 	equal = (oldz[i] == em->z[i]); */
/*     } */
/*   while (! equal); */

/*   return 1; */
/* } */

