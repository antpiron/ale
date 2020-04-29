#ifndef __NORMALIZE_H
#define __NORMALIZE_H

#include <stddef.h>
#include <inttypes.h>


enum {
      NORMALIZE_EDIVZERO = 1,
      NORMALIZE_EVARZERO,
      NORMALIZE_EMEANZERO      
};

enum {
      STATS_WEIGHT_MEAN = 0,
      STATS_WEIGHT_VARIANCE = 1
};

int stats_normalize_samples_ls(size_t m, size_t n, size_t r,
			       const double mat[m][n], const size_t ref[r], double beta[n],
			       unsigned int options);
int stats_normalize_samples_poisson(size_t m, size_t n, size_t r,
				    const double mat[m][n], const size_t ref[r], double beta[n]);


void stats_normalize_samples(size_t m, size_t n, double dst[m][n],
		     const double src[m][n], const double beta[n]);

#endif
