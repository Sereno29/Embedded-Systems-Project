#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

#include "libsoc_gpio.h"
#include "libsoc_debug.h"

int main() {
    double start, end;
    struct timeval timecheck;

    gettimeofday(&timecheck, NULL);
    start = (double)timecheck.tv_sec * 1000 + (double)timecheck.tv_usec / 1000;

    usleep(200000);  // 200ms

    gettimeofday(&timecheck, NULL);
    end = (double)timecheck.tv_sec * 1000 + (double)timecheck.tv_usec / 1000;

    printf("%f milliseconds elapsed\n", (end - start));

    return 0;
}