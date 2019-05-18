// Functions to drive and manage interrupts in our car
// Função de loop para teste
function change(){
    /*
    if(stat == 0){
        b.analogWrite(motorRight, dutyCicle[1]);
        b.analogWrite(motorLeft, dutyCicle[1]);
        stat = 1;
    }else{
        b.analogWrite(motorRight, dutyCicle[0]);
        b.analogWrite(motorLeft, dutyCicle[0]);
        stat = 0;
    }*/
    console.log("oi");
    stat += 0.1;
    if(stat <= 1)
        b.analogWrite(motorRight, stat);
    else{
        stat = 0.5;
        b.analogWrite(motorRight, stat);
    }
        
}
// Função de auxílio a navegação do carrinho
function callInterrupt(x){
    // Esta função será chamada assim que algum dos sensores detectar que o carrinho se aproximou de algo o valor diminui
    // Caso esteja muito perto, parar o carrinho. Caso esteja próximo, diminuir a velocidade dos motores
    // Identificar qual sensor foi
    console.log(x.pin.gpio);
}

// Defining the variables for the project
var b = require('bonescript');
// Available PWM pins: P9_29, P9_31, P9_21, P9_22, P9_14, P9_16, P9_28, P9_42, P8_13, P8_19, P8_34, P8_36, P8_45, P8_46
var motorRight = "P8_13";
//var motorLeft = "P9_31";
//var sensorRight = "P8_19";
//var dutyCicle = [ 0, 0.2, 0.4, 0.6, 0.8, 1]; // valores relativos ao valor total de 3.3V que sai da Beagle
var stat = 0.5; 
//var sensors = [ "P8_X", "P8_Y"];

// Setting pins for pwm, digital and analog use
//b.pinMode(sensorRight, b.INPUT);
b.pinMode(motorRight, b.OUTPUT);
//b.pinMode(motorLeft, b.OUTPUT);
 
// Defining initial pwm value for the motors
b.analogWrite(motorRight, stat);
//b.analogWrite(motorLeft, dutyCicle[10]);
 
/*
Models of important functions in the project

analogWrite(pino, dutyCicle, frequência, função callback);
attachInterrupt(pino, handler, mode, função callback);

*/
 
// Juntar todas os códigos em um arquivo único ou gerar bibliotecas
console.log("Este é o printf do js");
//b.attachInterrupt(sensorRight, true, b.CHANGE, callInterrupt); // if the sensor goes from a value high to low, then b.FALLING

setInterval(change, 1000);
// while and for: mesma sintaxe da linguagem C
// função setInterval(function, time): faz com que a função seja sempre chamada por um determinado tempo
// função setTimeout(function, time): faz com a função dada como parâmetro seja executada a cada time milisegundos

// The difference between the setInterval and setTimeout is that setTimeout only happens once, whereas setInterval continues until you tell it to     stop
// clearTimer(): clears the execution of a timer

// Arrays: var array = [ 12, 23, 34, 64 ]
// number[0] = 12
// array.length = 4
// adding elements to the array: array.push(10)
