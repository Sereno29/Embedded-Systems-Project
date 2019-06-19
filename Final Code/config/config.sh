GPIO27=/sys/class/gpio/gpio27
GPIO36=/sys/class/gpio/gpio36
GPIO44=/sys/class/gpio/gpio44
GPIO45=/sys/class/gpio/gpio45
GPIO46=/sys/class/gpio/gpio46
GPIO47=/sys/class/gpio/gpio47
GPIO62=/sys/class/gpio/gpio62
GPIO65=/sys/class/gpio/gpio65

config-pin -a P8_13 pwm
config-pin -a P8_19 pwm
cd /sys/class/gpio
if [ -f $GPIO27 ]; then
    echo 27 > export
fi

if [ -f $GPIO36 ]; then
    echo 36 > export
fi

if [ -f $GPIO44 ]; then
    echo 44 > export
fi

if [ -f $GPIO45 ]; then
    echo 45 > export
fi

if [ -f $GPIO46 ]; then
    echo 46 > export
fi

if [ -f $GPIO47 ]; then
    echo 47 > export
fi

if [ -f $GPIO62 ]; then
    echo 62 > export
fi

if [ -f $GPIO65 ]; then
    echo 65 > export
fi
