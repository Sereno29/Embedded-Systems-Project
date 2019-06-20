#ifndef SONAR
    #define SONAR

    #include "libs.h"

    // FUNCTIONS HEADERS

    void sonar_distance(gpio *trigger, gpio *echo); // Function to calculate the distance seen by the US Sensor and change the flag accordingly
#endif