#ifndef MOTOR_H
    #define MOTOR_H

    // Including auxiliary libraries
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <sys/wait.h>

    // Including libsoc library
    #include "libsoc_pwm.h"
    #include "libsoc_debug.h"
    #include "libsoc_gpio.h"

    // Enumeration to set direction of rotation of the motors
    typedef enum dir{ clockwise = 0, counterclockwise = 1}Direction;

    // FUNCTIONS

    int set_up_motors(void);
    void accelerate_motor_right(Direction direction, int duty_c);
    void accelerate_motor_left(Direction direction, int duty_c);
    void go_forward(int duty_c);
    void turn_right(int level, int duty_c);
    void turn_left(int level, int duty_c);
    void go_backwards(int duty_c);
    void rotate(Direction direction, int duty_c);
    void stop(void);
    void disable_motors(void);
    void free_subsystem_right(void);
    void free_subsystem_left(void);

// Prototypes of the sensor manipulation functions
int set_up_sensors(void);
int set_direction(gpio *trig, gpio *echo, int num);
void send_pulse_US(int sensor);
void free_sensor(int num);


#endif