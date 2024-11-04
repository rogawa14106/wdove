#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uri.h>

// http://abehiroshi.la.coocan.jp:8080/top.html?id=10
static char *REG_SCHEME = "^(http|https)://.*$";
// abehiroshi.la.coocan.jp:8080/top.html?id=10
static char *REG_HOST = "^([^:/?]+?).*$";
// :8080/top.html?id=10
static char *REG_PORT = "^:([0-9]+).*$";
// /top.html?id=10
static char *REG_PATH = "^([^?]+).*$";
// ?id=10
// static char *REG_QUERY = "^(\\?=.*)$";
static char *REG_QUERY = "^(\\?.*)";

// urlを標準出力に表示する。
void print_uri(wd_uri_t *uri) {
  printf("  scheme : %s\n", uri->scheme);
  printf("  host   : %s\n", uri->host);
  printf("  port   : %s\n", uri->port);
  printf("  path   : %s\n", uri->path);
  printf("  query  : %s\n", uri->query);
  return;
}

// uri文字列をパースして、wd_uri_t構造体に格納する。
wd_uri_t *parse_uri(char *uri_str) {
  wd_uri_t *uri;
  int offset = 0;

  uri = malloc(sizeof(wd_uri_t));

  // schema部分をパース
  uri->scheme = search_scheme(uri_str, offset);
  offset += strlen(uri->scheme) + strlen("://");

  // host部分をパース
  uri->host = search_host(uri_str, offset);
  offset += strlen(uri->host);

  // port番号部分をパース
  uri->port = search_port(uri_str, offset);
  if (uri->port[0] == '\0') {
    uri->port = "80";
  } else {
    // uri_strにport番号が含まれている場合のみ、offsetを加算する。
    offset += strlen(":") + strlen(uri->port);
  }

  // path部分をパース
  uri->path = search_path(uri_str, offset);
  offset += strlen(uri->path);
  if (strlen(uri->path) == 1 && uri->path[1] == '/') {
    // uri_strにpathが含まれている場合のみ、offsetを加算する。
    offset += strlen(uri->path);
  }

  // query paramater部分をパース
  uri->query = search_query(uri_str, offset);
  offset += strlen(uri->query);
  if (uri->query[0] != '\0') {
    // uri_strにquery paramaterが含まれている場合のみ、offsetを加算する。
    offset += strlen(uri->query);
  }
  return uri;
};

// Search scheme from uri (required)
char *search_scheme(char *uri_str, int offset) {
  char *scheme;
  int limit = 2;
  char *matches[limit];
  int target_idx = 1;

  // 配列の2要素目にスキーマが入る想定なので、
  // 配列の長さが2未満の場合エラーとする。
  if (re_match(matches, uri_str + offset, REG_SCHEME) < target_idx + 1) {
    printf("error::search_scheme: Invalid uri. URI scheme not found.\n");
    exit(EXIT_FAILURE);
  }
  scheme = matches[target_idx];

  return scheme;
}

// Search host from uri (required)
char *search_host(char *uri_str, int offset) {
  char *host;
  int limit = 2;
  char *matches[limit];
  int target_idx = 1;

  // Null文字だったら、エラーとする。
  if (*(uri_str + offset) == '\0') {
    printf("error::search_host: Invalid uri. URI host not found.\n");
    exit(EXIT_FAILURE);
  }
  // 配列の2要素目にホストが入る想定なので、
  // 配列の長さが2未満の場合エラーとする。
  if (re_match(matches, uri_str + offset, REG_HOST) < target_idx + 1) {
    printf("error::search_host: Invalid uri. URI host not found.\n");
    exit(EXIT_FAILURE);
  }
  host = matches[target_idx];

  return host;
}

// Search port from uri
char *search_port(char *uri_str, int offset) {
  char *port;
  int limit = 2;
  char *matches[limit];
  int target_idx = 1;

  // 配列の2要素目にポート番号が入る想定なので、
  // 配列の長さが2未満の場合、空文字列を渡す
  if (re_match(matches, uri_str + offset, REG_PORT) < target_idx + 1) {
    printf("info::search_port: URI port number is not specified.\n");
    return "";
  }
  port = matches[target_idx];

  return port;
}

// Search path from uri
char *search_path(char *uri_str, int offset) {
  char *path;
  int limit = 2;
  char *matches[limit];
  int target_idx = 1;

  // 配列の2要素目にパスが入る想定なので、
  // 配列の長さが2未満の場合、空文字列を渡す
  if (re_match(matches, uri_str + offset, REG_PATH) < target_idx + 1) {
    printf("info::search_port: URI path is not specified.\n");
    return "/";
  }
  path = matches[target_idx];

  return path;
}

// Search query params from uri
char *search_query(char *uri_str, int offset) {
  char *query;
  int limit = 2;
  char *matches[limit];
  int target_idx = 1;

  // 配列の2要素目にクエリパラメータが入る想定なので、
  // 配列の長さが2未満の場合、空文字列を渡す
  if (re_match(matches, uri_str + offset, REG_QUERY) < target_idx + 1) {
    printf("info::search_query: URI query params is not specified.\n");
    return "";
  }
  query = matches[target_idx];

  return query;
}

// 正規表現にマッチした個数を返す。
// マッチした文字列の配列が、matchesに入る。
int re_match(char **matches, char *str, char *regex) {
  regex_t regexBuffer; // 正規表現オブジェクト

  // 正規表現オブジェクトのコンパイル
  if (regcomp(&regexBuffer, regex, REG_EXTENDED | REG_NEWLINE) != 0) {
    printf("error::re_match: Failed to compile regex obj. Invalid regex.\n");
    return -1;
  }

  // 実行。マッチしなかったら、0を返す。
  int limit = sizeof(matches);
  regmatch_t regmatch[limit];
  if (regexec(&regexBuffer, str, limit, regmatch, 0) != 0) {
    // printf("re_match: No match!\n");
    return 0;
  }

  // マッチした場合patternMatch構造体に文字列のindex番号が入る
  // 配列の数がマッチ数を超えていた場合超えた構造体の各要素には-1が入る
  int match_cnt = 0;
  for (int i = 0; i < limit; ++i) {
    int start_idx = regmatch[i].rm_so;
    int end_idx = regmatch[i].rm_eo;
    if (start_idx == -1 || end_idx == -1) {
      break;
    }
    // マッチした場合、マッチした文字列を配列に格納する。
    // end_idxが一つ長いので、lenはend_idx - start_idxになる。
    matches[i] = str_sub(str, start_idx, end_idx - start_idx);
    // printf("re_match:match[%d]: %s\n", i, matches[i]);
    match_cnt++;
  }

  // オブジェクトの開放
  regfree(&regexBuffer);
  return match_cnt;
}

// start文字目から、len個の文字列を取り出す。
char *str_sub(char *str, int start, int len) {
  // 切り出す文字列の長さが0以下の場合はエラーとする。
  if (len < 0) {
    printf("error::str_sub: value of len is invalid.(len: %d)\n", len);
    exit(EXIT_FAILURE);
  } else if (len == 0) {
    return "";
  }
  // 切り出す文字列がもとの文字列からはみ出す場合は、エラーとする。
  if (strlen(str) < (start + len - 1)) {
    printf("error::str_sub: index out of range."
           "(str_len: %ld, start: %d, len: %d)\n",
           strlen(str), start, len);
    exit(EXIT_FAILURE);
  };
  char *substr = calloc(len, sizeof(char));
  // memset(substr, 0, len);
  strncpy(substr, str + start, len);
  substr[len] = '\0';
  return substr;
}
