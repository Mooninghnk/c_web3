#include "web3.h"

struct memory curl_it(const char *json_dt, CURL *curl, char *key) {
  struct curl_slist *headers = NULL;

  struct memory chunk = {0};

  headers = curl_slist_append(headers, "Content-Type: application/json");

  curl_easy_setopt(curl, CURLOPT_URL, key);
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_dt);
  curl_easy_setopt(
      curl, CURLOPT_WRITEFUNCTION,
      cb); // Define a write function if you want to handle the response
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
  }
  curl_slist_free_all(headers);
  return chunk;
}

static size_t cb(char *data, size_t size, size_t nmemb, void *clientp) {
  size_t realsize = size * nmemb;
  struct memory *mem = (struct memory *)clientp;
  char *ptr = realloc(mem->response, mem->size + realsize + 1);

  if (!ptr) {
    return 0;
    // out of memeory error
  }

  mem->response = ptr;
  memcpy(&(mem->response[mem->size]), data, realsize);
  mem->size += realsize;
  mem->response[mem->size] = 0;

  return realsize;
}

