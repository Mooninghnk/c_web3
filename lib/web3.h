#ifndef WEB3_H
#define WEB3_H

#include <cjson/cJSON.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

struct memory {
  char *response;
  size_t size;
};

struct memory curl_it(const char *json_dt, CURL *curl, char *key);
static size_t cb(char *data, size_t size, size_t nmemb, void *clientp);

#endif
