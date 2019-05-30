function change(){
// Alterna a direcao de rotacao ciclicamente, funcao de teste
    
    if(dir){
        b.analogWrite(motorRightA, 0.7);
        b.analogWrite(motorRightB, 0.0);
        dir = false;
        console.log('left dir');
    }
    else{
        b.analogWrite(motorRightA, 0.0);
        b.analogWrite(motorRightB, 0.7);
        dir = true;
        console.log('right dir');
    }
}


function turnRight(){ //funcao para virar a direita
        b.analogWrite(motorRightB, 0.0);
        b.analogWrite(motorRightA, 0.8);
        b.analogWrite(motorLeftB, 0.0);
        b.analogWrite(motorLeftA, 0.5);
}

function turnLeft(){ //funcao para virar a esquerda
        b.analogWrite(motorRightB, 0.0);
        b.analogWrite(motorRightA, 0.5);
        b.analogWrite(motorLeftB, 0.0);
        b.analogWrite(motorLeftA, 0.8);
}

// Função de auxílio a navegação do carrinho
function callInterrupt(x){
    // Esta função será chamada assim que algum dos sensores detectar que o carrinho se aproximou de algo o valor diminui
    // Caso esteja muito perto, parar o carrinho. Caso esteja próximo, diminuir a velocidade dos motores
    // Identificar qual sensor foi

    console.log(x.pin.gpio);
}

function usRight(Value){
    rightValue = Value; 
}

function usLeft(Value){
    leftValue = Value
}

function Avoidance(){
    //Sensor Readings
    b.analogRead(sensorRight, usRight);
    b.analogRead(sensorLeft, usLeft);
    
    //interrupção se a distância (''Value) for menor que a distância mínima (''Min). Precisa conferir o modo: FALLING, RISING or CHANGE
    
    b.attachInterrupt(sensorRight, rightValue < rightMin, b.FALLING, turnLeft()); //recebe a interrupção do ultrassom direito e vira pro lado contrário
    b.attachInterrupt(sensorLeft, leftValue < leftMin, b.FALLING, turnRight()); //recebe a interrupção do ultrassom esquerdo e vira pro lado contrário
}

// Defining the variables for the project
var b = require('bonescript');

// Available PWM pins: P9_29, P9_31, P9_21, P9_22, P9_14, P9_16, P9_28, P9_42, P8_13, P8_19, P8_34, P8_36, P8_45, P8_46
var motorRightA = "P9_14";
var motorRightB = "P9_16";

var motorLeftA = "P9_22";
var motorLeftB = "P9_21";

var sensorRight = "P9_41";
var sensorLeft = "P9_42";

var duty_min = 0.40; // aparentemente pelo nível de tensão fornecido na ponte H o mínimo necessário ficou bem alto
var stat = 0.5;
var dir = true; //bool direção para a funcao change, teste apenas

var rightValue, leftValue;
var rightMin = 666; //definir experimentalmente qual valor min o US vai ler quando precisar desviar
var leftMin = 999; //definir experimentalmente qual valor min o US vai ler quando precisar desviar

// Setting pins for pwm, digital and analog use
//b.pinMode(sensorRight, b.INPUT);

b.pinMode(motorRightA, b.OUTPUT);
b.pinMode(motorRightB, b.OUTPUT);
b.pinMode(motorLeftA, b.OUTPUT);
b.pinMode(motorLeftB, b.OUTPUT);

//Setting sensor pins:
b.pinMode(sensorRight, b.INPUT);
b.pinMode(sensorLeft, b.INPUT);


// Defining initial pwm value for the motors

b.analogWrite(motorRightA, stat);
b.analogWrite(motorRightB, stat);
b.analogWrite(motorLeftA, stat);
b.analogWrite(motorLeftB, stat);

setInterval(Avoidance, 1000);
