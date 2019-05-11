#include "motor.h"
#include <stdio.h>
#include <stdlib.h>

int main(void){
    State st;
    // Initializing pwms
    start_pwms(&st);
    // Various tests of velocity for the motors
    driving(low, forward);
    for(int i = 0; i<1000000; i++);
    // delay(10000);
    driving(high, forward);
    for(int i = 0; i<1000000; i++);
    // delay(10000);
    driving(stop, stop);
    for(int i = 0; i<1000000; i++);
    // Killing pwms
    kill_pwms(&st);
    return 0;
}