#ifndef MOTOR_H
    #define MOTOR_H

    #include "libs.h"

    // FUNCTIONS HEADERS
    
    void accelerate_motor_right(Direction direction, int duty_c);       // function to accelerate the right motor, direction is the rotation (clockwise or counterclockwise) and duty_c is the duty cycle percentage
    void accelerate_motor_left(Direction direction, int duty_c);        // function to accelerate the left motor, direction is the rotation (clockwise or counterclockwise) and duty_c is the duty cycle percentage
    void go_forward(int duty_c);                                        // function to move forward accelerating both motors using the duty_c value
    void turn_right(int level, int duty_c);                             // function to turn right, proportionally to the level value
    void turn_left(int level, int duty_c);                              // function to turn left, proportionally to the level value
    void go_backwards(int duty_c);                                      // function to move backward accelerating both motors using the duty_c value
    void rotate(Direction direction, int duty_c);                       // function to rotate the car around your own axis, in ohter words a curve with one motor off
    void stop(void);                                                    // function to stop the car, leave the motors free
    void braking(void);                                                 // function to brake the car actively
    void disable_motors(void);                                          // function to disable the motors, configuring the pins back to gpio

#endif