/* Demo application for SODTR lab using FreeRTOS
@2016 KCs <csaba.kertesz@etc.unitbv.ro>
See documentation of FreeRTOS <http://www.freertos.org> for further details

for this file there was use code parts from www.freertos.org: http://www.freertos.org/a00122.html
also I used some code from http://www.learningaboutelectronics.com/Articles/AVR-potentiometer-circuit.php
*/

#ifndef F_CPU				//Define F_CPU if not done 
#define F_CPU 8000000UL
#endif

// include AVR headers
#include <avr/io.h>

// include the operating system
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

//include another headers                modificat de mine
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>

//include spi and tranceiver files      modificat de mine
#include "spi.h"
#include "wl_module.h"
#include "nRF24L01.h"

 /* Private macro definitions.
 ******************************************************************************/
 
#define mainLED_TASK_PRIORITY   (2)

// tasks
static void blink( void *pvParameters );
static void vBlinkLed(void* pvParameters);
static void potentiometer(void* pvParameters);
static void askPotentiometer(void* pvParameters);
static void receiveFromSPI(void* pvParameters);


/*-----------------------------------------------------------*/
//to initialize the ADC
static inline void initADC4(void) {
ADMUX |= (1 << REFS0)|4; //reference voltage on AVCC
ADCSRA |= (1 << ADPS1) | (1 << ADPS0); //ADC clock prescaler /8
ADCSRA |= (1 << ADEN); //enables the ADC
}

//initialize values for potentiometer
	uint16_t potentiometerValue;
	uint16_t threshold_level;
	SemaphoreHandle_t xSemaphore;
	

// Variabila                               modificat de mine
volatile uint8_t PTX;

int main( void )
{	
	

	//initializari  pentru spi        adaugat de mine
	wl_module_init();		//Init nRF Module
	_delay_ms(50);			//wait for Module
	sei();					//activate Interrupts
	wl_module_config();		//config nRF as RX Module, simple Version


	// initialize port
	PORTC = 0x01;
	DDRC = 0x2F;
	PORTB = 0x01;
	DDRB = 0x20;
	PORTD = 0x10;
	DDRD = 0x30;

	threshold_level= 0b10000000;

	initADC4();

	xSemaphore = xSemaphoreCreateMutex();

	// create blinking task
	xTaskCreate( blink, "Blinky", configMINIMAL_STACK_SIZE, NULL, 1, NULL );

//another task

 xTaskCreate
    (
        vBlinkLed,     //pvTaskCode
        "blink",        //pcName
        configMINIMAL_STACK_SIZE, //usStackDepth
        NULL,           //pvParameters
        mainLED_TASK_PRIORITY, //uxPriority
        NULL      //pxCreatedTask
    );

//potentiometer task
xTaskCreate(potentiometer, "Turn", configMINIMAL_STACK_SIZE,  NULL, 2, NULL );

//led by potentiometer task
xTaskCreate(askPotentiometer, "light", configMINIMAL_STACK_SIZE,  NULL, 1, NULL );

//spi receiver   adaugat de mine
xTaskCreate (receiveFromSPI, "SPI", configMINIMAL_STACK_SIZE,  NULL, 1, NULL );

	// start the scheduler
	vTaskStartScheduler();

	return 0;
}
/*-----------------------------------------------------------*/

/*static void blink( void *pvParameters )
{
	uint8_t dir = 1; // direction flag
	TickType_t xLastWakeTime = xTaskGetTickCount();
	const TickType_t delay = 500;

	(void)pvParameters; // parameters not used

	// forever loop
	for(;;)
	{
		vTaskDelayUntil(&xLastWakeTime, delay);

		if (dir)
		{
			PORTD <<= 1;
			dir = PORTD != 0x20;
		}
		else
		{
			PORTD >>= 1;
			dir = PORTD == 0x10;
		}
	}
}
*/
static void blink(void* pvParameters)
{
    (void)pvParameters; // parameters not used
    
 	uint8_t dir = 1; // direction flag
	TickType_t xLastWakeTime = xTaskGetTickCount();
	const TickType_t delay = 500;


   while (1) {
	if( xSemaphore != NULL )
    	{
       		 /* See if we can obtain the semaphore.  If the semaphore is not
        	 available wait "portMAX_DELAY" ticks to see if it becomes free. */
        	if( xSemaphoreTake( xSemaphore, ( TickType_t ) portMAX_DELAY ) == pdTRUE )
        	{
			vTaskDelayUntil(&xLastWakeTime, delay);

		if (dir)
		{
			PORTD <<= 1;
			dir = PORTD != 0x20;
		}
		else
		{
			PORTD >>= 1;
			dir = PORTD == 0x10;
		}

			xSemaphoreGive( xSemaphore );
		}
	}
	vTaskDelay(50);
	
	}
}


