#include "motor.h"
#include <stdio.h>
#include <stdlib.h>

// ******************************** FUNCTIONS TO EXPORT AND ENABLE PWM PINS ******************************** 

// Exporting the pwm pin
void export(void){
    system("echo 0 > /sys/class/pwm/pwmchip7/export");
    system("echo 1 > /sys/class/pwm/pwmchip7/export");
    return;
}
// Enabling the pwm pin
void enable(void){
    system("echo 1 > /sys/class/pwm/pwmchip7/pwm-7:0/enable");
    system("echo 1 > /sys/class/pwm/pwmchip7/pwm-7:1/enable");
    return;
}

// Unexporting the pwm pin
void unexport(void){
    system("echo 0 > /sys/class/pwm/pwmchip7/unexport");
    system("echo 1 > /sys/class/pwm/pwmchip7/unexport");
    return;
}

// Disabling the pwm pin
void disable(void){
    system("echo 0 > /sys/class/pwm/pwmchip7/pwm-7:0/enable");
    system("echo 0 > /sys/class/pwm/pwmchip7/pwm-7:1/enable");
    return;
}


// Start pwms 
void start_pwms(State *status){
    export();
    enable();
    *status = active;
    return;
}

// Disable the pwm pins
void kill_pwms(State *status){
    disable();
    unexport();
    *status = inactive;
    return;
}

// ******************************** SETTING THE VELOCITY OF THE MOTORS USING DUTY CYCLE ******************************** 

void driving(DutyCicle dc, Operations op){
    Operations oper = op;
    char msg[60];
    switch(oper){
        case 0: // Go forward
            sprintf(msg, "echo %d > /sys/class/pwm/pwmchip7/pwm-7:%d/dutycicle", dc, LEFT);
            system(msg);
            sprintf(msg, "echo %d > /sys/class/pwm/pwmchip7/pwm-7:%d/dutycicle", dc, RIGHT);
            system(msg);
            break;
        case 1: // Go left
            sprintf(msg, "echo 0 > /sys/class/pwm/pwmchip7/pwm-7:%d/dutycicle", LEFT);
            system(msg);
            sprintf(msg, "echo %d > /sys/class/pwm/pwmchip7/pwm-7:%d/dutycicle", dc, RIGHT);
            system(msg);
            break;
        case 2: // Go right
            sprintf(msg, "echo %d > /sys/class/pwm/pwmchip7/pwm-7:%d/dutycicle", dc, LEFT);
            system(msg);
            sprintf(msg, "echo 0 > /sys/class/pwm/pwmchip7/pwm-7:%d/dutycicle", RIGHT);
            system(msg);
            break;
        case 3: // Go back
            sprintf(msg, "echo -%d > /sys/class/pwm/pwmchip7/pwm-7:%d/dutycicle", dc, LEFT);
            system(msg);
            sprintf(msg, "echo -%d > /sys/class/pwm/pwmchip7/pwm-7:%d/dutycicle", dc, RIGHT);
            system(msg);
            break;
        case 4: // Stop
            sprintf(msg, "echo 0 > /sys/class/pwm/pwmchip7/pwm-7:%d/dutycicle", LEFT);
            system(msg);
            sprintf(msg, "echo 0 > /sys/class/pwm/pwmchip7/pwm-7:%d/dutycicle", RIGHT);
            system(msg);
            break;
    }
    return;
}