#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

#include "libsoc_gpio.h"
#include "libsoc_debug.h"

int main() {
	int n = 0;

    long start, end;
    struct timeval timecheck;

    // Setting GPIO as a output named trigger
    gpio *trigger = libsoc_gpio_request(45, LS_GPIO_SHARED);
    if(trigger == NULL)
        printf("Failed to export GPIO\n");

    // Ensure gpio was successfully requested
    if(trigger == NULL){
        goto fail;
    }

    // Set direction to OUTPUT
    libsoc_gpio_set_direction(trigger, OUTPUT);

    // Check the direction
    if (libsoc_gpio_get_direction(trigger) != OUTPUT){
        printf("Failed to set direction to OUTPUT\n");
        goto fail;
    }

    //====================================================================================================================================

    printf("Program running ...\n");

    gettimeofday(&timecheck, NULL);
    start = (long)timecheck.tv_usec;

    libsoc_gpio_set_level(trigger, HIGH);

    libsoc_gpio_set_level(trigger, LOW);

    gettimeofday(&timecheck, NULL);
    end = (long)timecheck.tv_usec;

    printf("%ld microseconds elapsed\n", (end - start));

    //====================================================================================================================================

    libsoc_gpio_set_level(trigger, LOW);

    fail:

    if(trigger){
        libsoc_gpio_free(trigger);
    }

	return 0;
}
