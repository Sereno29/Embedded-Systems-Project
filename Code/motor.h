/* 
Vamos comprar carrinho na FilipeFlop: R$ 70,00
 
    Ver estrutura para fixar sensores no carrinho
    Para controlar motores, usar ponte H para não sobrecarregar a BBB
    Ver cabeamento da BBB
    Ver comunicação e aplicativo
    Destrinchar códigos de pwm do motor e ver biblioteca de sensores
    Fazer labirinto/obstáculos e apresentação para Sematron

*/

#include <stdio.h>
#include <stdlib.h>
#include <bbb_pwm.h>


#ifndef MOTOR_H
    #define MOTOR_H

    typedef enum msg{ failure = 0, success = 1}Message;

    // escala de -10 a 10 para acelerar e freiar

    Message disable_port(); // Desabilita o motor em caso de segurança

    // Driving 

    Message stop();
    void turning_right(); // inclui a situação de curva com uma roda travada e a outra rodando ao máximo
    void turning_left(); // inclui a situação de curva com uma roda travada e a outra rodando ao máximo
    void go_forward();
    void go_back();


    void turning_right(int DUTY_CYCLE_LEFT, int DUTY_CYCLE_RIGHT){
        
        struct bbb_pwm_controller_t* bpc = NULL; 

        struct bbb_pwm_t* bp_left = NULL;
        struct bbb_pwm_t* bp_right = NULL;

        char* pwm_name_left = "pwm_test_P9_14";
        char* pwm_name_right = "pwm_test_P9_16";

        bpc = bbb_pwm_controller_new();

        foreach_pwm(bp_left, bpc) {
            printf("%s\n", bbb_pwm_get_name(bp_left));
        }
        foreach_pwm(bp_right, bpc) {
            printf("%s\n", bbb_pwm_get_name(bp_right));
        }

        // Try getting a pwm.
        bp_left = bbb_pwm_controller_get_pwm(bpc, pwm_name_left);
        bp_right = bbb_pwm_controller_get_pwm( bpc, pwm_name_right);

        if(bp_left == NULL) {
            fprintf(stderr, "Failed to get pwm %s.\n", pwm_name_left);
            goto out;
        }
        if(bp_right == NULL) {
            fprintf(stderr, "Failed to get pwm %s.\n", pwm_name_right);
            goto out;
        }

        // Claim it so we can open some files.
        bbb_pwm_claim(bp_left);
        bbb_pwm_claim(bp_right);

        // Stop the pwm so we don't accidentally do something.
        bbb_pwm_stop(bp_left);
        bbb_pwm_stop(bp_right);

        // duty cycle percent, 0.0 --- 100.0
        bbb_pwm_set_duty_percent(bp_left, DUTY_CYCLE_LEFT);
        bbb_pwm_set_duty_percent(bp_right, DUTY_CYCLE_RIGHT);

    out:
            // Free the controller and all of the PWMs it manages.  
            bbb_pwm_controller_delete(&bpc);
    }
#endif