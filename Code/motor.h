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

    #define LEFT 0
    #define RIGHT 1

    typedef enum stt{  inactive= 0,security = 1, active = 2}State;

    typedef enum dc{ stop = 0, low = 100000, medium = 200000, high = 300000 }DutyCicle;

    typedef enum op{forward, left, right, back, stop}Operations;

    // Starting pwm pins
    void start_pwms(State*);
    void kill_pwms(State*);

    // Driving 

    void driving(DutyCicle, Operations);
    

#endif