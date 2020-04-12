#ifndef __NORMALIZE_H
#define __NORMALIZE_H

#include <stddef.h>
#include <inttypes.h>

int stats_normalize_samples_ls(size_t m, size_t n, size_t r,
			       const double mat[m][n], size_t ref[r], double beta[n]);


#endif
