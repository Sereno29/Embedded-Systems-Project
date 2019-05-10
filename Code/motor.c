#include "motor.h"

// Usar sprintf para comandos de pwm e regular velocidades

// Exporting the pwm pin
void export(void){
    system("echo 0 > /sys/class/pwm/pwmchip7/export");
    system("echo 1 > /sys/class/pwm/pwmchip7/export");
    return;
}
// Enabling the pwm pin
void enable(void){
    sytem("echo 1 > /sys/class/pwm/pwmchip7/pwm-7:0/enable");
    sytem("echo 1 > /sys/class/pwm/pwmchip7/pwm-7:1/enable");
    return;
}

// Unexporting the pwm pin
void unexport(void){
    system("echo 0 > /sys/class/pwm/pwmchip7/unexport");
    system("echo 1 > /sys/class/pwm/pwmchip7/unexport");
    return;
}

// Disabling the pwm pin
void disable(void){
    sytem("echo 0 > /sys/class/pwm/pwmchip7/pwm-7:0/enable");
    sytem("echo 0 > /sys/class/pwm/pwmchip7/pwm-7:1/enable");
    return;
}

// Start pwms 
void start_pwms(Message *status){
    export();
    import();
    *status = success;
    return;
}

// Disable the pwm pins
void kill_pwms(Message *status){
    unexport();
    disable();
    *status = failure;
    return;
}

void set_security_speed(void){
    system("echo 5000000 > /sys/class/pwm/pwmchip7/pwm-7:0/dutycicle ");
    system("echo 5000000 > /sys/class/pwm/pwmchip7/pwm-7:1/dutycicle ");
    return;
}

// escala de -10 a 10 para acelerar e freiar

Message disable_port(); // Desabilita o motor em caso de segurança

// Driving 

Message stop();
void turning_right(); // inclui a situação de curva com uma roda travada e a outra rodando ao máximo
void turning_left(); // inclui a situação de curva com uma roda travada e a outra rodando ao máximo
void go_forward(){

}
void go_back();