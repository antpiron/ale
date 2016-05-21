#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "ale/error.h"
#include "ale/netutils.h"

#define IPV4TOASN "ipv4toasn.txt"

int
main(int argc, char *argv[argc])
{
  struct netutils nu;
  uint32_t asn;
  
  netutils_init(&nu);

  ERROR_FATAL(-1 == netutils_load_ipv4_to_asn(&nu, IPV4TOASN), "FATAL: netutils_load_ipv4_to_asn()\n");
  
  ERROR_FATAL_FMT(23969 != (asn = netutils_ipv4_to_asn(&nu, "1.0.128.0")),
		  "FATAL: netutils_ipv4_to_asn(1.0.128.0) asn = %"PRIu32" != 23969\n", asn);
  ERROR_FATAL_FMT(23969 != (asn = netutils_ipv4_to_asn(&nu, "1.0.128.255")),
		  "FATAL: netutils_ipv4_to_asn(1.0.128.255) asn = %"PRIu32" != 23969\n", asn);
  ERROR_FATAL_FMT(9737 != (asn = netutils_ipv4_to_asn(&nu, "1.0.138.255")),
		  "FATAL: netutils_ipv4_to_asn(1.0.138.255) asn = %"PRIu32" != 9737\n", asn);
  ERROR_FATAL_FMT(71 != (asn = netutils_ipv4_to_asn(&nu, "15.224.193.14")),
		  "FATAL: netutils_ipv4_to_asn(115.224.193.14) asn = %"PRIu32" != 71\n", asn);

 
  netutils_destroy(&nu);

  return EXIT_SUCCESS;
}
