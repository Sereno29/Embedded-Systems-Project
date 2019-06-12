// Controlling the BBB using a generic library in C called libsoc for SoC (System on Chip)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "/home/magnus/Documentos/libsoc/lib/include/libsoc_pwm.h"
#include "/home/magnus/Documentos/libsoc/lib/include/libsoc_debug.h"
#include "/home/magnus/Documentos/libsoc/lib/include/libsoc_gpio.h"

#define PWM_OUTPUT_CHIP 0
#define PWM_CHIP_OUTPUT 1
#define GPIO_OUTPUT  115
#define GPIO_INPUT   7

/* PORTAS USADAS PARA FAZER O ROBO ANDAR E FAZER O SENSORIAMENTO DO AMBIENTE

 PWM
BlackPWM 	motorR(P8_13); EHRPWM2B
BlackPWM 	motorL(P8_19); EHRPWM2A
BOTH PWMS PINS ARE IN pwmchip6 -> PWM Subsystem 2. Ver se P8_19 é pwmchip7!!!!!!!

 Pino para enable de todo o programa 
BlackGPIO  	button(GPIO_26, input, SecureMode);

*/

// Defining the pins that will be used to activate the motors
#define MOTOR_RIGHT_IN1 65 // P8_18
#define MOTOR_RIGHT_IN2 27 // P8_17
#define MOTOR_LEFT_IN3 47 // P8_15
#define MOTOR_LEFT_IN4 46 // P8_16
#define PWM_CHIP_LEFT 5 // EHRPWM2A - LEFT MOTOR
#define PWM_CHIP_RIGHT 6 // EHRPWM2B - RIGHT MOTOR
#define PWM_MOTOR_RIGHT 0 // P8_13
#define PWM_MOTOR_LEFT 0 // P8_19

// Motor control variables
pwm *pwm_rig;
pwm *pwm_lef;

// GPIO's to control the motors
gpio *in1, *in2; // Right motor
gpio *in3, *in4; // Left motor

// Enabling the pins to control the motors through PWM and digital ports
int set_up_motors(){
    // SETTING THE RIGHT MOTOR
    // Exporting the pins used to control the H bridge 
    in1 = libsoc_gpio_request(MOTOR_RIGHT_IN1, LS_GPIO_GREEDY);
    in2 = libsoc_gpio_request(MOTOR_RIGHT_IN2, LS_GPIO_GREEDY);
    // Setting the GPIO pins to output
    libsoc_gpio_set_direction(in1, OUTPUT);
    libsoc_gpio_set_direction(in2, OUTPUT);
    // Exporting and enabling the PWM pin
    pwm_rig = libsoc_pwm_request(PWM_CHIP_RIGHT, PWM_MOTOR_RIGHT, LS_PWM_GREEDY);
    libsoc_pwm_set_enabled(pwm_rig, ENABLED);
    if( in1 == NULL || in2 == NULL || pwm_rig == NULL || libsoc_pwm_get_enabled(pwm_rig) != ENABLED){ // Checking if everything went well
        free_subsystem_right();
        printf("Erro no setup do motor direito.\n");
        return 1;
    }
    // Setting the period of the pwm
    libsoc_pwm_set_period(pwm_rig, 1000000);


    // SETTING THE LEFT MOTOR
    // Exporting the pins used to control the H bridge 
    in3 = libsoc_gpio_request(MOTOR_LEFT_IN3, LS_GPIO_GREEDY);
    in4 = libsoc_gpio_request(MOTOR_LEFT_IN4, LS_GPIO_GREEDY);
    // Setting the GPIO pins to output
    libsoc_gpio_set_direction(in3, OUTPUT);
    libsoc_gpio_set_direction(in4, OUTPUT);
    // Exporting and enabling the PWM chip
    pwm_lef = libsoc_pwm_request(PWM_CHIP_LEFT, PWM_MOTOR_LEFT, LS_PWM_GREEDY);
    libsoc_pwm_set_enabled(pwm_lef, ENABLED);
    if( in3 == NULL || in4 == NULL || pwm_lef == NULL || libsoc_pwm_get_enabled(pwm_lef) != ENABLED){
        free_subsystem_right();
        free_subsystem_left();
        printf("Erro no setup do motor esquerdo.\n");
        return 1;
    }
    // Setting the period of the pwm
    libsoc_pwm_set_period(pwm_lef, 1000000);

    return 0;
}

// Accelerating the right motor. Direction: 0 = CLOCK-WISE , 1 = COUNTERCLOCK-WISE. Velocity = value between 0 and 1000000.
void accelerate_motor_right(char direction, unsigned int velocity){
    // Checks if the motor is enabled
    if( libsoc_pwm_get_enabled(pwm_rig) == ENABLED ){
        libsoc_pwm_set_duty_cycle(pwm_rig, velocity);
        if(direction == '0'){ // Go forward (rotate clockwise)
            libsoc_gpio_set_level(in1, HIGH);
            libsoc_gpio_set_level(in2, LOW);
        }else{ // Go backwards (rotate counter-clockwise)
            libsoc_gpio_set_level(in1, LOW);
            libsoc_gpio_set_level(in2, HIGH);
        }
    }else{
        printf("Portas pwm não estão habilitadas");
    }
    return;
}

// Accelerating the right motor. Direction: 0 = CLOCK-WISE , 1 = COUNTERCLOCK-WISE. Velocity = value between 0 and 1000000.
void accelerate_motor_left(char direction, unsigned int velocity){
    // Checks if the motor is enabled
    if( libsoc_pwm_get_enabled(pwm_rig) == ENABLED ){
        libsoc_pwm_set_duty_cycle(pwm_rig, velocity);
        if(direction == '0'){ // Go forward (rotate clockwise)
            libsoc_gpio_set_level(in1, HIGH);
            libsoc_gpio_set_level(in2, LOW);
        }else{ // Go backwards (rotate counter-clockwise)
            libsoc_gpio_set_level(in1, LOW);
            libsoc_gpio_set_level(in2, HIGH);
        }
    }else{
        printf("Portas pwm não estão habilitadas");
    }
    return;
}

void accelerate_motor_left(char direction, int velocity){

}

void free_subsystem_right(void){
    libsoc_gpio_free(in1);
    libsoc_gpio_free(in2);
    libsoc_pwm_free(pwm_rig);
    return;
}

void free_subsystem_left(void){
    libsoc_gpio_free(in3);
    libsoc_gpio_free(in4);
    libsoc_pwm_free(pwm_lef);
    return;
}

int main(){

    return;
}