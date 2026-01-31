#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "esp_adc/adc_oneshot.h"

#define DRIVER_GPIO GPIO_NUM_10
#define PASSENGER_GPIO GPIO_NUM_11
#define DRIVER_SEATBELT_GPIO GPIO_NUM_12
#define PASSENGER_SEATBELT_GPIO GPIO_NUM_13
#define IGNITION_GPIO GPIO_NUM_14

#define GREENLED_GPIO GPIO_NUM_4
#define BLUELED_GPIO GPIO_NUM_5
#define BUZZER_GPIO GPIO_NUM_6

#define LEFT_LOW_BEAM GPIO_NUM_15
#define RIGHT_LOW_BEAM GPIO_NUM_16

#define HEADLIGHT_ON_GPIO GPIO_NUM_17

#define ADC_CHANNEL     ADC_CHANNEL_7 
#define ADC_ATTEN       ADC_ATTEN_DB_12
#define BITWIDTH        ADC_BITWIDTH_12

#define ADC_SENSOR_CHANNEL     ADC_CHANNEL_6


void turn_on_lights(){
    gpio_set_level(LEFT_LOW_BEAM, 1); 
    gpio_set_level(RIGHT_LOW_BEAM, 1); 
}

void turn_off_lights(){
    gpio_set_level(LEFT_LOW_BEAM, 0); 
    gpio_set_level(RIGHT_LOW_BEAM, 0); 
}

void app_main(void)
{
  //  int adc_bits;                        // ADC reading (bits)
//    int adc_mv;       

    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_2,
    };
     adc_oneshot_unit_handle_t adc2_handle;              // Unit handle
    adc_oneshot_new_unit(&init_config, &adc2_handle);  // Populate unit handle
   

    adc_oneshot_chan_cfg_t config = {
        .atten = ADC_ATTEN,
        .bitwidth = BITWIDTH
    };                                                  // Channel config
    adc_oneshot_config_channel                          // Configure the chan
    (adc2_handle, ADC_CHANNEL, &config);
   
    adc_cali_curve_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_2,
        .chan = ADC_CHANNEL,
        .atten = ADC_ATTEN,
        .bitwidth = BITWIDTH
    };                                                  

    adc_cali_handle_t adc2_cali_chan_handle;            // Calibration handle
    adc_cali_create_scheme_curve_fitting                // Populate cal handle
    (&cali_config, &adc2_cali_chan_handle);




    adc_oneshot_unit_init_cfg_t init_sensor_config = {
        .unit_id = ADC_UNIT_1,
    };
     adc_oneshot_unit_handle_t adc1_handle;              // Unit handle
    adc_oneshot_new_unit(&init_sensor_config, &adc1_handle);  // Populate unit handle

    adc_oneshot_config_channel                          // Configure the chan
    (adc1_handle, ADC_SENSOR_CHANNEL, &config);
   
    adc_cali_curve_fitting_config_t cali_sensor_config = {
        .unit_id = ADC_UNIT_1,
        .chan = ADC_SENSOR_CHANNEL,
        .atten = ADC_ATTEN,
        .bitwidth = BITWIDTH
    };                                                  // Calibration config

    adc_cali_handle_t adc1_cali_chan_handle;            // Calibration handle
    adc_cali_create_scheme_curve_fitting                // Populate cal handle
    (&cali_sensor_config, &adc1_cali_chan_handle);
    
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

    gpio_reset_pin(LEFT_LOW_BEAM);
    gpio_set_direction(LEFT_LOW_BEAM, GPIO_MODE_OUTPUT);

    gpio_reset_pin(RIGHT_LOW_BEAM);
    gpio_set_direction(RIGHT_LOW_BEAM, GPIO_MODE_OUTPUT);

    gpio_reset_pin(HEADLIGHT_ON_GPIO);
    gpio_set_direction(HEADLIGHT_ON_GPIO, GPIO_MODE_INPUT);
    gpio_pulldown_en(HEADLIGHT_ON_GPIO);


    bool carOn = 0; //boolean for not looping the message
    bool ignitionPressed = 0; //boolean to not turn off the vehicle immediately
    printf("\n");

    while(1) {
        turn_off_lights();
        if(gpio_get_level(IGNITION_GPIO) && !ignitionPressed){
            ignitionPressed = 1;
            if (gpio_get_level(DRIVER_GPIO) && gpio_get_level(PASSENGER_GPIO) && gpio_get_level(PASSENGER_SEATBELT_GPIO) && gpio_get_level(DRIVER_SEATBELT_GPIO)){
                int duskcount = 0;
                int daycount = 0;
                while(1){ //loop where vehicle is on

                    int adc_bits;                                   // ADC reading (bits)
                    adc_oneshot_read
                    (adc2_handle, ADC_CHANNEL, &adc_bits);          // Read ADC bits
       
                    int adc_mv;                                     // ADC reading (mV)
                    adc_cali_raw_to_voltage
                    (adc2_cali_chan_handle, adc_bits, &adc_mv);
                    
                    int adc_sensor_bits;                                   // ADC reading (bits)
                    adc_oneshot_read
                    (adc1_handle, ADC_SENSOR_CHANNEL, &adc_sensor_bits);          // Read ADC bits
       
                    int adc_sensor_mv;                                     // ADC reading (mV)
                    adc_cali_raw_to_voltage
                    (adc1_cali_chan_handle, adc_sensor_bits, &adc_sensor_mv);

                    gpio_set_level(GREENLED_GPIO, 0);
                    gpio_set_level(BLUELED_GPIO, 1);
                    

                    if(adc_mv > 2000){ //threshold for turning automatic headlights
                        if(adc_sensor_mv > 2000){
                            daycount += 1;
                            if(daycount >= 80){
                                turn_off_lights();
                            }
                            duskcount = 0;
                        }
                        else if(adc_sensor_mv < 600){
                            duskcount += 1;
                            if(duskcount >= 40){
                                turn_on_lights();
                            }
                            daycount = 0;
                        }
                        else{
                            duskcount = 0;
                            daycount = 0;
                        }
                    } // threshold for the lights to stay off
                    else if (adc_mv < 800){
                        turn_off_lights();
                    } //turns the lights on
                    else{
                        turn_on_lights();
                    }

                    vTaskDelay(25/  portTICK_PERIOD_MS); 

                    if(!gpio_get_level(IGNITION_GPIO)){
                        ignitionPressed = 0;
                    }
                    if(!ignitionPressed && gpio_get_level(IGNITION_GPIO)){ //turns off the engine from when it was on
                        gpio_set_level(BLUELED_GPIO, 0);
                        ignitionPressed = 1; //sets the value so it does not immediately loop
                        turn_off_lights();
                        break;
                    }
                    //printf("%d \n",adc_sensor_mv);
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

