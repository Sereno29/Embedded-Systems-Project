#define GPIO_TRIGGER  45
#define GPIO_ECHO   44

// Setup of the GPIO ports
// Returns 1 when errors occur, 0 everything ok
int gpio_setup(gpio *trigger, gpio *echo){

     // Request gpios
    trigger = libsoc_gpio_request(GPIO_TRIGGER, LS_GPIO_SHARED);
    echo = libsoc_gpio_request(GPIO_ECHO, LS_GPIO_SHARED);

    // Ensure both gpio were successfully requested
    if (trigger == NULL || echo == NULL){
       return 1;
    }
  
    // Set direction to OUTPUT
    libsoc_gpio_set_direction(trigger, OUTPUT);
  
    // Check the direction
    if (libsoc_gpio_get_direction(trigger) != OUTPUT){
        printf("Failed to set direction to TRIGGER OUTPUT\n");
        return 1;
    }
    
    // Set direction to INPUT
    libsoc_gpio_set_direction(echo, INPUT);
  
    // Check the direction
    if (libsoc_gpio_get_direction(echo) != INPUT){
        printf("Failed to set direction to ECHO INPUT\n");
        return 1;
    }

    // Turn off debug printing for fast toggle
    libsoc_set_debug(0);

}
