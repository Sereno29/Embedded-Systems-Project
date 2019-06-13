# Embedded-Systems-Project
_Assisted Car Steering to Avoid Collisions_

Esse projeto consiste em desenvolver um carrinho controlado remotamente, por joystick USB, e com auxílio de direção de sensores de ultrassom. Os sensores estimam a posição de obstáculos (paredes) e evitam a colisão alterando o comportamento do carro:

- Obstáculos afastados em mais de 30 cm: direção livre pelo joystick;
- Obstáculos entre 30 e 20 cm: velocidade máxima reduzida, controlável ainda pelo joystick;
- Obstáculos mais próximo de 20 cm: impedimento total de avançar na direção em que o obstáculo foi identificado.

## Componentes - _Bill of Materials_
- Beaglebone Black Rev C, com Debian 9.5 2018-10-07 4GB SD IoT instalada;
- Módulo driver de motor com ponte H [L298N](https://www.robocore.net/loja/drivers-de-motores/driver-motor-ponte-h-l298n);
- Dois (2x) [Motores DC DG01D-A130](https://www.robocore.net/loja/motores/motor-dc-3-6v-com-caixa-de-reducao-e-eixo-duplo);
- Três (3x) sensores de ultrassom [HC-SR04](https://www.robocore.net/loja/sensores/sensor-de-distancia-ultrassonico-hc-sr04);
- Bateria Li-Po 9V;
- Jumpers, protoboard...

## Histórico

Inicialmente foi utilizado a biblioteca [BoneScript](https://beagleboard.org/Support/BoneScript/) para desenvolver os códigos. Essa biblioteca é baseada em JavaScript e roda sobre o interpretador Node.js na Beaglebone Black, porém ao testar a implementação em JavaScript para os sensores de ultrassom percebeu-se um tempo de resposta muito grande (> 30 ms) para tal aplicação sendo assim crítico para o projeto o uso dessa linguagem. 

Logo, o projeto mudou para o desenvolvimento em C/C++ com o auxílio da biblioteca **BlackLib** que tem capacidade de gerenciar a comunicação das portas analógicas, sinal PWM, GPIO, comunicação UART, SPI e I2C. Entretanto, devido as versões dessa biblioteca terem sido desenvolvidas para SO sob o Kernel Linux 3.x na então época, a execução dos códigos apresentou problemas com as portas PWM devido a diferenças de Device Tree Overlays entre as versões de Kernel 3.x e a versão utilizada 4.14 (na Debian 9.5 2018-10-07 4GB SD IoT).

Migrou-se, então, parte do projeto para outra biblioteca em C, dessa vez a **Libsoc**. Nesta, há maior generalidade para Systems on Chip (SoC) sob interfaces usando um Kernel Linux qualquer. Pesquisando, foi possível adaptar a instalação recomendada da libsoc para executar cross-compilation tanto das bibliotecas (.h --> .o, .lo, .so e .a) como dos arquivos de execução (.c em executáveis linux-armhf). As adaptações necessárias encontram-se no arquivo [installing_libsoc](https://github.com/Sereno29/Embedded-Systems-Project/blob/master/Installing_libsoc).


Fonte BlackLib: [BlackLib v3.0](https://github.com/yigityuce/BlackLib/tree/master/v3_0)

Fonte Libsoc: [Libsoc](https://jackmitch.github.io/libsoc/#documentation)

## Desenvolvedores
- [Caique Garbin](https://github.com/caiquegarbin)
- [Leonardo Felipe L. S. dos Santos](https://github.com/qleonardolp)
- [Luciano Klafke Junior](https://github.com/lklafke)
- [Matheus de Souza Sereno](https://github.com/Sereno29)
