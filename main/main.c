//LIBRERIAS
    //libreria general
#include <stdio.h>

    //libreria gestión entrada/salida - configuración/lectura/escritura
#include "driver/gpio.h"

    //libreria gestión de tiempos
#include "freertos/FreeRTOS.h"  //hay que incluirlo para usar RTOS  
#include "freertos/task.h"      //gestión de tareas a los nucleos

//declaración constantes


//declaración variables
struct leds
{
    int pinLed;
    int valorLed;
};

//declaración funciones
esp_err_t iniciarLed(struct leds *);
esp_err_t blinkLed(struct leds *);

void app_main(void)
{
    //definición variables
    struct leds led1={2,0};
    //struct leds *led2={3,0};

    iniciarLed(&led1);

    while (true)
    {
        blinkLed(&led1);
        printf("valor led %d", led1.valorLed);
    }

}

esp_err_t iniciarLed(struct leds *led)
{
    gpio_set_direction(led->pinLed, GPIO_MODE_OUTPUT);
    return ESP_OK;
}
esp_err_t blinkLed(struct leds *led)
{
    led->valorLed = !led->valorLed;
    gpio_set_level(led->pinLed, led->valorLed);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    return ESP_OK;
}