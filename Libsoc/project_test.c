// Controlling the BBB using a generic librarie in C called libsoc for SoC (System on Chip)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "/home/magnus/Documentos/libsoc/lib/include/libsoc_pwm.h"
#include "/home/magnus/Documentos/libsoc/lib/include/libsoc_debug.h"
#include "/home/magnus/Documentos/libsoc/lib/include/libsoc_gpio.h"
#include "/home/magnus/Documentos/libsoc/lib/include/libsoc_debug.h"

#define PWM_OUTPUT_CHIP 0
#define PWM_CHIP_OUTPUT 1
#define GPIO_OUTPUT  115
#define GPIO_INPUT   7

/* PORTAS USADAS PARA FAZER O ROBO ANDAR E FAZER O SENSORIAMENTO DO AMBIENTE

 PWM
BlackPWM 	motorR(P8_13);
BlackPWM 	motorL(P8_19);

 Saida para ponte H 
BlackGPIO  	IN1(GPIO_65, output, SecureMode); // dir
BlackGPIO  	IN2(GPIO_27, output, SecureMode);
BlackGPIO  	IN3(GPIO_47, output, SecureMode); // esq
BlackGPIO  	IN4(GPIO_46, output, SecureMode);

 Pino para enable de todo o programa 
BlackGPIO  	button(GPIO_26, input, SecureMode);

*/

void set_up_motors(){

}

void enable_motors(){

}

void set_up_GPIOS(){

}

int main(){

    return;
}