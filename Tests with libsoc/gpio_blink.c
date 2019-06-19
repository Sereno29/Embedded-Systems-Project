#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "/home/magnus/Documentos/libsoc/lib/include/libsoc_gpio.h"
#include "/home/magnus/Documentos/libsoc/lib/include/libsoc_debug.h"

int main(){
    gpio *trig = libsoc_gpio_request(45, LS_GPIO_SHARED);
    if(trig == NULL)
        printf("Erro ao exportar.\n");
    libsoc_gpio_set_direction(trig, OUTPUT);
    libsoc_gpio_set_level(trig, HIGH);
    int n=0;
    while(n<100){
        sleep(1);
        printf("Hello there\n");
        n++;
    }
    libsoc_gpio_free(trig);
    return 0;
}