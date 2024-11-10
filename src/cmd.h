#ifndef _WD_CMD_H
#define _WD_CMD_H

#include <http.h>

typedef struct {
  // char *method;
  wd_http_method_t method;
  char *uri_str;
} wd_args_t;

int parse_cmd_args(int argc, char **argv, wd_args_t *wd_args);

#endif /* ifndef _WD_CMD_H */
