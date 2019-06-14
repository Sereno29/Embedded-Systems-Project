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

//#include "motor.h"


#define JS_EVENT_BUTTON	0x01	/*buttonpressed/released*/
#define JS_EVENT_AXIS	0x02	/*joystickmoved*/
#define JS_EVENT_INIT	0x80	/*initialstateofdevice*/

#define Axis_Range	32757
#define PWM 		1000000	//PWM max

#define vwarning	750000	//Vel on warning flag
#define vturn		800000	//180 turing vel
#define DELAY		500	//time to turn

//Para ganhar processamento, vou criar a variavel global e fazer apenas a operação na função
long long conversion = Axis_Range/PWM;
double vmax = 0;
int dir;
int REng;		//Right Engine
int LEng;		//Left Engine
int status = 1;
int FLAG = 2;

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
	}else if( number == 3){					//Car direction
		if(value > 0){ 					//car is turning to right side	
			LEng = vmax; 				//Left wheel full speed
			REng = vmax - vmax*(value/Axis_Range); 	//Right wheel slower
		}else{
			REng = vmax;
			LEng = vmax - vmax*(value/Axis_Range); //rever
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

	int fd = open("/dev/input/js0",O_RDONLY);

	if (fd<0){
		//Canot open dev blink Red LED
		
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
						if(e.value == 1){
							if(us_control == 1){
								printf("Disable AC\n");
							}
							else{
								printf("Enable AC\n");
							}
						}
					break;
					
					case 7:	//Start buttom kill all
						if(e.value == 1){
							status = 0;
						}
					break;

					case 5:	//Buttom Right 180 right turn
						if(e.value == 1){
							printf("180 Right\n");
						}
					break;
 
					case 4:	//Button Left 180 left turn
						if(e.value == 1){
							printf("180 Left\n");
						}
					break;

					case 1: // Brake with B button
						if(e.value == 1){
							printf("Breaking\n");
						}
					break;

					case 3: // Brake with B button
						if(e.value == 1){
							FLAG++;
							if(FLAG == 3)
								FLAG = 0;
							printf("FLAG: %d\n", &FLAG);
						}
					break;

					default:
					break;
				]
					
			}else{
				engine(e.number, e.value);
				printf("Vmax: %lf  Dir: %d\n", &vmax, &dir);
			}
		}else{
			//Light up Green LED (Waiting command)
		}

	}

	return 0;

}
