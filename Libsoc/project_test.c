// Controlling the BBB using a generic library in C called libsoc for SoC (System on Chip)
#include "motor.h"

/* PORTAS USADAS PARA FAZER O ROBO ANDAR E FAZER O SENSORIAMENTO DO AMBIENTE

 PWM
BlackPWM 	motorR(P8_13); EHRPWM2B
BlackPWM 	motorL(P8_19); EHRPWM2A
BOTH PWMS PINS ARE IN pwmchip6 -> PWM Subsystem 2. Ver se P8_19 é pwmchip7!!!!!!!

 Pino para enable de todo o programa 
BlackGPIO  	button(GPIO_26, input, SecureMode);

*/

// ************************************CONTROLLING 3 ULTRASSONIC HC-SR04 SENSORS************************************

// Defining the pins that will be used to activate the sensors
#define TRIG_1 45 // GPIO45 = P8_11
#define ECHO_1 44 // GPIO44 = P8_12
#define TRIG_2 69 // GPIO69 = P8_09
#define ECHO_2 68 // GPIO68 = P8_10
#define TRIG_3 66 // GPIO66 = P8_07
#define ECHO_3 67 // GPIO67 = P8_08

// Global variables used to handle the 3 sensors
gpio *trig1, *echo1; // Front sensor
gpio *trig2, *echo2; // Right sensor
gpio *trig3, *echo3; // Left sensor

int set_up_sensors(void){
    // Setup Sensor 1
    trig1 = libsoc_gpio_request(TRIG_1, LS_GPIO_GREEDY);
    echo1 = libsoc_gpio_request(ECHO_1, LS_GPIO_GREEDY);
    if(trig1 == NULL || echo1 == NULL){
        ruim1:
        printf("Não foi possível setar o sensor 1.\n");
        free_sensor(1);
        return 1;
    }else
        if(set_direction(trig1, echo1, 1))
            goto ruim1;
    libsoc_gpio_set_level(trig1, LOW);
    
    // Setup Sensor 2
    trig2 = libsoc_gpio_request(TRIG_2, LS_GPIO_GREEDY);
    echo2 = libsoc_gpio_request(ECHO_2, LS_GPIO_GREEDY);
    if(trig2 == NULL || echo2 == NULL){
        ruim2:
        printf("Não foi possível setar o sensor 1.\n");
        free_sensor(1);
        free_sensor(2);
        return 1;
    }else
        if(set_direction(trig2, echo2, 2))
            goto ruim2;
    libsoc_gpio_set_level(trig2, LOW);

    // Setup Sensor 3
    trig3 = libsoc_gpio_request(TRIG_3, LS_GPIO_GREEDY);
    echo3 = libsoc_gpio_request(ECHO_3, LS_GPIO_GREEDY);
    if(trig3 == NULL || echo3 == NULL){
        ruim3:
        printf("Não foi possível setar o sensor 1.\n");
        free_sensor(1);
        free_sensor(2);
        free_sensor(3);
        return 1;
    }else
        if(set_direction(trig3, echo3, 3))
            goto ruim3;
    libsoc_gpio_set_level(trig3, LOW);
    
    return 0;
}

// Setting the trigger to be a output gpio and the echo to be a input gpio
int set_direction(gpio *trig, gpio *echo, int num){
    libsoc_gpio_set_direction(trig, OUTPUT);
    libsoc_gpio_set_direction(echo, INPUT);
    if(libsoc_gpio_get_direction(trig) != OUTPUT || libsoc_gpio_get_direction(echo) != INPUT){
        printf("Falha ao habilitar as portas digitais do sensor %d\n.", num);
        return 1;
    }
    return 0;
}

// Making a pulse in the trigger as fast as possible
void send_pulse_US(int sensor){
    switch(sensor){
        case 1: // Sensor 1
            libsoc_gpio_set_level(trig1, HIGH);
            libsoc_gpio_set_level(trig1, LOW);
            break;
        case 2: // Sensor 2
            libsoc_gpio_set_level(trig2, HIGH);
            libsoc_gpio_set_level(trig2, LOW);
            break;
        case 3: // Sensor 3
            libsoc_gpio_set_level(trig3, HIGH);
            libsoc_gpio_set_level(trig3, LOW);
            break;
    }
    return;
}

// Freeing the sensors 
void free_sensor(int num){
    switch(num){
        case 1:
            libsoc_gpio_free(trig1);
            libsoc_gpio_free(echo1);
            break;
        case 2:
            libsoc_gpio_free(trig2);
            libsoc_gpio_free(echo2);
            break;
        case 3:
        libsoc_gpio_free(trig3);
        libsoc_gpio_free(echo3);
        break;
    }
    return;
}


// ************************************CONTROLLING 2 DC MOTORS************************************

