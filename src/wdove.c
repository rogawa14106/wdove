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

#include <cmd.h>
#include <dns.h>
#include <http.h>
#include <uri.h>

int main(int argc, char **argv) {
  printf("%s\n", "hello, wdove");
  int sock;

  // Validate the number of command line arguments
  if (argc < 2) {
    printf("error::wdove: argument is required.\n");
    exit(EXIT_FAILURE);
  }

  // Parse command line arguments
  wd_args_t *wd_args;
  wd_args = malloc(sizeof(wd_args_t));
  parse_cmd_args(argc, argv, wd_args);

  // Receive command line arguments
  char *uri_str = wd_args->uri_str;
  printf("info::wdove: === Received Arguments ===================\n");
  printf("URI String : %s\n", uri_str);
  printf("HTTP Method: %s\n", method_itoa(wd_args->method));
  printf("info::wdove: === Received Arguments ===================\n\n");

  // Parse URI
  wd_uri_t *uri = parse_uri(uri_str);
  // printf("info::wdove: === URI ==================================\n");
  // print_uri(uri);
  // printf("info::wdove: ==========================================\n");

  // Resolve name
  char *hostname = uri->host;
  struct in_addr hostaddr = resolve_host(hostname);

  // create http header
  wd_http_hdr_t *http_req_hdr = create_http_hdr(uri, wd_args->method);

  // create http reqest
  u_char http_req_data[] = "{\"name\": \"rogawa\"}"; // TODO send json data
  u_char http_req[1024];
  create_http_req(http_req, http_req_hdr, http_req_data);
  printf("info::wdove: === REQUEST ==============================\n");
  printf("host address: %s\n", inet_ntoa(hostaddr));
  printf("tcp portnum : %d\n", atoi(uri->port));
  printf("http header :\n%s", http_req);
  printf("info::wdove: === REQUEST ==============================\n\n");

  // connect to httpserver
  if ((sock = http_connect(hostaddr, (in_port_t)atoi(uri->port))) < 0) {
    printf("error::wdove: Connection does not established. exit.\n");
    exit(EXIT_FAILURE);
  }

  // send http request
  // send(int fd, const void *buf, size_t n, int flags)
  int send_size;
  send_size = send(sock, http_req, strlen((char *)http_req), 0);
  printf("send done. size: %d\n", send_size);
  if (send_size != strlen((char *)http_req)) {
    printf("faled to send request.\n");
    exit(EXIT_FAILURE);
  };

  // read http response
  int max_rcv_len = 65535;
  char read_buf[256];
  int byte_recv_tot = 0;
  int byte_recv = 0;

  // while (byte_recv_tot < sizeof(read_buf) - 1) {
  while (byte_recv_tot < max_rcv_len) {
    // 0 clear
    memset(read_buf, 0, sizeof(read_buf));

    // read
    byte_recv = recv(sock, read_buf, sizeof(read_buf) - 1, 0);

    if (byte_recv <= 0) {
      printf(" All responses have been received. Or, failed to receive "
             "response.\n");
      exit(EXIT_FAILURE);
    };

    printf("info::wdove: === RESPONSE size:%d, total: %d "
           "=============================\n%s\n",
           byte_recv, byte_recv_tot, read_buf);

    byte_recv_tot += byte_recv;

    if (byte_recv < sizeof(read_buf) - 1) {
      printf(" All responses have been received.\n");
      break;
    }
  }
  printf("info::wdove: === RESPONSE =============================\n\n");

  // close socket
  close(sock);
  return 0;
}
