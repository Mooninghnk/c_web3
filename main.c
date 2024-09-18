#include <cjson/cJSON.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CURL *init_curl();
void get_block_number(CURL *curl, char *key);
static size_t cb(char *data, size_t size, size_t nmemb, void *clientp);

int main(int argc, char *argv[]) {
  FILE *fptr;

  fptr = fopen("./key.txt", "r");
  if(fptr == NULL) {
    perror("error occured");
    return 1;
  }


 // fix opening the file and reading the key
//make it read how big of a buffer we need
 //fixed it make this work dumbass
 
  char key[62];

  fgets(key, 62, fptr);
  
  CURL *curl = init_curl();
  if (curl) {
    get_block_number(curl, key);
    curl_easy_cleanup(curl);
  }
  return 0;
}
//memory struct
struct memory {
  char *response;
  size_t size;
};

//callback function i found on libcurl docs
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

CURL *init_curl() {
  CURL *curl = curl_easy_init();
  if (!curl) {
    fprintf(stderr, "Failed to init curl\n");
    return NULL;
  }
  return curl;
}

void get_block_number(CURL *curl, char *key) {
  const char *json_data = "{\"jsonrpc\":\"2.0\",\"method\":\"eth_blockNumber\","
                          "\"params\":[],\"id\":1}";
  struct curl_slist *headers = NULL;
  struct memory chunk = {0};
  headers = curl_slist_append(headers, "Content-Type: application/json");

  curl_easy_setopt(curl, CURLOPT_URL, key);
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
  curl_easy_setopt(
      curl, CURLOPT_WRITEFUNCTION,
      cb); // Define a write function if you want to handle the response
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
  }

  printf("this %s", chunk.response); //capture the response 

  free(chunk.response);
  curl_slist_free_all(headers); // Cleanup
  
}

// example of a fetch method
