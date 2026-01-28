#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"

#define DRIVER_GPIO GPIO_NUM_10
#define PASSENGER_GPIO GPIO_NUM_11
#define DRIVER_SEATBELT_GPIO GPIO_NUM_12
#define PASSENGER_SEATBELT_GPIO GPIO_NUM_13
#define IGNITION_GPIO GPIO_NUM_14

#define GREENLED_GPIO GPIO_NUM_4
#define BLUELED_GPIO GPIO_NUM_5
#define BUZZER_GPIO GPIO_NUM_6



void app_main(void)
{
    gpio_reset_pin(DRIVER_GPIO);
    gpio_set_direction(DRIVER_GPIO, GPIO_MODE_INPUT);
    gpio_pulldown_en(DRIVER_GPIO);

    gpio_reset_pin(PASSENGER_GPIO);
    gpio_set_direction(PASSENGER_GPIO, GPIO_MODE_INPUT);
    gpio_pulldown_en(PASSENGER_GPIO);

    gpio_reset_pin(DRIVER_SEATBELT_GPIO);
    gpio_set_direction(DRIVER_SEATBELT_GPIO, GPIO_MODE_INPUT);
    gpio_pulldown_en(DRIVER_SEATBELT_GPIO);

    gpio_reset_pin(PASSENGER_SEATBELT_GPIO);
    gpio_set_direction(PASSENGER_SEATBELT_GPIO, GPIO_MODE_INPUT);
    gpio_pulldown_en(PASSENGER_SEATBELT_GPIO);

    gpio_reset_pin(IGNITION_GPIO);
    gpio_set_direction(IGNITION_GPIO, GPIO_MODE_INPUT);
    gpio_pulldown_en(IGNITION_GPIO);

    gpio_reset_pin(GREENLED_GPIO);
    gpio_set_direction(GREENLED_GPIO, GPIO_MODE_OUTPUT);

    gpio_reset_pin(BLUELED_GPIO);
    gpio_set_direction(BLUELED_GPIO, GPIO_MODE_OUTPUT);

    gpio_reset_pin(BUZZER_GPIO);
    gpio_set_direction(BUZZER_GPIO, GPIO_MODE_OUTPUT);

    bool carOn = 0; //boolean for not looping the message
    bool ignitionPressed = 0; //boolean to not turn off the vehicle immediately
    printf("\n");

    while(1) {
        if(gpio_get_level(IGNITION_GPIO) && !ignitionPressed){
            ignitionPressed = 1;
            if (gpio_get_level(DRIVER_GPIO) && gpio_get_level(PASSENGER_GPIO) && gpio_get_level(PASSENGER_SEATBELT_GPIO) && gpio_get_level(DRIVER_SEATBELT_GPIO)){
                while(1){
                    gpio_set_level(GREENLED_GPIO, 0);
                    gpio_set_level(BLUELED_GPIO, 1);
                    vTaskDelay(25/  portTICK_PERIOD_MS);
                    if(!gpio_get_level(IGNITION_GPIO)){
                        ignitionPressed = 0;
                    }
                    if(!ignitionPressed && gpio_get_level(IGNITION_GPIO)){ //turns off the engine from when it was on
                        gpio_set_level(BLUELED_GPIO, 0);
                        ignitionPressed = 1; //sets the value so it does not immediately loop
                        break;
                    }
                }
            }
            else{ //if the ignition was inhibited enters this loop
                printf("Ignition inhibited \n");
                if(!gpio_get_level(DRIVER_GPIO)){
                    printf("Driver seat not occupied \n");
                }
                if(!gpio_get_level(PASSENGER_GPIO)){
                    printf("Passenger seat not occupied \n");
                }
                if(!gpio_get_level(DRIVER_SEATBELT_GPIO)){
                    printf("Driver seatbelt not fastened \n");
                }
                if(!gpio_get_level(PASSENGER_SEATBELT_GPIO)){
                    printf("Passenger seatbelt not fastened \n");
                }
                while(1){
                    gpio_set_level(BUZZER_GPIO, 1);
                    vTaskDelay(25/  portTICK_PERIOD_MS); //delays so it is not always testing to fix debounce
                    if(!gpio_get_level(IGNITION_GPIO)){
                        ignitionPressed = 0;
                    }
                    if(!ignitionPressed && gpio_get_level(IGNITION_GPIO)){ //turns off the engine from when it had error
                        gpio_set_level(BUZZER_GPIO, 0);
                        ignitionPressed = 1; //sets the value so it does not immediately loop
                        break;
                    }
                }
            }
        }
        else if (!gpio_get_level(IGNITION_GPIO)){
            ignitionPressed = 0; //allows the loop above to be reentered when the button is released
        }
        if(gpio_get_level(DRIVER_GPIO)){ //checks if the user enters the vehicle
            if(!carOn){
                printf("Welcome to enhanced alarm system model 218-W25 \n");
                carOn = 1;
            }
        }
        else{ //statement to make it so the welcome text only plays once each time the button is pressed
            carOn = 0;
        }
        if((gpio_get_level(DRIVER_GPIO)) && (gpio_get_level(PASSENGER_GPIO)) && (gpio_get_level(PASSENGER_SEATBELT_GPIO)) && (gpio_get_level(DRIVER_SEATBELT_GPIO))){
            gpio_set_level(GREENLED_GPIO, 1); //turns on the green LED if all of the correct buttons are pressed
        }
        else{
            gpio_set_level(GREENLED_GPIO, 0); //turns it off after the loop
        }
        vTaskDelay(25/  portTICK_PERIOD_MS);
    }
    
}   