#include <stdio.h>
#include "lib/web3.h"

#define KEYSIZE 62

CURL *init_curl();
void get_block_number(CURL *curl, char *key);
static size_t cb(char *data, size_t size, size_t nmemb, void *clientp);

int main(int argc, char *argv[]) {
  FILE *fptr;

  fptr = fopen("./key.txt", "r");
  if (fptr == NULL) {
    perror("error occured");
    return 1;
  }
  // fix opening the file and reading the key
  // make it read how big of a buffer we need
  // fixed it make this work dumbass
  // char key[KEYSIZE+1] or set keysize with an extra filed !!
  char key[KEYSIZE]; // null terminator is required

  fgets(key, KEYSIZE, fptr);

  CURL *curl = init_curl();
  if (curl) {
    get_block_number(curl, key);
    curl_easy_cleanup(curl);
  }
  return 0;
}

CURL *init_curl() {
  CURL *curl = curl_easy_init();
  if (!curl) {
    fprintf(stderr, "Failed to init curl\n");
    return NULL;
  }
  return curl;
}


// return the current block
void get_block_number(CURL *curl, char *key) {
  const char *json_data = "{\"jsonrpc\":\"2.0\",\"method\":\"eth_blockNumber\","
                          "\"params\":[],\"id\":1}";
  struct memory chunk = curl_it(json_data, curl, key);

  cJSON *response = cJSON_Parse(chunk.response);
  if (response == NULL) {
    printf("err");
  } else {
    cJSON *result = cJSON_GetObjectItem(response, "result");
    if (cJSON_IsString(result) && (result->valuestring != NULL)) {
      printf("Current block %s\n", result->valuestring);
    }
    cJSON_Delete(response);
  }

  free(chunk.response);
}

int hex_to_dec(char *hexval) {
  int len = strlen(hexval);
  return 0;
}
