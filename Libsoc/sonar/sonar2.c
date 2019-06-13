#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

#include "libsoc_gpio.h"
#include "libsoc_debug.h"

#define GPIO_OUTPUT  45
#define GPIO_INPUT   44

int flag = 2;

int sonar_distance(gpio *trigger, gpio *echo){
    int i = 0;

    double start, end, distance;
    struct timeval timecheck;

    libsoc_gpio_set_level(trigger, HIGH);

    libsoc_gpio_set_level(trigger, LOW);

    gettimeofday(&timecheck, NULL);
    start = (double)timecheck.tv_sec * 1000 + (double)timecheck.tv_usec / 1000;

    i = 0;
    while( libsoc_gpio_get_level(echo) == LOW && i < 10000){
       i++;
    }

    while( libsoc_gpio_get_level(echo) == HIGH){
    }

    gettimeofday(&timecheck, NULL);
    end = (double)timecheck.tv_sec * 1000 + (double)timecheck.tv_usec / 1000;

    if(i == 1000){
        printf("NO ECHO SIGNAL\t");
        return 1;
    }else{
        //printf("%f milliseconds elapsed\n", (end - start));
        distance = 17.3 * (end - start);
        printf("DISTANCE: %.2f cm\t", distance);
    }

    if(distance < 30){
        if(distance < 20)
            flag = 2;
        else
            flag = 1;
    }else
        flag = 0;

    return 0;
}

int main() {
    int n = 0, error = 0;

    // Create both gpio pointers
    gpio *trigger, *echo;

     // Request gpios
    trigger = libsoc_gpio_request(GPIO_OUTPUT, LS_GPIO_SHARED);
    echo = libsoc_gpio_request(GPIO_INPUT, LS_GPIO_SHARED);

    // Ensure both gpio were successfully requested
    if (trigger == NULL || echo == NULL){
        printf("Failed to request GPIO\n");
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

    // Turn off debug printing for fast toggle
    libsoc_set_debug(0);

    //====================================================================================================================================
    printf("Program running .....\n");

    for( n = 0; n < 200; n++ ){
        error = sonar_distance(trigger, echo);
        if(error == 1)
            goto fail;
        printf("FLAG: %d\n", flag);
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
