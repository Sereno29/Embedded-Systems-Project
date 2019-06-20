// ===== INCLUDING LIBRARY ============================
#include "free.h"

// ===== GLOBAL VARIABLES ============================

// ===== ULTRASSONIC SENSOR CONTROL VARIABLES ============================
extern gpio *trigger, *echo;

// ===== MOTOR CONTROL VARIABLES ============================
extern pwm *pwm_rig;
extern pwm *pwm_lef;

// ===== GPIO'S TO CONTROL MOTORS ============================
extern gpio *in1, *in2; // Right motor
extern gpio *in3, *in4; // Left motor


// ===== FREEING THE PWM AND GPIO'S CONTROLLING THE RIGHT MOTOR ============================
void free_subsystem_right(void){
    // Setting the digital ports to low before freeing the ports. Otherwise, they may stay in high status
    libsoc_gpio_set_level(in1, LOW);
    libsoc_gpio_set_level(in2, LOW);
    // Freeing ports
    libsoc_gpio_free(in1);
    libsoc_gpio_free(in2);
    libsoc_pwm_free(pwm_rig);
    return;
}

// ===== FREEING THE PWM AND GPIOS CONTROLLING THE LEFT MOTOR ============================
void free_subsystem_left(void){
    // Setting the digital ports to low before freeing the ports. Otherwise, they may stay in high status
    libsoc_gpio_set_level(in3, LOW);
    libsoc_gpio_set_level(in4, LOW);
    // Freeing ports
    libsoc_gpio_free(in3);
    libsoc_gpio_free(in4);
    libsoc_pwm_free(pwm_lef);
    return;
}

// ===== SHUTTING DOWN THE "POWERTRAIN" AND THE DISTANCE SENSOR ============================
void kill_car(void){
    // Setting the digital port to low before freeing the ports. Otherwise, they may stay in high status
    libsoc_gpio_set_level(trigger, LOW);
  
    if (echo) // Freeing the sensor trigger port if it's different than NULL
        libsoc_gpio_free(echo);
    
    if (trigger) // Freeing the sensor echo port if it's different than NULL
        libsoc_gpio_free(trigger);

    // Freeing "Powertrain"
    free_subsystem_left();
    free_subsystem_right();
    return;
}