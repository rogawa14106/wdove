#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <cmd.h>
#include <http.h>

int wd_set_default_args(wd_args_t *wd_args) {
  // set default http method
  if (wd_args->method == '\0') {
    wd_args->method = HTTP_GET;
  }
  return 0;
};

int wd_validate_args(wd_args_t *wd_args) {
  if (strlen(wd_args->uri_str) < 1) {
    printf("URI is required.");
  }
  if (wd_args->method == 0) {
    printf("Http method is required.");
  }
  return 0;
}

int parse_cmd_args(int argc, char **argv, wd_args_t *wd_args) {

  int i, opt;

  opterr = 0; // getopt()のエラーメッセージを無効にする。

  const char *short_opts = "X:";
  while ((opt = getopt(argc, argv, short_opts)) != -1) {
    // Repeat until there are no command line arguments options.
    switch (opt) {
    case 'X':
      // printf("Method = %s\n", optarg);
      wd_args->method = method_atoi(optarg);
      break;

    default: /* '?' */
      // If an unspecified option is passed.
      printf("Usage: %s [-X method] url_string\n", argv[0]);
      break;
    }
  }

  // Receive non-option arguments.
  for (i = optind; i < argc; i++) {
    // printf("args[%d] = %s\n", i, argv[i]);
    wd_args->uri_str = argv[i];
  }

  // set default values
  wd_set_default_args(wd_args);

  // validate args
  if (wd_validate_args(wd_args) < 0) {
    printf("invalid arguments.");
    exit(EXIT_FAILURE);
  };

  return 0;
}
