# Embedded-Systems-Project
_Assisted Car Steering to Avoid Collisions_

This project focuses on the development of a car robot controlled remotely by a Xbox 360 controller connected through USB to a BeagleBone Black and with ultrassonic sensors providing assistance to the steering process of the robot. The sensors estimate the position of obstacles (walls) and avoid collisions by making the robot stop in a safe distance to the object:

- Obstacles are 300 mm near the sensor: controller executes freely the commands from the user;
- Obstacles between 300 and 200 mm near the sensor: executes the commands  given by the user but in a reduced maximum velocity;
- Obstacles at a distance smaller than 200mm: the car is prohibited to accelerate in the direction of the obstacle;

## Componentes - _Bill of Materials_
- Beaglebone Black Rev C, with Debian 9.5 2018-10-07 4GB SD IoT installled;
- L298N Motor Drive Controller Board Module Dual H Bridge [L298N](https://www.amazon.com/Qunqi-Controller-Module-Stepper-Arduino/dp/B014KMHSW6/ref=pd_cp_328_2?pd_rd_w=2spqf&pf_rd_p=ef4dc990-a9ca-4945-ae0b-f8d549198ed6&pf_rd_r=5PYWEXETQC33M4BDRFQP&pd_rd_r=76c4bafe-91f5-11e9-8704-597ede040640&pd_rd_wg=oJWKh&pd_rd_i=B014KMHSW6&psc=1&refRID=5PYWEXETQC33M4BDRFQP);
- Two (2x) [DC Motors DG01D-A130 3-6V](https://www.alibaba.com/product-detail/3v-6v-9v-dc-flat-motor_60755111901.html?spm=a2700.7724857.normalList.149.66fddd90uUyg67);
- Three (3x) Ultrassonic Sensors [HC-SR04](https://www.alibaba.com/product-detail/HC-SR04-Ultrasonic-Sensor-Module-HC_60787536064.html?spm=a2700.7724857.normalList.76.4ac73e2eXOXmhw);
- 9V Li-Po Battery;
- [MH Level converter](https://www.mepits.com/product/2413/converters/mh-level-converter);
- Voltage regulator circuit to power the BeagleBone through the 9V Battery
- Jumpers, protoboard...

## History of the Project

In the beginning of the project, the members of the group found an [API](https://github.com/Coderlane/c-pwm-api) in C language in github and tried to cross-compile the code to the BeagleBone Black. This attempt didn't work since the API (Application Programming Interface) used the udev library and our expertise into cross-compile and Debian system(enabling pwm through config-pin or uEnv.txt) was still very poor. Many thanks to Travis Lane who helped us with our questions! Unfortunately, we didn't use the API in our application.

After this initial attempt, the members opted to try the [BoneScript library](https://beagleboard.org/Support/BoneScript/) in order to develop the coding of the project in a similar way as we would program an Arduino, since the BoneScript library is very similar commands to the Arduino library. This library is based on the JavaScript language and runs through the help of the Node.js interpreter already within the BeagleBone Black. However, during some tests regarding the response time of the language to the activation of the sensors, the members realized that the response time is too long ( > 30 ms) which didn't meet the project's requirement to use the HC-SR04 sensor. The [HC-SR04 datasheet](https://www.mouser.com/ds/2/813/HCSR04-1022824.pdf) mentions that the pulse to send a wave through the trigger should have around 0.01 ms.

After this second attempt, the members started to work with the [BlackLib](http://blacklib.yigityuce.com/) which is a library in C/C++ written to control Beaglebone Black's features such as the communication of analogic ports, PwM, GPIO, UART communication, SPI and I2C. Because the versions of this library were developed for the kernel 3.x of the Debian OS in the time, the execution of the codes presented difficulties with the PWM ports as well as the input digital GPIO's. These difficulties were due to differences in the Device Tree Overlays between the kernel versions 3.x and the version used in this project which was the 4.14 (Debian 9.5 2018-10-07 4GB SD IoT). This updated version can be found in [here](https://beagleboard.org/latest-images).

Finally, the project migrated to use the [Libsoc library](https://jackmitch.github.io/libsoc/) which is a C language library that interfaces with common peripherals found in System on Chip(SoC) through generic Linux kernel interfaces. Searching through forums, the github issues tab and reaching the responsible for the library, we were able to adapt the installation of the library in order to cross-compile the library files(.h and .c --> .o, .lo, .so and .a) as well as our own code that makes use of the functions implemented in this library files. The adaptations necessary are due to the change of path to the pwm ports in the sysfs. In the new kernel, they are presented as /sys/class/pwm/pwmchip0/pwm-0:0 and in the old kernel versions they were /sys/class/pwm/pwmchip0/pwm0. These changes of path were made in the pwm.c that comes in the standard library installation which can be done followed [here](https://github.com/jackmitch/libsoc).
```
In **Libsoc**, [pwm.c](https://github.com/Sereno29/Embedded-Systems-Project/blob/master/Libsoc/pwm.c), was altered. See how one of the functions looks like after the alteration:

```
int libsoc_pwm_set_enabled(pwm *pwm, pwm_enabled enabled)
{
  char path[STR_BUF];

  if (pwm == NULL)
  {
    libsoc_pwm_debug(__func__, -1, -1, "invalid pwm pointer");
    return EXIT_FAILURE;
  }

  if (enabled != ENABLED && enabled != DISABLED)
  {
    return EXIT_FAILURE;
  }

  libsoc_pwm_debug(__func__, pwm->chip, pwm->pwm,
    "setting enabled to %s", pwm_enabled_strings[enabled]);

  sprintf(path, "/sys/class/pwm/pwmchip%d/pwm-%d:%d/enable", pwm->chip, pwm->pwm, pwm->pwm);

  return file_write_str(path, pwm_enabled_strings[enabled], 1);
}
```

Source: BlackLib: [BlackLib v3.0](https://github.com/yigityuce/BlackLib/tree/master/v3_0)

Source: Libsoc: [Libsoc](https://jackmitch.github.io/libsoc/#documentation)
 
# Embedded-Systems-Project
_Robô com Direção Assistida para Prevenção de Colisões_

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
Foi necessário atualizar a **libsoc** pois entendemos que os caminhos das portas PWM estavam descritas para Kernel Linux 3.x. Nessa versão de Kernel, as portas estão sob o caminho:
```
/sys/class/pwm/pwmchip0/pwm0
```
Para o caso do chip0 porta 0, por exemplo. Mas, pesquisando as portas no Debian 9.5 (Kernel 4.14) há incosistência com esse caminho, sendo na verdade:
```
/sys/class/pwm/pwmchip0/pwm-0:0
```
Portanto foi necessário alterar o código de pwm da **libsoc**, [pwm.c](https://github.com/Sereno29/Embedded-Systems-Project/blob/master/Libsoc/pwm.c), para usar os caminhos corretos. Veja como uma das funções do pwm.c passou a ser:

```
int libsoc_pwm_set_enabled(pwm *pwm, pwm_enabled enabled)
{
  char path[STR_BUF];

  if (pwm == NULL)
  {
    libsoc_pwm_debug(__func__, -1, -1, "invalid pwm pointer");
    return EXIT_FAILURE;
  }

  if (enabled != ENABLED && enabled != DISABLED)
  {
    return EXIT_FAILURE;
  }

  libsoc_pwm_debug(__func__, pwm->chip, pwm->pwm,
    "setting enabled to %s", pwm_enabled_strings[enabled]);

  sprintf(path, "/sys/class/pwm/pwmchip%d/pwm-%d:%d/enable", pwm->chip, pwm->pwm, pwm->pwm);

  return file_write_str(path, pwm_enabled_strings[enabled], 1);
}
```

Fonte BlackLib: [BlackLib v3.0](https://github.com/yigityuce/BlackLib/tree/master/v3_0)

Fonte Libsoc: [Libsoc](https://jackmitch.github.io/libsoc/#documentation)

## Desenvolvedores/Developers
- [Caique Garbin](https://github.com/caiquegarbin)
- [Leonardo Felipe L. S. dos Santos](https://github.com/qleonardolp)
- [Luciano Klafke Junior](https://github.com/lklafke)
- [Matheus de Souza Sereno](https://github.com/Sereno29)
