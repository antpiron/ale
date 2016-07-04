#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#include "ale/error.h"
#include "ale/netutils.h"

#define DBPATH "/tmp/ips/"

#define BGP4 DBPATH"bgp4.txt"
#define ASNTOOWNER DBPATH"asntoowner.txt"
#define RIR_AFRINIC DBPATH"afrinic.txt"
#define RIR_APNIC DBPATH"apnic.txt"
#define RIR_ARIN DBPATH"arin.txt"
#define RIR_LACNIC DBPATH"lacnic.txt"
#define RIR_RIPENCC DBPATH"ripencc.txt"

#define MAXLINE 256
int
main(int argc, char *argv[argc])
{
  struct netutils nu;
  char *ip;
  size_t len = 0;
  
  netutils_init(&nu);

  ERROR_FATAL(-1 == netutils_load_bgp4(&nu, BGP4), "FATAL: netutils_load_ipv4_to_asn()\n");
  ERROR_FATAL(-1 == netutils_load_asn_to_owner(&nu, ASNTOOWNER), "FATAL: netutils_load_asn_to_owner()\n");
  ERROR_FATAL(-1 == netutils_load_rir(&nu, RIR_AFRINIC), "FATAL: netutils_load_rir()\n");
  ERROR_FATAL(-1 == netutils_load_rir(&nu, RIR_APNIC), "FATAL: netutils_load_rir()\n");
  ERROR_FATAL(-1 == netutils_load_rir(&nu, RIR_ARIN), "FATAL: netutils_load_rir()\n");
  ERROR_FATAL(-1 == netutils_load_rir(&nu, RIR_LACNIC), "FATAL: netutils_load_rir()\n");
  ERROR_FATAL(-1 == netutils_load_rir(&nu, RIR_RIPENCC), "FATAL: netutils_load_rir()\n");

  printf("IP,ASN,COUNTRY,OWNER\n");
  while (getline(&ip, &len, stdin) != -1)
    {
      uint32_t asn;
      char *owner = "";
      char *country = "";
      struct rir *rir;
      char *str;
      
      for (str = ip ; *str && ( isdigit(*str) ||  *str == '.') ; str++) ;
      *str = '\0';
      
      asn = netutils_bgp4_ip_to_asn(&nu, ip);
      
      if (asn > 0)
	owner = netutils_asn_to_owner(&nu, asn);

      netutils_rir_getinfo(&nu, ip, &rir)?rir->country:"";
 
      if ( NULL == owner )
	    owner = "";

      if ( NULL != rir->country)
	    country = rir->country;

      country = netutils_rir_getinfo(&nu, ip, &rir)?rir->country:"";
      printf("%s,%"PRIu32",%s,\"%s\"\n", ip, asn, country, owner);
    }

  free(ip);
  netutils_destroy(&nu);

  return EXIT_SUCCESS;
}
