#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "libsoc_gpio.h"
#include "libsoc_debug.h"

int main() {
	int n = 0;

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

    printf("Program running .....\n");

	while(n < 10){
		sleep(1);

        libsoc_gpio_set_level(trigger, HIGH);
        printf("HIGH\n");

        sleep(1);

        libsoc_gpio_set_level(trigger, LOW);
        printf("LOW\n");

		n++;
	}

    libsoc_gpio_set_level(trigger, LOW);

    fail:

    if(trigger){
        libsoc_gpio_free(trigger);
    }

	return 0;
}
