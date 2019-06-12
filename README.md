# Embedded-Systems-Project
_Assisted Car Steering to Avoid Collisions_

Esse projeto consiste em desenvolver uma apicação de auxílio a direção veicular usando sensores de ultrassom para identificar
e estimar a distância de obstáculos e em seguida atuar na direção do carrinho. Inicialmente foi utilizado a biblioteca 
[BoneScript](https://beagleboard.org/Support/BoneScript/) para desenvolver os códigos. Essa biblioteca é baseada em JavaScript 
e roda sobre o interpretador Node.js na Beaglebone Black, porém ao testar a implementação em JavaScript para os sensores de 
ultrassom percebeu-se um tempo de resposta muito grande para tal aplicação sendo assim crítico para o projeto o uso dessa 
linguagem. 

Logo, o projeto mudou para o desenvolvimento em C/C++ com o auxílio da biblioteca **BlackLib** que tem capacidade de gerenciar
a comunicação das portas analógicas, sinal PWM, GPIO, comunicação UART, SPI e I2C. Entretanto, devido as versões dessa
biblioteca terem sido desenvolvidas para SO sob o Kernel Linux 3.x na então época, a execução dos códigos apresentou problemas com as portas PWM devido a diferenças de Device Tree Overlays entre as versões de Kernel 3.x e a versão utilizada 4.14.

Migrou-se, então, parte do projeto para outra biblioteca em C, dessa vez a **Libsoc**. Nesta, há maior generalidade para Systems on Chip (SoC) sob interfaces usando um Kernel Linux qualquer. Pesquisando, foi possível adaptar a instalação recomendada da libsoc para executar cross-compilation tanto das bibliotecas (.h --> .o, .lo, .so e .a) como dos arquivos de execução (.c em executáveis linux-armhf). As adaptações necessárias encontram-se no arquivo [installing_libsoc](https://github.com/Sereno29/Embedded-Systems-Project/blob/master/Installing_libsoc).


Fonte BlackLib: [BlackLib v3.0](https://github.com/yigityuce/BlackLib/tree/master/v3_0)

Fonte Libsoc: [Libsoc](https://jackmitch.github.io/libsoc/#documentation)

### Integrantes
- [Caique Garbin](https://github.com/caiquegarbin)
- [Leonardo Felipe L. S. dos Santos](https://github.com/qleonardolp)
- [Luciano Klafke Junior](https://github.com/lklafke)
- [Matheus de Souza Sereno](https://github.com/Sereno29)
