#include "setup.h"

extern gpio *trigger, *echo;

extern gpio *in1, *in2, *in3, *in4;
extern pwm *pwm_rig, *pwm_lef;

// Setup of the GPIO ports
// Returns 1 when errors occur, 0 everything ok
int gpio_setup(void){

     // Request gpios
    trigger = libsoc_gpio_request(GPIO_TRIGGER, LS_GPIO_SHARED);
    echo = libsoc_gpio_request(GPIO_ECHO, LS_GPIO_SHARED);

    // Ensure both gpio were successfully requested
    if (trigger == NULL || echo == NULL){
       return 1;
    }
  
    // Set direction to OUTPUT
    libsoc_gpio_set_direction(trigger, OUTPUT);
  
    // Check the direction
    if (libsoc_gpio_get_direction(trigger) != OUTPUT){
        printf("Failed to set direction to TRIGGER OUTPUT\n");
        return 1;
    }
    
    // Set direction to INPUT
    libsoc_gpio_set_direction(echo, INPUT);
  
    // Check the direction
    if (libsoc_gpio_get_direction(echo) != INPUT){
        printf("Failed to set direction to ECHO INPUT\n");
        return 1;
    }

    // Turn off debug printing for fast toggle
    libsoc_set_debug(0);

}

// Enabling the pins to control the motors through PWM and digital ports
int set_up_motors(void){
    // SETTING THE RIGHT MOTOR
    // Exporting the pins used to control the H bridge 
    in1 = libsoc_gpio_request(MOTOR_RIGHT_IN1, LS_GPIO_GREEDY);
    in2 = libsoc_gpio_request(MOTOR_RIGHT_IN2, LS_GPIO_GREEDY);
    // Setting the GPIO pins to output
    libsoc_gpio_set_direction(in1, OUTPUT);
    libsoc_gpio_set_direction(in2, OUTPUT);
    // Exporting and enabling the PWM pin
    pwm_rig = libsoc_pwm_request(PWM_CHIP, PWM_MOTOR_RIGHT, LS_PWM_GREEDY);
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
    pwm_lef = libsoc_pwm_request(PWM_CHIP, PWM_MOTOR_LEFT, LS_PWM_GREEDY);
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
