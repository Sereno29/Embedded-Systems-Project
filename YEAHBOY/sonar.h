#ifndef SONAR
#define SONAR

// Including auxiliary libraries
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

// Including libsoc library
#include "libsoc_gpio.h"
#include "libsoc_debug.h"

// FUNCTIONS
void sonar_distance(gpio *trigger, gpio *echo);

#endif