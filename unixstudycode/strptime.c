
#define _XOPEN_SOURCE
#include <time.h>
#include <stdio.h>

int main() {

    struct tm tm;
    time_t t;
    if (strptime("6 Dec 2001 12:33:45", "%d %b %Y %H:%M:%S", &tm) == NULL) {
      printf("Handle error");
      return 1;
    }

    printf("year: %d; month: %d; day: %d;\n",
                tm.tm_year, tm.tm_mon, tm.tm_mday);
    printf("hour: %d; minute: %d; second: %d\n",
                tm.tm_hour, tm.tm_min, tm.tm_sec);
    printf("week day: %d; year day: %d\n", tm.tm_wday, tm.tm_yday);

    t = mktime(&tm);
    printf("seconds since the Epoch: %ld\n", (long) t);
    return 0;
}


