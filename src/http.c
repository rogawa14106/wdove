#include <arpa/inet.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <http.h>
#include <uri.h>

char *method_to_name(wd_http_method_t method) {
  char *method_name;
  u_int8_t is_valid = 0;
  for (int i = 0; i < sizeof(wd_http_method_name_t); i++) {
    if ((method_names[i].method & method) == 0x00000000) {
      method_name = method_names[i].name;
      is_valid = 1;
      break;
    }
  }

  if (is_valid == 1) {
    return method_name;
  } else {
    printf("error::method_to_name: Unknown http method.");
    exit(EXIT_FAILURE);
  }
}

char *version_to_name(wd_http_version_t method) {
  char *version_name;
  u_int8_t is_valid = 0;
  for (int i = 0; i < sizeof(wd_http_version_name_t); i++) {
    if ((version_names[i].version & method) == 0x00000000) {
      version_name = version_names[i].name;
      is_valid = 1;
      break;
    }
  }

  if (is_valid == 1) {
    return version_name;
  } else {
    printf("error::version_to_name: Unknown http version.");
    exit(EXIT_FAILURE);
  }
}

// create request header
wd_http_hdr_t *create_http_hdr(wd_uri_t *uri, wd_http_method_t method) {
  wd_http_hdr_t *http_hdr;
  switch (method) {
  case HTTP_GET: {
    http_hdr = create_http_hdr_get(uri);
    break;
  }
  // case HTTP_POST: {
  // http_hdr = create_http_hdr_post(uri);
  // break;
  // }
  default: {
    printf("error::create_http_hdr: Method not supported\n");
    exit(EXIT_FAILURE);
    break;
  }
  };
  return http_hdr;
}

// GET
wd_http_hdr_t *create_http_hdr_get(wd_uri_t *uri) {
  wd_http_hdr_t *http_hdr;
  http_hdr = malloc(sizeof(wd_http_hdr_t));
  http_hdr->method = method_to_name(HTTP_GET);
  http_hdr->path = uri->path;
  http_hdr->version = version_to_name(HTTP_VERSION_1_1);

  // http_hdr->fields = calloc(3, sizeof(wd_http_hdr_field_t));
  http_hdr->fields_len = 3;
  // TODO
  // これはデフォルトの設定とするとして、コマンドライン引数からヘッダをシテイできるようにもしたい
  // かなりハードコードなのもどうにかしたい。
  http_hdr->fields = calloc(http_hdr->fields_len, 8);
  http_hdr->fields[0].name = "Host"; //"Host: "
  http_hdr->fields[0].val = uri->host;
  http_hdr->fields[1].name = "User-Agent";
  http_hdr->fields[1].val = "httpc/0.0";
  http_hdr->fields[2].name = "Accept";
  http_hdr->fields[2].val = "*/*";
  return http_hdr;
}

// create request
u_char *create_http_req(u_char *http_req, wd_http_hdr_t *hdr, u_char *data) {
  // u_char *http_req;

  // create first line
  sprintf((char *)http_req, "%s %s %s\r\n", hdr->method, hdr->path,
          hdr->version);

  // create fields
  for (int i = 0; i < hdr->fields_len; i++) {
    u_char field[1024];
    sprintf((char *)field, "%s: %s\r\n", (char *)hdr->fields[i].name,
            (char *)hdr->fields[i].val);
    strcat((char *)http_req, (char *)field);
    printf("i:%d, %s", i, field);
  };

  strcat((char *)http_req, "\r\n");
  return http_req;
};

// int parse_req_params(char *req_param_str) {
// typedef struct {
// char *key;
// char *val;
// } Param;
//
// Param *req_params;
// return 0;
// }

int http_connect(struct in_addr address, in_port_t port) {
  int sock_fd;
  struct sockaddr_in sockaddr;
  size_t addr_len;
  // create tcp socket
  sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock_fd < 0) {
    printf("faild to create new tcp socket.\n");
    return -1;
  }

  // set addresses
  addr_len = sizeof(struct sockaddr);
  memset(&sockaddr, 0, addr_len);
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = (in_addr_t)address.s_addr;
  sockaddr.sin_port = htons(port);

  // connect server on tcp/80
  if (connect(sock_fd, (struct sockaddr *)&sockaddr, addr_len) < 0) {
    printf("falied to connect server.\n");
    return -1;
  } else {
    printf("Connection has been established with http server.\n");
  };
  return sock_fd;
}
