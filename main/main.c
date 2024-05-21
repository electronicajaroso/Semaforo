//LIBRERIAS
    //libreria general
#include <stdio.h>

    //libreria gestión entrada/salida - configuración/lectura/escritura
#include "driver/gpio.h"

    //libreria gestión de tiempos
#include "freertos/FreeRTOS.h"  //hay que incluirlo para usar RTOS  
#include "freertos/task.h"      //gestión de tareas a los nucleos

#include "esp_log.h"


//declaración constantes

#define NUM_TIMERS 1
#define INTERVALBASE 5000
static const char* TAG = "Semaforo";

//declaración variables
    //variable tipo led
struct leds
{
    int pinLed;
    int valorLed;
};
    //variable gestión de timers
    // An array to hold handles to the created timers.
TimerHandle_t xTimers[ NUM_TIMERS ];



//declaración funciones
esp_err_t iniciarLed(struct leds *);
esp_err_t blinkLed(struct leds *);
esp_err_t setTimers(void);

//definición función a llamar si expira timer
void vTimerCallback( TimerHandle_t pxTimer )
{
    int32_t idTimer;

     // Which timer expired?
     idTimer = ( int32_t ) pvTimerGetTimerID( pxTimer );
     printf("el timer %ld ha expirado\n", idTimer);
}

void app_main(void)
{
    //definición variables
    struct leds led1={2,0};
    //struct leds *led2={3,0};
    
    iniciarLed(&led1);
    setTimers();

    
    while (true)
    {
        blinkLed(&led1);
        printf("valor led %d\n", led1.valorLed);
    }

}

esp_err_t iniciarLed(struct leds *led)
{
    gpio_set_direction(led->pinLed, GPIO_MODE_OUTPUT);
    //log led iniciado
    ESP_LOGI(TAG, "led iniciado");
    return ESP_OK;
}
esp_err_t blinkLed(struct leds *led)
{
    led->valorLed = !led->valorLed;
    gpio_set_level(led->pinLed, led->valorLed);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    return ESP_OK;
}


esp_err_t setTimers(void)
{
 int32_t x;

     // Create then start some timers.  Starting the timers before the scheduler
     // has been started means the timers will start running immediately that
     // the scheduler starts.
     for( x = 0; x < NUM_TIMERS; x++ )
     {
         xTimers[ x ] = xTimerCreate(    "Timer",             // Just a text name, not used by the kernel.
                                         ( pdMS_TO_TICKS(INTERVALBASE) ), // The timer period in ticks.
                                         pdTRUE,              // The timers will auto-reload themselves when they expire.
                                         ( void * ) x,        // Assign each timer a unique id equal to its array index.
                                         vTimerCallback       // Each timer calls the same callback when it expires.
                                     );

         if( xTimers[ x ] == NULL )
         {
             // The timer was not created.
             ESP_LOGE(TAG, "The timer was not created.");
         }
         else
         {
             // Start the timer.  No block time is specified, and even if one was
             // it would be ignored because the scheduler has not yet been
             // started.
             if( xTimerStart( xTimers[ x ], 0 ) != pdPASS )
             {
                 // The timer could not be set into the Active state.
                 ESP_LOGW(TAG, "The timer could not be set into the Active state.");
             }
             else
                ESP_LOGI(TAG, "The timer created \n");
         }
     }

  return ESP_OK;   
}