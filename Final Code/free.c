#include "free.h"

extern gpio *trigger, *echo;

// Motor control variables
extern pwm *pwm_rig;
extern pwm *pwm_lef;

// GPIO's to control the motors
extern gpio *in1, *in2; // Right motor
extern gpio *in3, *in4; // Left motor

// Freeing the pwm and gpios controlling the right motor
void free_subsystem_right(void){
    libsoc_gpio_set_level(in1, LOW);
    libsoc_gpio_set_level(in2, LOW);
    libsoc_gpio_free(in1);
    libsoc_gpio_free(in2);
    libsoc_pwm_free(pwm_rig);
    return;
}

// Freeing the pwm and gpios controlling the left motor
void free_subsystem_left(void){
    libsoc_gpio_set_level(in3, LOW);
    libsoc_gpio_set_level(in4, LOW);
    libsoc_gpio_free(in3);
    libsoc_gpio_free(in4);
    libsoc_pwm_free(pwm_lef);
    return;
}

void kill_car(void){
    libsoc_gpio_set_level(trigger, LOW);
  
    if (echo)
    {
        libsoc_gpio_free(echo);
    }
    
    if (trigger)
    {
        libsoc_gpio_free(trigger);
    }

    free_subsystem_left();
    free_subsystem_right();
    return;
}