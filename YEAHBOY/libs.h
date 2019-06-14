#ifndef LIBS_H
    #define LIBS_H

    // Enumeration for the motor
    typedef enum dir{ clockwise = 0, counterclockwise = 1}Direction;
    
    struct js_event{
        unsigned int	 time;/*eventtimestampinmilliseconds*/
        short		 value;/*value*/
        unsigned char	 type;/*eventtype*/
        unsigned char 	number;/*axis/buttonnumber*/
    };

    // Including auxiliary libraries

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


    // Including libsoc library
    #include "libsoc_pwm.h"
    #include "libsoc_debug.h"
    #include "libsoc_gpio.h"
    #include "motor.h"
    #include "sonar.h"

    // Sensor definitions
    #define GPIO_TRIGGER  45
    #define GPIO_ECHO   44

    // USB definitions
    #define JS_EVENT_BUTTON	0x01	/* buttonpressed/released*/
    #define JS_EVENT_AXIS	0x02	/* joystickmoved */
    #define JS_EVENT_INIT	0x80	/* initialstateofdevice*/

    #define Axis_Range	32757
    #define PWM 		1000000	//PWM max

    #define vwarning	750000	// Vel on warning flag
    #define vturn		800000	// 180 turing vel
    #define DELAY		500		// time to turn
    #define vmin		500000	// minimum velocity value
    #define LED_RED		62		// Beaglebone Black pin to control red led - P8_21
    #define LED_GREEN 	36		// Beaglebone Black pin to control green led - P8_23

    // Motor definitions
    
    // Defining the pins that will be used to activate the motors
    #define MOTOR_RIGHT_IN1 65 // P8_18
    #define MOTOR_RIGHT_IN2 27 // P8_17
    #define MOTOR_LEFT_IN3 47 // P8_15
    #define MOTOR_LEFT_IN4 46 // P8_16 
    #define PWM_CHIP 7 // EHRPWM2B - RIGHT MOTOR - P8_13 - pwmchip7 e pwm-7:1 // EHRPWM2A - LEFT MOTOR - P8_19 - pwmchip7 e pwm -7:0
    #define PWM_MOTOR_RIGHT 1 // P8_13
    #define PWM_MOTOR_LEFT 0 // P8_19

#endif

/* Setup.c
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <sys/wait.h>
    #include <sys/time.h>

    // Including libsoc library
    #include "libsoc_pwm.h"
    #include "libsoc_debug.h"
    #include "libsoc_gpio.h"

    // Sensor definitions
    #define GPIO_TRIGGER  45
    #define GPIO_ECHO   44

*/

/* USB.c
     #include <stdio.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <stdlib.h>
    #include <string.h>
    #include <math.h>

    #include <sys/ioctl.h>
    #include <linux/usbdevice_fs.h>

    #include "motor.h"
    #include "sonar.h"

    #define JS_EVENT_BUTTON	0x01	/* buttonpressed/released
    #define JS_EVENT_AXIS	0x02	/* joystickmoved
    #define JS_EVENT_INIT	0x80	/* initialstateofdevice

    #define Axis_Range	32757
    #define PWM 		1000000	//PWM max

    #define vwarning	750000	// Vel on warning flag
    #define vturn		800000	// 180 turing vel
    #define DELAY		500		// time to turn
    #define vmin		500000	// minimum velocity value
    #define LED_RED		62		// Beaglebone Black pin to control red led - P8_21
    #define LED_GREEN 	36		// Beaglebone Black pin to control green led - P8_23
 */

/* SONAR.H
    // Including auxiliary libraries
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <sys/wait.h>
    #include <sys/time.h>

    // Including libsoc library
    #include "libsoc_gpio.h"
    #include "libsoc_debug.h"

 */

/* MOTOR.H
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
 */