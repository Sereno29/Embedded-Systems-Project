#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "libsoc_gpio.h"
#include "libsoc_debug.h"

#define GPIO_OUTPUT  45
#define GPIO_INPUT   44

int main() {
	int n = 0;

    // Create both gpio pointers
    gpio *trigger, *echo;

     // Request gpios
    trigger = libsoc_gpio_request(GPIO_OUTPUT, LS_GPIO_SHARED);
    echo = libsoc_gpio_request(GPIO_INPUT, LS_GPIO_SHARED);

    // Ensure both gpio were successfully requested
    if (trigger == NULL || echo == NULL){
       goto fail;
    }
  
    // Set direction to OUTPUT
    libsoc_gpio_set_direction(trigger, OUTPUT);
  
    // Check the direction
    if (libsoc_gpio_get_direction(trigger) != OUTPUT){
        printf("Failed to set direction to OUTPUT\n");
        goto fail;
    }
    
    // Set direction to INPUT
    libsoc_gpio_set_direction(echo, INPUT);
  
    // Check the direction
    if (libsoc_gpio_get_direction(echo) != INPUT){
        printf("Failed to set direction to INPUT\n");
        goto fail;
    }

    //====================================================================================================================================
    printf("Program running .....\n");

    libsoc_gpio_set_level(trigger, HIGH);

	while(n < 20){
		sleep(1);

        if( libsoc_gpio_get_level(echo) == HIGH ){
            printf("BUTTON PRESSED !!!\n");
        }else{
            printf("...\n");
        }

		n++;
	}

    //====================================================================================================================================

    libsoc_gpio_set_level(trigger, LOW);

    fail:
  
    // If gpio_request was successful
    if (echo)
    {
        libsoc_gpio_free(echo);
    }
    
    if (trigger)
    {
        libsoc_gpio_free(trigger);
    }

	return 0;
}
