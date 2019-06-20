#ifndef LIBS_H
    #define LIBS_H

// ===== DATA STRUCTURES ============================

    typedef enum dir{ // Enumeration for the direction the motor's axis rotates
        
        clockwise = 0,
        counterclockwise = 1
    
    }Direction;
    
    struct js_event{           // Struct that represents when a button is pressed in the Xbox Controller
        
        unsigned int	 time; // Event times tamp in milliseconds
        short		    value; // Value
        unsigned char	 type; // Event type
        unsigned char 	number;// Axis buttonnumber/
    
    };

// ===== AUXILIARY LIBRARIES ============================
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <string.h>
    #include <math.h>
    #include <sys/wait.h>
    #include <sys/time.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <sys/ioctl.h>
    #include <linux/usbdevice_fs.h>

// ===== LIBSOC LIBRARY ============================
    #include "lib/libsoc_pwm.h"
    #include "lib/libsoc_debug.h"
    #include "lib/libsoc_gpio.h"

// ===== PROJECT'S LIBRARIES ============================
    #include "motor.h"
    #include "sonar.h"
    #include "setup.h"
    #include "free.h"

// ===== SENSOR'S PINOUT ============================    
    #define GPIO_TRIGGER  45 // P8_11
    #define GPIO_ECHO   44  // P8_12

// ===== USB DEFINITIONS TO CHARACTERIZE EVENTS ============================
    #define JS_EVENT_BUTTON	0x01	// Button pressed released
    #define JS_EVENT_AXIS	0x02	// Joy stick moved
    #define JS_EVENT_INIT	0x80	// Initial state of device

// ===== FLAGS AND DEFINITIONS ============================
    #define Axis_Range	33000 // 
    #define PWM 		1000000	// PWM maximum duty cycle
    #define vwarning	600000	// Maximum duty cycle when the warning flag is set
    #define vturn		700000	// Duty cycle when making a 180 turn
    #define DELAY		1		// Time to that the motors spend to make a 180 turn
    #define vmin		500000	// Minimum duty cycle value

// ===== VISUAL ERROR SYSTEM PINOUT ============================
    #define LED_RED		62		// Beaglebone Black pin to control red led - P8_21
    #define LED_GREEN 	36		// Beaglebone Black pin to control green led - P8_23
    
// ===== PWM PINOUT ============================
    #define MOTOR_RIGHT_IN1 65 // P8_18
    #define MOTOR_RIGHT_IN2 27 // P8_17
    #define MOTOR_LEFT_IN3 47 // P8_15
    #define MOTOR_LEFT_IN4 46 // P8_16 
    #define PWM_CHIP 7 // EHRPWM2B - RIGHT MOTOR - P8_13 - pwmchip7 e pwm-7:1 // EHRPWM2A - LEFT MOTOR - P8_19 - pwmchip7 e pwm -7:0
    #define PWM_MOTOR_RIGHT 1 // P8_13
    #define PWM_MOTOR_LEFT 0 // P8_19

#endif