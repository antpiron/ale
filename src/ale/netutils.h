#ifndef __NETUTILS_H

#include <stdint.h>
#include <ale/skiplist.h>
#include <ale/vector.h>

static inline int
cmp_uint64_t(uint64_t a, uint64_t b)
{
  if (a < b)
    return -1;
  else if (a == b)
    return 0;
  else
    return 1;
}

// uint64_t = (ip<<32) | netmask ; uint32_t = asn
SKL_INIT(ipv4_to_asn,18,uint64_t,uint32_t,cmp_uint64_t)
VECTOR_INIT_FULL(asn_to_owner,char*, (1ul << 16))

struct netutils
{
  uint32_t asn_len;
  struct vector_asn_to_owner asn_to_owner;
  struct skl_ipv4_to_asn ipv4_to_asn;
};

void netutils_init(struct netutils *nu);
void netutils_destroy(struct netutils *nu);

// http://thyme.apnic.net/current/data-raw-table format <ip>/<mask> <asn>
int netutils_load_ipv4_to_asn(struct netutils *nu, const char *filename);
uint32_t netutils_ipv4_to_asn(struct netutils *nu, const char *ip);

// http://thyme.apnic.net/current/data-used-autnums <asn>' '<owner>
int netutils_load_asn_to_owner(struct netutils *nu, const char *filename);
char* netutils_asn_to_owner(struct netutils *nu, uint32_t asn);


#endif
