// Controlling the BBB using a generic librarie in C called libsoc for SoC (System on Chip)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "/home/magnus/Documentos/libsoc/lib/include/libsoc_pwm.h"
#include "/home/magnus/Documentos/libsoc/lib/include/libsoc_debug.h"
#include "/home/magnus/Documentos/libsoc/lib/include/libsoc_gpio.h"
#include "/home/magnus/Documentos/libsoc/lib/include/libsoc_debug.h"

#define PWM_OUTPUT_CHIP 0
#define PWM_CHIP_OUTPUT 1
#define GPIO_OUTPUT  115
#define GPIO_INPUT   7

/* PORTAS USADAS PARA FAZER O ROBO ANDAR E FAZER O SENSORIAMENTO DO AMBIENTE

 PWM
BlackPWM 	motorR(P8_13);
BlackPWM 	motorL(P8_19);

 Saida para ponte H 
BlackGPIO  	IN1(GPIO_65, output, SecureMode); // dir
BlackGPIO  	IN2(GPIO_27, output, SecureMode);
BlackGPIO  	IN3(GPIO_47, output, SecureMode); // esq
BlackGPIO  	IN4(GPIO_46, output, SecureMode);

 Pino para enable de todo o programa 
BlackGPIO  	button(GPIO_26, input, SecureMode);

*/

// Defining the pins that will be used to activate the motors
#define MOTOR_RIGHT_IN1 125
#define MOTOR_RIGHT_IN2 7
#define MOTOR_LEFT_IN3 28
#define MOTOR_LEFT_IN4 93
#define PWM_CHIP 0
#define PWM_MOTOR_RIGHT 0
#define PWM_MOTOR_LEFT 1

// Motor control variables
pwm *pwm_rig;
pwm *pwm_lef;

// GPIO's to control the motors
gpio *in1, *in2; // Right motor
gpio *in3, *in4; // Left motor

// Enabling the pins to control the motors through PWM and digital ports
void set_up_motors(){
    // SETTING THE RIGHT MOTOR
    // Exporting the pins used to control the H bridge 
    in1 = libsoc_gpio_request(MOTOR_RIGHT_IN1, LS_GPIO_GREEDY);
    in2 = libsoc_gpio_request(MOTOR_RIGHT_IN2, LS_GPIO_GREEDY);
    // Setting the GPIO pins to output
    libsoc_gpio_set_direction(in1, OUTPUT);
    libsoc_gpio_set_direction(in2, OUTPUT);
    // Exporting the PWM pin
    pwm_rig = libsoc_pwm_request(PWM_CHIP, PWM_MOTOR_RIGHT, LS_PWM_GREEDY);
    libsoc_pwm_set_enabled(pwm_rig, ENABLED);


    // SETTING THE LEFT MOTOR
    // Exporting the pins used to control the H bridge 
    in3 = libsoc_gpio_request(MOTOR_LEFT_IN3, LS_GPIO_GREEDY);
    in4 = libsoc_gpio_request(MOTOR_LEFT_IN4, LS_GPIO_GREEDY);
    // Setting the GPIO pins to output
    libsoc_gpio_set_direction(in3, OUTPUT);
    libsoc_gpio_set_direction(in4, OUTPUT);
    // Exporting the PWM chip
    pwm_lef = libsoc_pwm_request(PWM_CHIP, PWM_MOTOR_LEFT, LS_PWM_GREEDY);
    libsoc_pwm_set_enabled(pwm_lef, ENABLED);

    return;
}

void enable_motors(){

}

void set_up_GPIOS(){

}

int main(){

    return;
}