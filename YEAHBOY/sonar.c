#include "sonar.h"

extern int FLAG;

void sonar_distance(gpio *trigger, gpio *echo){
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
    }else{
        //printf("%f milliseconds elapsed\n", (end - start));
        distance = 17.3 * (end - start);
        //printf("DISTANCE: %.2f cm\t", distance);
    }

    if(distance > 400)
        return;
    if(distance < 40){
        if(distance < 28)
            FLAG = 2;
        else
            FLAG = 1;
    }else
        FLAG = 0;
    return;
}