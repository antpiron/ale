#include <stdio.h>
#include <stdlib.h>

#include "ale/netutils.h"

void
netutils_init(struct netutils *nu)
{
  skl_ipv4_to_asn_init(&nu->ipv4_to_asn);
}

void
netutils_destroy(struct netutils *nu)
{
  skl_ipv4_to_asn_destroy(&nu->ipv4_to_asn);
}


#define SKIP_AFTER(c) do {			\
    line = strchr(line, (c));			\
    if (NULL == line)				\
      return -1;				\
    line++;					\
  } while (0)

int
parse_ip(const char *line, uint32_t *ip)
{
  // want local endianness (not big endian like inet_aton)
  *ip = ( (uint32_t) atoi(line) ) << 24;
  SKIP_AFTER('.');
  *ip |= ( (uint32_t) atoi(line) ) << 16;
  SKIP_AFTER('.');
  *ip |= ( (uint32_t) atoi(line) ) << 8;
  SKIP_AFTER('.');
  *ip |= (uint32_t) atoi(line);

  return 0;
}

static int
parse_line(const char *line, uint32_t *ip, uint32_t *netmask, uint32_t *asn)
{
  if ( -1 == parse_ip(line, ip) )
    return -1;

  SKIP_AFTER('/');   
  *netmask = ~ ( ( UINT32_C(1) << (32 - atoi(line)) ) - UINT32_C(1) );

  SKIP_AFTER('\t');
  *asn = atoi(line);
  
  return 0;
}

#define IP_NETMASK_TO64(ip,netmask) ( (((uint64_t) (ip)) << 32) | (uint64_t) (netmask) )

int
netutils_load_ipv4_to_asn(struct netutils *nu, const char *filename)
{
#define MAXLEN 256
  char line[MAXLEN];
  FILE *file = fopen(filename, "r");
  ERROR_ERRNO_RET(NULL == file, -1);

  while (1)
    {
      char* ret = fgets(line, MAXLEN, file);
      uint32_t ip, netmask, asn;

      if (NULL == ret)
	{
	  ERROR_FERROR_RET(ferror(file), -1);
	  break;
	}

      if (-1 == parse_line(line, &ip, &netmask, &asn))
	continue;

      skl_ipv4_to_asn_insert(&nu->ipv4_to_asn, IP_NETMASK_TO64(ip, netmask), asn, NULL);
    }
  
  ERROR_ERRNO_RET(EOF == fclose(file), -1);
  
  return 0;
}

uint32_t
netutils_ipv4_to_asn(struct netutils *nu, const char *ip_str)
{
  struct skl_ipv4_to_asn_node *node;
  uint32_t ip;
  
  if ( -1 == parse_ip(ip_str, &ip) )
    return 0;

  for (uint32_t netmask = ~ UINT32_C(0) ; 0 != netmask ; netmask <<= 1, ip &= netmask)
    {
      int ret = skl_ipv4_to_asn_search(&nu->ipv4_to_asn, IP_NETMASK_TO64(ip, netmask), &node);
      
      if ( ret )
	return node->value;
      else if (node != &nu->ipv4_to_asn.header)
      	{
      	  uint32_t found_ip = node->key >> 32;
      	  uint32_t found_netmask = node->key & 0xFFFFFFFF;
	  
      	  if ( found_ip == (ip & found_netmask) )
      	    return node->value;
      	  else
      	    if (found_netmask < netmask)
      	      netmask = found_netmask;
      	}
      else
      	return 0;
    }
  
  return 0;
}