/*static void vBlinkLed(void* pvParameters)
{
    (void)pvParameters; // parameters not used
    
 	//uint8_t dir = 1;

    for (;; )
    {
	
        //ne-necesar: PORTB ^= _BV(PB5);


	//this turns pin C0 on and off
//turns C0 HIGH
//PORTC |=(1<<0);
PORTC = (1<<PC0);
//PAUSE 500 miliseconds
vTaskDelay(500);
//turns C0 LOW
//PORTC &= ~(1 << 0);
PORTC = (0<<PC5);
//PAUSE 500 miliseconds
vTaskDelay(500); 

   

}

}*/

static void vBlinkLed(void* pvParameters)
{
    (void)pvParameters; // parameters not used
    
 	//uint8_t dir = 1;

   while (1) {
	if( xSemaphore != NULL )
    	{
       		 /* See if we can obtain the semaphore.  If the semaphore is not
        	 available wait "portMAX_DELAY" ticks to see if it becomes free. */
        	if( xSemaphoreTake( xSemaphore, ( TickType_t ) portMAX_DELAY ) == pdTRUE )
        	{
				//this turns pin C0 on and off
//turns C0 HIGH
//PORTC |=(1<<0);
PORTC = (1<<PC0);
//PAUSE 500 miliseconds
vTaskDelay(500);
//turns C0 LOW
//PORTC &= ~(1 << 0);
PORTC = (0<<PC0);
//PAUSE 500 miliseconds
vTaskDelay(500); 

		}
	}
	vTaskDelay(50);
	
	}
}

static void potentiometer(void* pvParameters)
{
    (void)pvParameters; // parameters not used
    
 	//uint8_t dir = 1;

   while (1) {
	if( xSemaphore != NULL )
    	{
       		 /* See if we can obtain the semaphore.  If the semaphore is not
        	 available wait "portMAX_DELAY" ticks to see if it becomes free. */
        	if( xSemaphoreTake( xSemaphore, ( TickType_t ) portMAX_DELAY ) == pdTRUE )
        	{
			ADCSRA |= (1 << ADSC); //start ADC conversion
			loop_until_bit_is_clear(ADCSRA, ADSC); //wait until ADC conversion is done		
			potentiometerValue= ADC; //read ADC value in

			xSemaphoreGive( xSemaphore );
		}
	}
	vTaskDelay(50);
	
	}
}

static void askPotentiometer(void* pvParameters)
{
    (void)pvParameters; // parameters not used
    
 	//uint8_t dir = 1;

  while (1) {
	if( xSemaphore != NULL )
    	{
       		 /* See if we can obtain the semaphore.  If the semaphore is not
        	 available wait "portMAX_DELAY" ticks to see if it becomes free. */
        	if( xSemaphoreTake( xSemaphore, ( TickType_t ) portMAX_DELAY ) == pdTRUE )
        	{
	 

			if (potentiometerValue > threshold_level +20) { 
			PORTC = (1<<PC5); //turn on LED attached to port PC5
		
			}
			else if(potentiometerValue< threshold_level){
			PORTC = (0<<PC5); //turn off LED attached to port PC5
		
				}
			xSemaphoreGive( xSemaphore );
		}
	}
	vTaskDelay(50);
	    }
}

static void receiveFromSPI(void* pvParameters)
{
    (void)pvParameters; // parameters not used

//variabile pentru spi      adaugat de mine
	uint8_t payload[wl_module_PAYLOAD];		//holds the payload
	//uint8_t nRF_status;						//STATUS information of nRF24L01+
	uint8_t zaehler = 0;
   
   while(1)
    { if( xSemaphore != NULL )
    	{
       		 /* See if we can obtain the semaphore.  If the semaphore is not
        	 available wait "portMAX_DELAY" ticks to see if it becomes free. */
        	if( xSemaphoreTake( xSemaphore, ( TickType_t ) portMAX_DELAY ) == pdTRUE )
        	{
		while (!wl_module_data_ready());			//waits for RX_DR Flag in STATUS
		//nRF_status = wl_module_get_data(payload);	//reads the incomming Data to Array payload
		zaehler = payload[0];

		//modificat de mine
		if (zaehler > 125) {PORTC = (1<<PC3);} //turn on LED attached to port PC3;
		else {PORTC = (0<<PC3);} //turn off LED attached to port PC3;
		
    
		
				
		xSemaphoreGive( xSemaphore );
		}
	}
	vTaskDelay(50);
     }

}
