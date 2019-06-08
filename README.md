# Embedded-Systems-Project
_Assisted Car Steering to Avoid Collisions_

Esse projeto consiste em desenvolver uma apicação de auxílio a direção veicular usando sensores de ultrassom para identificar
e estimar a distância de obstáculos e em seguida atuar na direção do carrinho. Inicialmente foi utilizado a biblioteca 
[BoneScript](https://beagleboard.org/Support/BoneScript/) para desenvolver os códigos. Essa biblioteca é baseada em JavaScript 
e roda sobre o interpretador Node.js na Beaglebone Black, porém ao testar a implementação em JavaScript para os sensores de 
ultrassom percebeu-se um tempo de resposta muito grande para tal aplicação sendo assim crítico para o projeto o uso dessa 
linguagem. Logo, o projeto mudou para o desenvolvimento em C/C++ com o auxílio da biblioteca **BlackLib** que tem capacidade 
de gerenciar a comunicação das portas analógicas, sinal PWM, GPIO, comunicação UART, SPI e I2C.

Fonte BlackLib: [BlackLib v3.0](https://github.com/yigityuce/BlackLib/tree/master/v3_0)

### Integrantes
- Caique Garbin
- Leonardo Felipe Lima Santos dos Santos
- Luciano Klafke Junior
- Matheus de Souza Sereno