/* Inside of /sys/class/pwm, we should have the following:

    pwmchip0: eCAP0 - controls only one PWM which is in pin P9_42
    pwmchip1: ePWM0 - EHRPWM0- controls two PWM pins which are P9_22 and P9_21 (which are PWM0A and PWM0B, respectively, should confirm with a multimeter) - 4830 0200
    pwmchip4: ePWM1 - EHRPWM1 - controls two PWM pins which are P9_14 and P9_16 ( which are PWM1A and PWM1B, respectively, should confirm with a multimeter) - 4830 2200
    pwmchip7: ePWM2 - EHRPWM2 - controls two PWM pins which are P8_19 and P8_13 (which are PWM2A and PWM2B, respectively, should confirm with a multimeter) - 4830 4200

    source: https://stackoverflow.com/questions/50204329/pwm-chip-to-pin-mapping-on-beaglebone-black-v4-14/50204330#50204330
*/  

// Defining the pins that will be used to activate the motors
#define MOTOR_RIGHT_IN1 65 // P8_18
#define MOTOR_RIGHT_IN2 27 // P8_17
#define MOTOR_LEFT_IN3 47 // P8_15
#define MOTOR_LEFT_IN4 46 // P8_16 
#define PWM_CHIP 7 // EHRPWM2B - RIGHT MOTOR - P8_13 - pwmchip7 e pwm-7:1 // EHRPWM2A - LEFT MOTOR - P8_19 - pwmchip7 e pwm -7:0
#define PWM_MOTOR_RIGHT 1 // P8_13
#define PWM_MOTOR_LEFT 0 // P8_19

/* Global variables */

// Motor control variables
pwm *pwm_rig;
pwm *pwm_lef;

// GPIO's to control the motors
gpio *in1, *in2; // Right motor
gpio *in3, *in4; // Left motor

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

// Accelerating the right motor. Direction: 0 = CLOCKWISE , 1 = COUNTERCLOCKWISE. Duty_c = VALUE BETWEEN 0-100.
void accelerate_motor_right(Direction direction, int duty_c){
    // Checks if the motor is enabled
    if( libsoc_pwm_get_enabled(pwm_rig) == ENABLED ){
        if(direction == clockwise){ // Go forward (rotate clockwise)
            libsoc_gpio_set_level(in1, HIGH);
            libsoc_gpio_set_level(in2, LOW);
        }else{ // Go backwards (rotate counter-clockwise)
            libsoc_gpio_set_level(in1, LOW);
            libsoc_gpio_set_level(in2, HIGH);
        }
        libsoc_pwm_set_duty_cycle(pwm_rig, duty_c);
    }else
        printf("Portas pwm não estão habilitadas");
    return;
}

// Accelerating the right motor. Direction: 0 = CLOCKWISE , 1 = COUNTERCLOCKWISE. Duty_c = VALUE BETWEEN 0-100.
void accelerate_motor_left(Direction direction, int duty_c){
    // Checks if the motor is enabled
    if( libsoc_pwm_get_enabled(pwm_lef) == ENABLED ){
        if(direction == clockwise){ // Go forward (rotate clockwise)
            libsoc_gpio_set_level(in3, HIGH);
            libsoc_gpio_set_level(in4, LOW);
        }else{ // Go backwards (rotate counter-clockwise)
            libsoc_gpio_set_level(in3, LOW);
            libsoc_gpio_set_level(in4, HIGH);
        }
        libsoc_pwm_set_duty_cycle(pwm_lef, duty_c);
    }else
        printf("Portas pwm não estão habilitadas");
    return;
}

// Make both motors operate clockwise and go forward
void go_forward(int duty_c){
    accelerate_motor_left(clockwise, duty_c);
    accelerate_motor_right(clockwise, duty_c);
    return;
}

void turn_left(int level, int duty_c){
    accelerate_motor_right(clockwise, duty_c);
    if(duty_c - level > 0)
        accelerate_motor_left(clockwise, duty_c - level);
    else
        accelerate_motor_left(clockwise, 0);
    return;
}

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

// Make motors operate in different directions
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

// Stopping both motors
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

// Disable the ports from the motors
void disable_motors(void){
    libsoc_pwm_set_enabled(pwm_rig, DISABLED);
    libsoc_pwm_set_enabled(pwm_lef, DISABLED);
    return;
}

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
    free_subsystem_left();
    free_subsystem_right();
    return;
}

int main(){
    // Enable debug output
    libsoc_set_debug(1);
    if(set_up_motors() == 1) // if setup is not successful then end program
        return 1;
    rotate(clockwise, 500000);
    sleep(30);
    rotate(counterclockwise, 500000);
    sleep(30);
    stop();
    disable_motors();
    free_subsystem_left();
    free_subsystem_right();
    // Disable debug output
    libsoc_set_debug(0);
    return 0;
}