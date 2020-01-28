#ifndef __BITSET_H
#define __BITSET_H

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "error.h"

struct bitset
{
  size_t n;
  size_t alloc_size;
  uint64_t *buf;
};

static inline int
bitset_init(struct bitset *bs, size_t n)
{
  bs->n = n;
  bs->alloc_size = (n + 63) / 64;
  ERROR_ERRNO_RET( NULL == (bs->buf = calloc( bs->alloc_size, sizeof(uint64_t)) ), -1);
  
  return 0;
}

static inline int
bitset_resize(struct bitset *bs, size_t n)
{
  size_t realloc_size = (n + 63) / 64;
  
  if (realloc_size > bs->alloc_size)
    {
      void *ptr;
      realloc_size += bs->alloc_size;
      ptr = realloc(bs->buf, realloc_size * sizeof(uint64_t));
      ERROR_ERRNO_RET( NULL == ptr, -1);
      bs->buf = ptr;
      for (size_t i = bs->alloc_size ; i < realloc_size ; i++)
	bs->buf[i] = 0ull;
      bs->alloc_size = realloc_size;
    }

  bs->n = n;

  return 0;
}

static inline void
bitset_reset(struct bitset *bs)
{
  for (size_t i = 0 ; i < (bs->n + 63) / 64 ; i++)
    bs->buf[i] = 0ull;
}

static inline void
bitset_set(struct bitset *bs, size_t index)
{
  bitset_resize(bs, index+1);
  bs->buf[index / 64] |= 1ull << (index % 64);
}

static inline void
bitset_unset(struct bitset *bs, size_t index)
{
  bitset_resize(bs, index+1);
  bs->buf[index / 64] &= ~(1ull << (index % 64));
}

static inline int
bitset_isset(struct bitset *bs, size_t index)
{
  return ( bs->buf[index / 64] >> (index % 64) ) & 1ull;
}

static inline int
bitset_isempty(struct bitset *bs)
{
  int ret = 0;

  for (size_t i = 0 ; !ret && i < (bs->n + 63) / 64 ; i++)
    ret = ret || bs->buf[i];

  return !ret;
}

static inline void
bitset_xor(struct bitset *dst, struct bitset *a, struct bitset *b)
{
  size_t s = b->n;
  if (a->n < s)
    s = a->n;
  if (dst->n < s)
    s = dst->n;

  bitset_resize(dst, s);
  for (size_t i = 0 ; i < (s + 63) / 64 ; i++)
    dst->buf[i] = a->buf[i] ^ b->buf[i];
}

static inline void
bitset_and(struct bitset *dst, struct bitset *a, struct bitset *b)
{
  size_t s = b->n;
  if (a->n < s)
    s = a->n;
  if (dst->n < s)
    s = dst->n;

  bitset_resize(dst, s);
  for (size_t i = 0 ; i < (s + 63) / 64 ; i++)
    dst->buf[i] = a->buf[i] & b->buf[i];
}

static inline void
bitset_or(struct bitset *dst, struct bitset *a, struct bitset *b)
{
  size_t s = b->n;
  if (a->n < s)
    s = a->n;
  if (dst->n < s)
    s = dst->n;

  bitset_resize(dst, s);
  for (size_t i = 0 ; i < (s + 63) / 64 ; i++)
    dst->buf[i] = a->buf[i] | b->buf[i];
}

static inline size_t
bitset_ones(struct bitset *bs)
{
  // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetKernighan
  unsigned int v; // count the number of bits set in v
  size_t c = 0; // c accumulates the total bits set in v

  for (size_t i = 0 ; i < (bs->n + 63) / 64 ; i++)
    {
      uint64_t v = bs->buf[i];

#if defined HAVE_POPCOUNTLL && SIZEOF_LONG_LONG == 8
      c += __builtin_popcountll(v);
#else
      for (; v; c++)
	v &= v - 1; // clear the least significant bit set
#endif
     }
  
  return c;
}
  
static inline void
bitset_setrange(struct bitset *bs, size_t index, size_t len)
{
  size_t i1 = index / 64u;
  size_t m1 = index % 64u;
  size_t index2 = index + len - 1u;
  size_t i2 = index2 / 64u;
  size_t m2 = index2 % 64u;

  bitset_resize(bs, index + len);
  if (i1 == i2)
    bs->buf[i1] |= ~( (1ull << m1) - 1ull ) & (~0ull >> (63-m2) );
  else
    {
      bs->buf[i1] |= ~( (1ull << m1) - 1ull );
      for (size_t i = i1+1 ; i < i2 ; i++)
	bs->buf[i] = ~0ull;
      bs->buf[i2] |= ~0ull >> (63-m2);      
    }
}

static inline void
bitset_destroy(struct bitset *bs)
{
  free(bs->buf);
}

#endif
