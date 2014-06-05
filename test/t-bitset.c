#include <stdlib.h>
#include <stdio.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <unistd.h>

#include "error.h"
#include "bitset.h"

#define NBRBIT (2043)

int
main(int argc, char *argv[argc])
{
  struct bitset bs;
  
  ERROR_FATAL(bitset_init(&bs,NBRBIT) < 0, "FAIL: Init failed\n");
  
  for (int i = 0 ; i < (NBRBIT + 63) / 64 ; i++)
    ERROR_FATAL_FMT(bs.buf[i], "FAIL: bitset_init() bs.buf[%d] = %"PRIx64" != 0\n", i, bs.buf[i]);

  bitset_set(bs, 57);
  ERROR_FATAL_FMT((1ull << 57) != bs.buf[0], "FAIL: bitset_set() %"PRIx64" != %llx\n", bs.buf[0], (1ull << 57));
  ERROR_FATAL(! bitset_isset(bs, 57), "FAIL: bitset_set() isset(57) != 1");

  bitset_unset(bs, 57);
  ERROR_FATAL(bitset_isset(bs, 57), "FAIL: bitset_unset() isset(57) != 0");

  bitset_setrange(bs, 8, 48);
  ERROR_FATAL_FMT(0x00FFFFFFFFFFFF00ull != bs.buf[0], "FAIL: bitset_setrange() %"PRIx64" != 0x00FFFFFFFFFFFF00", bs.buf[0]);
  for (int i = 1 ; i < (NBRBIT + 63) / 64 ; i++)
    ERROR_FATAL_FMT(bs.buf[i], "FAIL: bs.buf[%d] = %"PRIx64" != 0\n", i, bs.buf[i]);

  bitset_setrange(bs, 73,553);
  for (int i = 73 ; i < 73+553 ; i++)
    ERROR_FATAL_FMT(! bitset_isset(bs, i), "FAIL: setrange() isset(%d) != 1", i);

  bitset_reset(bs);
  for (int i = 0 ; i <  (NBRBIT + 63) / 64 ; i++)
    ERROR_FATAL_FMT(bs.buf[i], "FAIL:  bitset_reset() bs.buf[%d] = %"PRIx64" != 0\n", i, bs.buf[i]);
    
  ERROR_FATAL(0 == bitset_isempty(bs), "FAIL: bitset_isempty() do not say empty\n");
  bitset_set(bs, 1201);
  ERROR_FATAL(bitset_isempty(bs), "FAIL: bitset_isempty() say empty\n");

  bitset_free(bs);

  return EXIT_SUCCESS;
}
