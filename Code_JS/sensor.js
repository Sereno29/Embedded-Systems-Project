#!/usr/bin/env node
// The command above enables the file to be executed in the terminal of Cloud 9

// This is an example of reading HC-SR04 Ultrasonic Range Finder
// This version measures from the fall of the Trigger pulse
// to the end of the Echo pulse

var b = require('bonescript'); // Library of communication with the BBB

var trigger = 'P9_16',// Pin to trigger the ultrasonic pulse -> emitting the waves
echo = 'P9_41', // Pin to measure to pulse width related to the distams = 250; -> receiving the signal of the waves
ms = 250; // Trigger period in ms

// Auxiliary variables
var startTime, pulseTime; 

// Setting up the receiving pin of the sensor
b.pinMode(echo, b.INPUT, 7, 'pulldown', 'fast', doAttach);

// Attaching an interrupting to the receiving pin of the sensor
function doAttach(x) {
    if(x.err) { // x.err = error status message
        console.log('x.err = ' + x.err);
        return;
    }
    // Call pingEnd when the pulse ends
    b.attachInterrupt(echo, true, b.FALLING, pingEnd);
}

// Setting the emitting pin of the sensor
b.pinMode(trigger, b.OUTPUT);
b.digitalWrite(trigger, 1); // Unit triggers on a falling edge.
                            // Set trigger to high so we call pull it low

// Pull the trigger low at a regular interval.
setInterval(ping, ms);

// The funtion below makes the emissor pin go low and causes the sensor to send waves to the environment
// Pull trigger low and start timing.
function ping() {
    // console.log('ping');
    b.digitalWrite(trigger, 0);
    startTime = process.hrtime();
}

// Callback function from the interrupt attached to the receiving pin
// Compute the total time and get ready to trigger again.
function pingEnd(x) {
    if(x.attached) {
        console.log("Interrupt handler attached");
        return;
    }
    if(startTime) {
        pulseTime = process.hrtime(startTime);
        b.digitalWrite(trigger, 1);
        console.log('pulseTime = ' + (pulseTime[1]/1000000-0.8).toFixed(3));
    }
}

/*
pinMode(pin, direction, [mux], [pullup], [slew], [callback])

Note: This function doesn't work until BoneScript version 0.2.3

This function configures the mode of a digital I/O pin.

Arguments
pin: the BeagleBone pin identifier
direction: INPUT, INPUT_PULLUP (under development on 3.8 kernel) or OUTPUT
mux: index to mux mode (under development on 3.8 kernel)
pullup: 'pullup', 'pulldown' or 'disabled' (under development on 3.8 kernel)
slew: 'fast' or 'slow' (under development on 3.8 kernel)
callback: called upon completion

*/

/*
Measurement with process.hrtime()
Node.js has a more powerful mechanism to measure code execution time. It is process.hrtime() function, which returns high-resolution real time in [seconds, nanoseconds] array. The time is relative to an arbitrary time in the past (not related to the time of day) and therefore not subject to clock drifts. Another advantage of process.hrtime() over the measurement with Date is better accurateness of measures. It is very important, especially when measuring executions, which last less than a millisecond.

In order to measure code execution time you have to provide time returned by the first process.hrtime() call, as a parameter to the second process.hrtime() call.

Let’s check the following example. As in the previous example, I use setTimeout function to simulate code running for 5 millisecond.

var start = new Date()
var hrstart = process.hrtime()
var simulateTime = 5

setTimeout(function(argument) {
  // execution time simulated with setTimeout function
  var end = new Date() - start,
    hrend = process.hrtime(hrstart)

  console.info('Execution time: %dms', end)
  console.info('Execution time (hr): %ds %dms', hrend[0], hrend[1] / 1000000)
}, simulateTime)
A few execution outputs:

Execution time: 7ms
Execution time (hr): 0s 6.858948ms

Execution time: 6ms
Execution time (hr): 0s 6.285195ms

******FILIPEFLOP******

Funcionamento:
Para começar a medição é necessário alimentar o módulo e colocar o pino Trigger em nível alto por mais de 10us. Assim, o sensor emitirá uma onda sonora que, ao encontrar um obstáculo, rebaterá de volta em direção ao módulo. Durante o tempo de emissão e recebimento do sinal, o pino ECHO ficará em nível alto. Logo, o cálculo da distância pode ser feito de acordo com o tempo em que o pino ECHO permaneceu em nível alto após o pino Trigger ter sido colocado em nível alto.

Distância = [Tempo ECHO em nível alto * Velocidade do Som] / 2

A velocidade do som poder ser considerada idealmente igual a 340 m/s, logo o resultado é obtido em metros se considerado o tempo em segundos. Na fórmula, a divisão por 2 deve-se ao fato de que a onda é enviada e rebatida, ou seja, ela percorre 2 vezes a distância procurada.


*/
