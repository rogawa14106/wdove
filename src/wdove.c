#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <dns.h>
#include <http.h>
#include <uri.h>

int main(int argc, char **argv) {
  printf("%s\n", "hello, wdove");
  int sock;

  // コマンドライン引数チェック
  if (argc < 2) {
    printf("error::wdove: argument is required.\n");
    exit(EXIT_FAILURE);
  }

  // Receive cmdline arguments
  printf("info::wdove: === Received Arguments ===================\n");
  char *uri_str = argv[1];
  printf("  URI string: %s\n", uri_str);
  printf("info::wdove: ==========================================\n");

  // Parse URI
  wd_uri_t *uri = parse_uri(uri_str);
  printf("info::wdove: === URI ==================================\n");
  print_uri(uri);
  printf("info::wdove: ==========================================\n");

  // Resolve name
  char *hostname = uri->host;
  struct in_addr hostaddr = resolve_host(hostname);

  // create http header // -X オプションでメソッドを分ける。
  wd_http_hdr_t *http_req_hdr = create_http_hdr(uri, HTTP_GET);
  u_char http_req_data[] = "{\"name\": \"rogawa\"}"; // TODO send json data
  u_char http_req[128];
  create_http_req(http_req, http_req_hdr, http_req_data);
  printf("info::wdove: === REQUEST ==============================\n");
  printf("host address: %s\n", inet_ntoa(hostaddr));
  printf("%s\n", http_req);
  printf("info::wdove: ==========================================\n");

  // printf("== Request Http Header (len:%d)\n%s", (int)strlen(http_hdr),
  // http_hdr);

  // connect to httpserver
  printf("port : %d\n", atoi(uri->port));
  if ((sock = http_connect(hostaddr, (in_port_t)atoi(uri->port))) < 0) {
    printf("error::wdove: Connection does not established. exit httpc.\n");
    return -1;
  }

  // send http request
  // send(int fd, const void *buf, size_t n, int flags)
  int send_size;
  send_size = send(sock, http_req, strlen((char *)http_req), 0);
  printf("send done. size: %d\n", send_size);
  if (send_size != strlen((char *)http_req)) {
    printf("exit httpc.\n");
    return -1;
  };

  // read http response
#define BUFF_LEN 1024

  char read_buf[BUFF_LEN];
  int total_bytes = 0;
  int byte_recv = 0;

  while (total_bytes < 1) {
    if ((byte_recv = recv(sock, read_buf, BUFF_LEN - 1, 0)) <= 0) {
      printf("exit httpc.\n");
      return -1;
    };
    printf("== Response Received: %d bytes:\n%s\n", byte_recv, read_buf);
    total_bytes += byte_recv;
  }

  // close socket
  close(sock);
  return 0;
}
