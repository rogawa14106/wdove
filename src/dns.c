#include <netdb.h>
#include <netinet/in.h>

struct in_addr resolve_host(char *hostname) {
  struct hostent *host_ent;
  char *addr_ipv4;

  // resolve hostname
  host_ent = gethostbyname(hostname);

  // return ipv4 address
  addr_ipv4 = host_ent->h_addr_list[0];
  return *(struct in_addr *)addr_ipv4;
}
