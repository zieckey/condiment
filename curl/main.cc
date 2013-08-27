#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>

static size_t writeFunc( void* ptr, size_t size, size_t nmemb, void *usrptr )
{
    char buf[4096*32] = {0};
    memcpy(buf, ptr, size* nmemb);
    printf("%s", buf);
    return nmemb * size;
}


int main(int argc, char* argv[])
{
    long connection_timeout_ms = 10;
    const char* url = "http://10.125.209.252/status.html";

    if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        printf("usage : %s <connection_timeout_ms> <url>\n", argv[0]);
        return 0;
    }

    if (argc > 1) {
        connection_timeout_ms = atoi(argv[1]);
    }

    if (argc > 2) {
        url = argv[2];
    }

    CURL *curl = NULL;
    CURLcode res = CURLE_OK;
    curl = curl_easy_init();
    curl_easy_setopt( curl, CURLOPT_URL, url );
    curl_easy_setopt( curl, CURLOPT_CONNECTTIMEOUT_MS, (long)connection_timeout_ms );
    curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, &writeFunc );
    res = curl_easy_perform( curl );
    if(res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
                  curl_easy_strerror(res));
    }
    curl_easy_cleanup( curl );
    return 0;
}


