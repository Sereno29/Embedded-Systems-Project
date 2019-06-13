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


#define JS_EVENT_BUTTON	0x01	/*buttonpressed/released*/
#define JS_EVENT_AXIS	0x02	/*joystickmoved*/
#define JS_EVENT_INIT	0x80	/*initialstateofdevice*/

#define Axis_Range	32757
#define PWM 		1000000	//PWM max

#define vwarning	750000	//Vel on warning flag
#define vturn		800000	//180 turing vel
#define DELAY		500		//time to turn
#define vmin		500000	// minimum velocity value

// Global Variables
int flag = 2;
long long conversion = Axis_Range/PWM;
double vmax = 0;
int dir;
int REng;		//Right Engine
int LEng;		//Left Engine
int status = 1;


//About engine control
void engine (int number, int value){
	if (number == 1 && FLAG == 0){ 		//Normal
		vmax = abs(value*conversion);
		if(value > 0){
			dir = 0;
		}else{
			dir = 1;
		}
	}else if (number == 1 && FLAG == 1){	//Warning
			if(value > 0){
				dir = 0;
				if(abs(value*conversion) < vwarning){
					vmax = abs(value*conversion);
				}else{
					vmax = vwarning;
				}
			}else{
				vmax = abs(value*conversion);
				dir = 1;
			}
	}else if (number == 1 && FLAG == 2){ 	//Danger
			if(value > 0){
				vmax = 0;
			}else{
				vmax = abs(value*conversion);
				dir = 1;
			}
	}else{					//Car direction
		if(value > 0){ 					//car is turning to right side	
			LEng = vmax; 				//Left wheel full speed
			REng = vmax - vmax*(value*PWM); 	//Right wheel slower
		}else{
			REng = vmax;
			LEng = vmax - vmax*(value*PWM); //rever
		}
	}
}


struct js_event{
	unsigned int	 time;/*eventtimestampinmilliseconds*/
	short		 value;/*value*/
	unsigned char	 type;/*eventtype*/
	unsigned char 	number;/*axis/buttonnumber*/
};



int main(){

	gpio *trigger, *echo;
	gpio *led_green, *led_red;

	// if(setup(trigger, echo) == 1){
	// 	kill_car();
	// 	return 1;
	// }

	int fd = open("/dev/input/js0",O_RDONLY);

	if (fd<0){
		//Canot open dev blink Red LED
		led_red = lisbsoc_gpio_request()
		
	}else{
		//Sucess bright Green LED
	}


	struct js_event e;
		//e.number number of button/axis
		//e.value value read
		

	while(status)
		{
		read(fd, &e, sizeof(e));

		if(e.type == JS_EVENT_BUTTON || e.type == JS_EVENT_AXIS){
			if(e.type == JS_EVENT_BUTTON){
				//Put a Switch function to do things when pushing buttons
				switch(e.number){
					case 6: //Back buttom Disable avoidance colision
						
					break;
					
					case 7:	//Start buttom kill all
						status = 0;
					break;

					case 5:	//Buttom Right 180 right turn
						accelarate_motor_right(1, vturn);
						accelarate_motor_left(0, vturn);
						delay(DELAY);
					break;
 
					case 4:	//Button Left 180 left turn
						accelarate_motor_right(0, vturn);
						accelarate_motor_left(1, vturn);
						delay(DELAY);
					break;
					
					default:
					break;
					
			}else{
				engine(e.number, e.value);	
				accelarate_motor_right(dir, REng);	//Call Engine
				accelarate_motor_left(dir, LEng);
			}
		}else{
			//Light up Green LED (Waiting command)
		}
		sonar_distance(trigger, echo);
	}
	
	kill_car();
	return 0;

}
