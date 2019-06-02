function change(){
// Alterna a direcao de rotacao ciclicamente, funcao de teste
    
    if(dir){
        b.digitalWrite(MR1, b.LOW);
	b.digitalWrite(MR2, b.HIGH);
	b.analogWrite(speedMR, 0.7);
        dir = false;
        console.log('right dir');
    }
    else{
        b.digitalWrite(MR1, b.HIGH);
	b.digitalWrite(MR2, b.LOW);
	b.analogWrite(speedMR, 0.7);
        dir = true;
        console.log('left dir');
    }
}


function turnRight(){ //funcao para virar a direita
        b.digitalWrite(MR1, b.LOW);
	b.digitalWrite(MR2, b.HIGH);
	b.analogWrite(speedMR, 0.8);

        b.digitalWrite(ML1, b.LOW);
	b.digitalWrite(ML2, b.HIGH);
        b.analogWrite(speedML, 0.5);
}

function turnLeft(){ //funcao para virar a esquerda
	b.digitalWrite(MR1, b.LOW);
	b.digitalWrite(MR2, b.HIGH);
	b.analogWrite(speedMR, 0.5);

        b.digitalWrite(ML1, b.LOW);
	b.digitalWrite(ML2, b.HIGH);
        b.analogWrite(speedML, 0.8);
}

function goForward(rightOffset, leftOffset){ //funcao para ir para frente

	rightOffset = 0.01;
	leftOffset = 0.02; //offsets para corrigir possíveis diferenças de velocidade entre os motores mesmo sob duty cycle igual	

	b.digitalWrite(MR1, b.LOW);
	b.digitalWrite(MR2, b.HIGH);
	b.analogWrite(speedMR, 0.9 + rightOffset);

        b.digitalWrite(ML1, b.LOW);
	b.digitalWrite(ML2, b.HIGH);
        b.analogWrite(speedML, 0.9 + leftOffset);
}

function goBackward(rightOffset, leftOffset){ //funcao para dar ré

	rightOffset = 0.01;
	leftOffset = 0.02;
	
	b.digitalWrite(MR1, b.HIGH);
	b.digitalWrite(MR2, b.LOW);
	b.analogWrite(speedMR, 0.6 + rightOffset);

        b.digitalWrite(ML1, b.HIGH);
	b.digitalWrite(ML2, b.LOW);
        b.analogWrite(speedML, 0.6 + leftOffset);
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
var MR1 = "P9_14";
var MR2 = "P9_16";
var speedMR = "P9_12";

var ML1 = "P9_22";
var ML2 = "P9_21";
var speedML = "P9_23";

var sensorRight = "P9_41";
var sensorLeft = "P9_42";

var duty_min = 0.40; // aparentemente pelo nível de tensão fornecido na ponte H o mínimo necessário ficou bem alto
var stat = 0.5; //Duty Cycle in %
var dir = true; //bool direção para a funcao change, teste apenas

var rightValue, leftValue;
var rightMin = 666; //definir experimentalmente qual valor min o US vai ler quando precisar desviar
var leftMin = 999; //definir experimentalmente qual valor min o US vai ler quando precisar desviar

// Setting pins for pwm, digital and analog use

b.pinMode(MR1, b.OUTPUT);
b.pinMode(MR2, b.OUTPUT);
b.pinMode(ML1, b.OUTPUT);
b.pinMode(ML2, b.OUTPUT);
b.pinMode(speedMR, b.OUTPUT);
b.pinMode(speedML, b.OUTPUT);

//Setting sensor pins:
b.pinMode(sensorRight, b.INPUT);
b.pinMode(sensorLeft, b.INPUT);


// Defining initial pwm value for the motors

b.digitalWrite(MR1, b.LOW);
b.digitalWrite(MR2, b.HIGH);
b.digitalWrite(ML1, b.LOW);
b.digitalWrite(ML2, b.HIGH);

b.analogWrite(speedMR, stat);
b.analogWrite(speedML, stat);


setInterval(change, 1700);
