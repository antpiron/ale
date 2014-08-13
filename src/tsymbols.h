#ifndef __TSYMB_H
#define __TSYMB_H

#include <stdlib.h>

struct tsymbols {
  struct tsymbols *dad;
  size_t num, num_allocated;
  struct {
    char *name;
    void *data;
  } *symbol;
};

int ts_init(struct tsymbols *ts, struct tsymbols *dad);
void ts_destroy(struct tsymbols *ts);

 
void* ts_search(struct tsymbols *ts, char *key);
int ts_insert(struct tsymbols *ts, char *key, void *data);


#endif
