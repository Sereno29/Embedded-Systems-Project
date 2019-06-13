#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ioctl.h>
#include <linux/usbdevice_fs.h>


#define JS_EVENT_BUTTON	0x01	/*buttonpressed/released*/
#define JS_EVENT_AXIS	0x02	/*joystickmoved*/
#define JS_EVENT_INIT	0x80	/*initialstateofdevice*/

#define Axis_Range	32757
#define PWM 		256	//PWM max

//Para ganhar processamento, vou criar a variavel global e fazer apenas a operação na função
long long conversion = Axis_Range/PWM;
double vmax = 0;
int REng;		//Right Engine
int LEng;		//Left Engine


//About engine control
void engine (int number, int value){
	if (number == 1){
		vmax = value;
	}else{
		if(value > 0){ 					//car is turning to right side	
			LEng = vmax; 				//Left wheel full speed
			REng = vmax - vmax*(value/Axis_Range); 	//Right wheel slower
		}else{
			REng = vmax;
			LEng = vmax - vmax*(value/Axis_Range);
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
		

	while(1)//eventloop
		{
		read(fd, &e, sizeof(e));

		if(e.type == JS_EVENT_BUTTON || e.type == JS_EVENT_AXIS){
			if(e.type == JS_EVENT_BUTTON){
				//Put a Switch function to do things when pushing buttons
			}else{
				engine(e.number, e.value);	//Call Engine
			}
		}else{
			printf("InitEvents\n");
		}
	}

	return 0;

}
