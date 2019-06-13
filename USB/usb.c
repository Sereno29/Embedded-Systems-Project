#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ioctl.h>
#include <linux/usbdevice_fs.h>


#define JS_EVENT_BUTTON	0x01 /*buttonpressed/released*/
#define JS_EVENT_AXIS	0x02 /*joystickmoved*/
#define JS_EVENT_INIT	0x80 /*initialstateofdevice*/


struct js_event{
unsigned int	 time;/*eventtimestampinmilliseconds*/
short		 value;/*value*/
unsigned char	 type;/*eventtype*/
unsigned char 	number;/*axis/buttonnumber*/
};


int main(){

	int fd = open("/dev/input/js0",O_RDONLY);

	if (fd<0){
		printf("cannotopendev\n");

	}else{
		printf("opened success...:)\n");
	}


	struct js_event e;

	while(1)//eventloop
		{
		read(fd, &e, sizeof(e));

		
		if(e.type == JS_EVENT_BUTTON || e.type == JS_EVENT_AXIS){
			if(e.type == JS_EVENT_BUTTON){
				printf("button#%d value:%d\n",(int) e.number, e.value);
			}else{
				printf("axis#%d value:%d\n",(int) e.number, e.value);
			}
		}else{
			printf("InitEvents\n");
		}
	}

	return 0;

}
