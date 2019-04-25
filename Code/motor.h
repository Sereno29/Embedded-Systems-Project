/* 
Vamos comprar carrinho na FilipeFlop: R$ 70,00
 
    Ver estrutura para fixar sensores no carrinho
    Para controlar motores, usar ponte H para não sobrecarregar a BBB
    Ver cabeamento da BBB
    Ver comunicação e aplicativo
    Destrinchar códigos de pwm do motor e ver biblioteca de sensores
    Fazer labirinto/obstáculos e apresentação para Sematron

*/

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
        
        
    }
#endif