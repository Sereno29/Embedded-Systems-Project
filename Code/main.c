#include "motor.h"
#include <stdlib.h>

int main(void){
    State st;
    // Initializing pwms
    start_pwms(&st);
    // Various tests of velocity for the motors
    driving(low, forward);
    delay(10000);
    driving(high, forward);
    delay(10000);
    driving(stop, stop);
    // Killing pwms
    kill_pwms(&st);
    return;
}