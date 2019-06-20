// Main file where the polling proces occurs

#include "libs.h" // adding the libraries used and definitions

// GLOBAL VARIABLES

int FLAG = 2; // stop flag
long long conversion = PWM/Axis_Range; // conversion coefficient to convert the value from the Xbox controller into a duty cycle from 0 to 1000000
double vmax = 0; // maximum value for the duty cycle
int dir;		//0 forward and 1 backward
int REng;		//Right Engine
int LEng;		//Left Engine
int status = 1;

// Motor
pwm *pwm_rig;
pwm *pwm_lef;
gpio *in1, *in2; // Right motor
gpio *in3, *in4; // Left motor

// Sensor
gpio *trigger ,*echo; 


// Functions that receives the data from the controller. INPUTS: number = the button pressed, value: the value of the button
// There are analogic buttons and digital ones. 
void engine (int number, float value){
	if (number == 1 && FLAG == 0){ 		//Normal
		vmax = abs(value*conversion);
		if(value < 0){
			dir = 0;
		}else{
			dir = 1;
		}
	}else if ( (number == 1 && FLAG == 1) || (number == 0 && FLAG == 1 ) ){	//Warning
			if(value < 0){
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
	}else if ( (number == 1 && FLAG == 2) || (number == 0 && FLAG == 2 ) ){ 	//Danger
			if(value < 0){
				vmax = 0;
				REng = LEng = vmax;
				braking();
			}else{
				vmax = abs(value*conversion);
				REng = LEng = vmax;
				dir = 1;
			}
	}else if( number == 0){					//Car direction
		if(value > 0){ 					//car is turning to right side	
			LEng = vmax; 				//Left wheel full speed
			REng = vmax - vmax*(value/Axis_Range); 	//Right wheel slower
		}else{
			REng = vmax;
			LEng = vmax + vmax*(value/Axis_Range);
		}
	}
}



int main(){

	int us_control = 1;

	gpio *led_green, *led_red;

	printf("Program running ...\n");

	if(sonar_setup() == 1 || motor_setup() == 1){
	 	kill_car();
		printf("SETUP ERROR\n");
	 	return 1;
	}

	printf("Setup successful ...\n");

	int fd = open("/dev/input/js0",O_RDONLY);

	if (fd<0){
		//Canot open dev blink Red LED
		led_red = libsoc_gpio_request(LED_RED, LS_GPIO_SHARED);
		libsoc_gpio_set_direction(led_red, OUTPUT);
		libsoc_gpio_set_level(led_red, HIGH);
		
	}else{
		//Sucess bright Green LED
		led_green = libsoc_gpio_request(LED_GREEN, LS_GPIO_SHARED);
		libsoc_gpio_set_direction(led_green, OUTPUT);
		libsoc_gpio_set_level(led_green, HIGH);
	}


	struct js_event e;
		//e.number number of button/axis
		//e.value value read
		//e.time time pressed
		

	while(status){
		read(fd, &e, sizeof(e));

		if(e.type == JS_EVENT_BUTTON || e.type == JS_EVENT_AXIS){
			if(e.type == JS_EVENT_BUTTON){
				//Put a Switch function to do things when pushing buttons
				switch(e.number){
					case 6: //Back buttom Disable avoidance colision
						if(e.value == 1){
							if(us_control == 1){
								us_control = 0;
								FLAG = 0;
								libsoc_gpio_set_level(led_red, HIGH);
								printf("Disable AC\n");
							}
							else{
								us_control = 1;
								libsoc_gpio_set_level(led_red, LOW);
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
							accelerate_motor_right(1, vturn);
							accelerate_motor_left(0, vturn);
							sleep(DELAY);
							braking();
						}
					break;
 
					case 4:	//Button Left 180 left turn
						if(e.value == 1){
							accelerate_motor_right(0, vturn);
							accelerate_motor_left(1, vturn);
							sleep(DELAY);
							braking();
						}
					break;

					case 1: // Brake with B button
						if(e.value == 1){
							braking();
							sleep(1);
						}
					break;

					default:
					break;
					
				}
			}else{
				engine(e.number, e.value);	
				accelerate_motor_right(dir, REng);	//Call Engine
				accelerate_motor_left(dir, LEng);
			}
		}

		if(us_control == 1)
			sonar_distance(trigger, echo);
			printf("FLAG: %d\n", FLAG);
	}
	
	kill_car();
	return 0;

}
