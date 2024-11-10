#ifndef _WD_HTTP_H
#define _WD_HTTP_H

#include <netinet/in.h>
#include <stdint.h>
#include <sys/types.h>

#include "uri.h"

// define http method
// - reference
//   https://tex2e.github.io/rfc-translater/html/rfc9110.html
//   #9-3-1--GET
typedef enum {
  HTTP_UNKNOWN = (1 << 0),    // 0x00000001
  HTTP_GET = (1 << 1),        // 0x00000002
  HTTP_HEAD = (1 << 2),       // 0x00000004
  HTTP_POST = (1 << 3),       // 0x00000008
  HTTP_PUT = (1 << 4),        // 0x00000010
  HTTP_DELETE = (1 << 5),     // 0x00000020
  HTTP_MKCOL = (1 << 6),      // 0x00000040
  HTTP_COPY = (1 << 7),       // 0x00000080
  HTTP_MOVE = (1 << 8),       // 0x00000100
  HTTP_OPTIONS = (1 << 9),    // 0x00000200
  HTTP_PROPFIND = (1 << 10),  // 0x00000400
  HTTP_PROPPATCH = (1 << 11), // 0x00000800
  HTTP_LOCK = (1 << 12),      // 0x00001000
  HTTP_UNLOCK = (1 << 13),    // 0x00002000
  HTTP_PATCH = (1 << 14),     // 0x00004000
  HTTP_TRACE = (1 << 15),     // 0x00008000
  HTTP_CONNECT = (1 << 16),   // 0x00010000
} wd_http_method_t;
// #define HTTP_UNKNOWN (1 << 0)    // 0x00000001
// ...
// #define HTTP_CONNECT (1 << 16)   // 0x00010000

typedef struct {
  char *name;
  uint32_t method;
} wd_http_method_name_t;

static wd_http_method_name_t method_names[] = {
    {(char *)"GET", (uint32_t)~HTTP_GET},
    {(char *)"HEAD", (uint32_t)~HTTP_HEAD},
    {(char *)"POST", (uint32_t)~HTTP_POST},
    {(char *)"PUT", (uint32_t)~HTTP_PUT},
    {(char *)"DELETE", (uint32_t)~HTTP_DELETE},
    {(char *)"MKCOL", (uint32_t)~HTTP_MKCOL},
    {(char *)"COPY", (uint32_t)~HTTP_COPY},
    {(char *)"MOVE", (uint32_t)~HTTP_MOVE},
    {(char *)"OPTIONS", (uint32_t)~HTTP_OPTIONS},
    {(char *)"PROPFIND", (uint32_t)~HTTP_PROPFIND},
    {(char *)"PROPPATCH", (uint32_t)~HTTP_PROPPATCH},
    {(char *)"LOCK", (uint32_t)~HTTP_LOCK},
    {(char *)"UNLOCK", (uint32_t)~HTTP_UNLOCK},
    {(char *)"PATCH", (uint32_t)~HTTP_PATCH},
    //{NULL, 0 } };
};

// version
typedef enum {
  HTTP_VERSION_0_9 = (1 << 0),
  HTTP_VERSION_1_0 = (1 << 1),
  HTTP_VERSION_1_1 = (1 << 2),
  HTTP_VERSION_2_0 = (1 << 3),
  HTTP_VERSION_3_0 = (1 << 4),
} wd_http_version_t;

typedef struct {
  char *name;
  uint32_t version;
} wd_http_version_name_t;

static wd_http_version_name_t version_names[] = {
    {(char *)"HTTP/0.9", (uint32_t)~HTTP_VERSION_0_9},
    {(char *)"HTTP/1.0", (uint32_t)~HTTP_VERSION_1_0},
    {(char *)"HTTP/1.1", (uint32_t)~HTTP_VERSION_1_1},
    {(char *)"HTTP/2.0", (uint32_t)~HTTP_VERSION_2_0},
    {(char *)"HTTP/3.0", (uint32_t)~HTTP_VERSION_3_0},
    //{NULL, 0 } };
};

// http-URI = "http" "://" authority path-abempty [ "?" query ]
//  http header field
typedef struct {
  char *name;
  char *val;
} wd_http_hdr_field_t;

typedef struct {
  char *path;
  char *query;
} wd_http_hdr_uri_t;

// http header
typedef struct {
  char *method; // GET
                // char *path;                  // /top.html
  wd_http_hdr_uri_t uri;
  char *version;               // HTTP/1.1
  int fields_len;              // HOST: xxx.com
  wd_http_hdr_field_t *fields; // HOST: xxx.com
  // struct http_trailer_field *fields; TODO
} wd_http_hdr_t;

// struct dynhds_entry {
//   char *name;
//   char *value;
//   size_t namelen;
//   size_t valuelen;
// };

// struct dynhds {
//   struct dynhds_entry **hds;
//   size_t hds_len;       /* number of entries in hds */
//   size_t hds_allc;      /* size of hds allocation */
//   size_t max_entries;   /* size limit number of entries */
//   size_t strs_len;      /* length of all strings */
//   size_t max_strs_size; /* max length of all strings */
//   int opts;
// };
// struct wd_httpreq {
//   struct wd_http_hdr hdr;
//   u_char *data;
//
//   // char method[24];
//   // char *scheme;
//   // char *authority;
//   // char *path;
//   // struct dynhds headers;
//   // struct dynhds trailers;
// };

wd_http_hdr_t *create_http_hdr(wd_uri_t *uri, wd_http_method_t method);
wd_http_hdr_t *create_http_hdr_get(wd_uri_t *uri);
u_char *create_http_req(u_char *http_req, wd_http_hdr_t *hdr, u_char *data);
// int parse_req_params(char *req_param_str);

// convert method flg to method name
char *method_itoa(wd_http_method_t method);
wd_http_method_t method_atoi(char *method_name);
char *version_itoa(wd_http_version_t method);
int http_connect(struct in_addr, in_port_t port);
#endif //_WD_HTTP_H
// vim:ft=c :
