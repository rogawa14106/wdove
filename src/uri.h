#ifndef _WD_URI_H
#define _WD_URI_H

#include <sys/types.h>

typedef struct { // http://wdove.com:8080/top.html?id=1&name=rogawa
  char *scheme;  // http
                 //   char *user;
                 //   char *password;
  char *host;    // wdove.com
  char *port;    // 8080(char)
  char *path;    // /top.html
  char *query;   // ?id=1&name=rogawa
} wd_uri_t;

// reference
// struct Curl_URL {
//   char *scheme;
//   char *user;
//   char *password;
//   char *options; /* IMAP only? */
//   char *host;
//   char *zoneid; /* for numerical IPv6 addresses */
//   char *port;
//   char *path;
//   char *query;
//   char *fragment;
//   unsigned short portnum; /* the numerical version (if 'port' is set) */
//   BIT(query_present);    /* to support blank */
//   BIT(fragment_present); /* to support blank */
//   BIT(guessed_scheme);   /* when a URL without scheme is parsed */
// };

void print_uri(wd_uri_t *uri);

wd_uri_t *parse_uri(char *uri_str);
char *search_scheme(char *uri_str, int offset);
char *search_host(char *uri_str, int offset);
char *search_port(char *uri_str, int offset);
char *search_path(char *uri_str, int offset);
char *search_query(char *uri_str, int offset);

int re_match(char **matches, char *str, char *regex);
char *str_sub(char *str, int start, int offset);

#endif /* ifndef _WD_URI_H */
// vim:ft=c :
