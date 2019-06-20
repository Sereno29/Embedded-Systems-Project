#include "sonar.h"

// ===== GLOBAL VARIABLES ============================

extern int FLAG;    // Object too close: FLAG = 2 ; object close: FLAG = 1 ; no object: FLAG = 0

// ===== FUNCTIONS ===================================

/*  This function receives as parameters the gpio pointers to trigger and echo ports.
    It sends a (approximately 20us) HIGH signal in the trigger port. The sensor responds with a signal in the echo port.
    The function calculates the time between sending the trigger and receiving all the echo signal (until echo is LOW).
    Multiplying this time interval by the sound speed (normal temperature and pressure conditions), dividing by 2 (the sound wave
    goes forth and back to the sensor) and adjusting the units, the function results in the distance between the sensor and the closest object.
    Finally the function uses the distance value to determine the FLAG used by the other parts of the code.
*/
void sonar_distance(gpio *trigger, gpio *echo){
    int i = 0;  // counter that prevest that the echo loop waits for too long

    // 'start' and 'end' variables are time stamps used to calculate the time of the sonar wave
    // 'distance' corresponds to the distance to the closest object, calculated with the time of the sonar wave
    double start, end, distance;
    struct timeval timecheck;   // Struct used to get the time stamps with the 'sys/time.h' library


    libsoc_gpio_set_level(trigger, HIGH);  // Sets a HIGH value in the trigger port and subsequently sets a LOW value.
    libsoc_gpio_set_level(trigger, LOW);   // This way the HIGH pulse lasts for approximately 20us, that is near the time specified in the sensor datasheet.

    gettimeofday(&timecheck, NULL);     // Gets the time stamp 'start' in milliseconds
    start = (double)timecheck.tv_sec * 1000 + (double)timecheck.tv_usec / 1000;

    i = 0;  // Loop waits for the echo signal to turn HIGH or ends when the 'i' counter gets too high
    while( libsoc_gpio_get_level(echo) == LOW && i < 10000){
       i++;
    }

    // Loop waits for the echo signal to turn LOW
    while( libsoc_gpio_get_level(echo) == HIGH){
    }

    gettimeofday(&timecheck, NULL);     // Gets the time stamp 'end' in milliseconds
    end = (double)timecheck.tv_sec * 1000 + (double)timecheck.tv_usec / 1000;

    if(i == 1000){
        printf("NO ECHO SIGNAL\t");     // If the counter 'i' ended the first loop, it means that the ECHO signal was not received in time
    }else{
        distance = 17.3 * (end - start);    // The difference between the time stamps is the time for the sonar wave to go forth and back
    }                                       // Hence the distance in centimeters of the closest object is calculated multiplying by the sound speed (divided by 2 and adjusted unit)

    /* At last the function sets the FLAG based on the calculated distance
        For distances lower than 20 cm: FLAG = 2
        For distances between 20 and 40 cm: FLAG = 1
        For distances higher than 40 cm: FLAG = 0
    */
    if(distance > 400)
        return;
    if(distance < 40){
        if(distance < 20)
            FLAG = 2;
        else
            FLAG = 1;
    }else
        FLAG = 0;
        
    return;
}