#include "motor.h"

// ===== GLOBAL VARIABLES ============================

// Motor control variables
extern pwm *pwm_rig; //Right motor
extern pwm *pwm_lef; //Left motor

// GPIO's to control the motors
extern gpio *in1, *in2; // Right motor
extern gpio *in3, *in4; // Left motor

// ===== FUNCTIONS ============================

// Accelerating the right motor. Direction: 0 = CLOCKWISE , 1 = COUNTERCLOCKWISE. Duty_c = VALUE BETWEEN 0-100.

void accelerate_motor_right(Direction direction, int duty_c){

    // Checks if the motor is enabled

    if( libsoc_pwm_get_enabled(pwm_rig) == ENABLED ){
        if(direction == clockwise){                     // Define the pins to move forward (rotate clockwise)
            libsoc_gpio_set_level(in1, HIGH);
            libsoc_gpio_set_level(in2, LOW);
        }else{                                          // Define the pins to move backwards (rotate counter-clockwise)
            libsoc_gpio_set_level(in1, LOW);
            libsoc_gpio_set_level(in2, HIGH);
        }
        libsoc_pwm_set_duty_cycle(pwm_rig, duty_c);     // Moving
    }else
        printf("Portas pwm não estão habilitadas");     // error printing
    return;
}

// Accelerating the right motor. Direction: 0 = CLOCKWISE , 1 = COUNTERCLOCKWISE. Duty_c = VALUE BETWEEN 0-100.

void accelerate_motor_left(Direction direction, int duty_c){

    // Checks if the motor is enabled

    if( libsoc_pwm_get_enabled(pwm_lef) == ENABLED ){
        if(direction == clockwise){                     // Define the pins to move forward (rotate clockwise)
            libsoc_gpio_set_level(in3, HIGH);
            libsoc_gpio_set_level(in4, LOW);
        }else{                                          // Define the pins to move backwards (rotate counter-clockwise)
            libsoc_gpio_set_level(in3, LOW);
            libsoc_gpio_set_level(in4, HIGH);
        }
        libsoc_pwm_set_duty_cycle(pwm_lef, duty_c);     // Moving
    }else
        printf("Portas pwm não estão habilitadas");     // error printing
    return;
}

// Make both motors operate clockwise and go forward

void go_forward(int duty_c){
    accelerate_motor_left(clockwise, duty_c);
    accelerate_motor_right(clockwise, duty_c);
    return;
}

// Turn left in a with a certain level/angle

void turn_left(int level, int duty_c){
    accelerate_motor_right(clockwise, duty_c);              // level value come from the controller and indicates the proportion
    if(duty_c - level > 0)                                  // to make the curve
        accelerate_motor_left(clockwise, duty_c - level);
    else
        accelerate_motor_left(clockwise, 0);
    return;
}

// Turn right in a with a certain level/angle

void turn_right(int level, int duty_c){
    accelerate_motor_left(clockwise, duty_c);
    if(duty_c - level > 0)
        accelerate_motor_right(clockwise, duty_c - level);
    else
        accelerate_motor_right(clockwise, 0);
    return;
}

// Make both motors operate counterclockwise and go backwards

void go_backwards(int duty_c){
    accelerate_motor_left(counterclockwise, duty_c);
    accelerate_motor_right(counterclockwise, duty_c);
    return;
}

// Make motors operate in different directions, rotating around the own car

void rotate(Direction direction, int duty_c){
    if(direction == clockwise){
        accelerate_motor_left(direction, duty_c);
        accelerate_motor_right(counterclockwise, duty_c);
    }else{
        accelerate_motor_left(direction, duty_c);
        accelerate_motor_right(clockwise, duty_c);
    }
    return;
}

// Stopping both motors, passively

void stop(void){
    // Putting the duty cycle to 0

    libsoc_pwm_set_duty_cycle(pwm_rig, 0);
    libsoc_pwm_set_duty_cycle(pwm_lef, 0);

    // Setting the level of the digital pins in the H bridge to 0
    
    libsoc_gpio_set_level(in1, LOW);
    libsoc_gpio_set_level(in2, LOW);
    libsoc_gpio_set_level(in3, LOW);
    libsoc_gpio_set_level(in4, LOW);

    return;
}

// Stopping both motors, actively

void braking(void){
    // Putting the duty cycle to 0

    libsoc_pwm_set_duty_cycle(pwm_rig, 0);
    libsoc_pwm_set_duty_cycle(pwm_lef, 0);

    // Setting the level of the digital pins in the H bridge to 0

    libsoc_gpio_set_level(in1, HIGH);       //Both pins on HIGH in the H bridge braking the motor
    libsoc_gpio_set_level(in2, HIGH);
    libsoc_gpio_set_level(in3, HIGH);
    libsoc_gpio_set_level(in4, HIGH);

    return;
}

// Disable the ports from the motors

void disable_motors(void){
    libsoc_pwm_set_enabled(pwm_rig, DISABLED);
    libsoc_pwm_set_enabled(pwm_lef, DISABLED);
    
    return;
}