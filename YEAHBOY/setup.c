#include "setup.h"

extern gpio *trigger, *echo;

extern gpio *in1, *in2, *in3, *in4;
extern pwm *pwm_rig, *pwm_lef;

// Setup of the Sonar GPIO ports
// Returns 1 when errors occur, 0 everything ok
int sonar_setup(void){

     // Request gpios
    trigger = libsoc_gpio_request(GPIO_TRIGGER, LS_GPIO_SHARED);
    echo = libsoc_gpio_request(GPIO_ECHO, LS_GPIO_SHARED);

    // Ensure both gpio were successfully requested
    if (trigger == NULL || echo == NULL){
        printf("Failed request sonar GPIO\n");
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
int motor_setup(void){
    // ===== SETTING THE RIGHT MOTOR ============================
    // Exporting the pins used to control the H bridge 
    in1 = libsoc_gpio_request(MOTOR_RIGHT_IN1, LS_GPIO_GREEDY);
    in2 = libsoc_gpio_request(MOTOR_RIGHT_IN2, LS_GPIO_GREEDY);

    if (in1 == NULL || in2 == NULL){
        printf("Failed request enable pins in1, in2\n");
        return 1;
    }

    // Setting the GPIO pins to output
    libsoc_gpio_set_direction(in1, OUTPUT);
    // Check the direction
    if (libsoc_gpio_get_direction(in1) != OUTPUT){
        printf("Failed to set direction to in1\n");
        return 1;
    }

    libsoc_gpio_set_direction(in2, OUTPUT);
    // Check the direction
    if (libsoc_gpio_get_direction(in2) != OUTPUT){
        printf("Failed to set direction to in2\n");
        return 1;
    }

    // Exporting and enabling the PWM pin
    pwm_rig = libsoc_pwm_request(PWM_CHIP, PWM_MOTOR_RIGHT, LS_PWM_GREEDY);
    libsoc_pwm_set_enabled(pwm_rig, ENABLED);
    if( pwm_rig == NULL || libsoc_pwm_get_enabled(pwm_rig) != ENABLED){ // Checking if everything went well
        free_subsystem_right();
        printf("Right motor setup error\n");
        return 1;
    }
    // Setting the period of the pwm
    libsoc_pwm_set_period(pwm_rig, 1000000);


    // ===== SETTING THE LEFT MOTOR ============================
    // Exporting the pins used to control the H bridge 
    in3 = libsoc_gpio_request(MOTOR_LEFT_IN3, LS_GPIO_GREEDY);
    in4 = libsoc_gpio_request(MOTOR_LEFT_IN4, LS_GPIO_GREEDY);

    if (in3 == NULL || in4 == NULL){
        printf("Failed request enable pins in3, in4\n");
        return 1;
    }

    // Setting the GPIO pins to output
    libsoc_gpio_set_direction(in3, OUTPUT);
    // Check the direction
    if (libsoc_gpio_get_direction(in3) != OUTPUT){
        printf("Failed to set direction to in3\n");
        return 1;
    }

    libsoc_gpio_set_direction(in4, OUTPUT);
    // Check the direction
    if (libsoc_gpio_get_direction(in4) != OUTPUT){
        printf("Failed to set direction to in4\n");
        return 1;
    }

    // Exporting and enabling the PWM chip
    pwm_lef = libsoc_pwm_request(PWM_CHIP, PWM_MOTOR_LEFT, LS_PWM_GREEDY);
    libsoc_pwm_set_enabled(pwm_lef, ENABLED);
    if( pwm_lef == NULL || libsoc_pwm_get_enabled(pwm_lef) != ENABLED){
        free_subsystem_right();
        free_subsystem_left();
        printf("Left motor setup error\n");
        return 1;
    }
    // Setting the period of the pwm
    libsoc_pwm_set_period(pwm_lef, 1000000);

    return 0;
}
