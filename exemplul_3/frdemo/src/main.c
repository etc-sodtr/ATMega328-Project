/* Demo application for SODTR lab using FreeRTOS
@2016 KCs <csaba.kertesz@etc.unitbv.ro>
See documentation of FreeRTOS <http://www.freertos.org> for further details

For this file there was use code parts from www.freertos.org: http://www.freertos.org/a00122.html
also I used some code from http://www.learningaboutelectronics.com/Articles/AVR-potentiometer-circuit.php
*/

// include AVR headers
#include <avr/io.h>

// include the operating system
#include "FreeRTOS.h"
#include "task.h"

 /* Private macro definitions.
 ******************************************************************************/
 
#define mainLED_TASK_PRIORITY   (2)

// tasks
static void blink( void *pvParameters );
static void vBlinkLed(void* pvParameters);
static void potentiometer(void* pvParameters);


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
	
	

int main( void )
{	
	// initialize port
	PORTC = 0x01;
	DDRC = 0x2F;
	PORTB = 0x01;
	DDRB = 0x20;

	threshold_level= 0b10000000;

	initADC4();

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
xTaskCreate(potentiometer, "Turn", configMINIMAL_STACK_SIZE,  NULL, 1, NULL );

	// start the scheduler
	vTaskStartScheduler();

	return 0;
}
/*-----------------------------------------------------------*/

static void blink( void *pvParameters )
{
	uint8_t dir = 1; // direction flag
	TickType_t xLastWakeTime = xTaskGetTickCount();
	const TickType_t delay = 200;

	(void)pvParameters; // parameters not used

	// forever loop
	for(;;)
	{
		vTaskDelayUntil(&xLastWakeTime, delay);

		if (dir)
		{
			PORTC <<= 1;
			dir = PORTC != 0x04;
		}
		else
		{
			PORTC >>= 1;
			dir = PORTC == 0x01;
		}
	}
}

static void vBlinkLed(void* pvParameters)
{
    (void)pvParameters; // parameters not used
    
 	//uint8_t dir = 1;

    for (;; )
    {
	
        //PORTB ^= _BV(PB5);
	PORTB = 0x20;
        vTaskDelay(500);
	PORTB = 0x00;
        vTaskDelay(500);
    }

}

static void potentiometer(void* pvParameters)
{
    (void)pvParameters; // parameters not used
    
 	//uint8_t dir = 1;

   while (1) {
	ADCSRA |= (1 << ADSC); //start ADC conversion
	loop_until_bit_is_clear(ADCSRA, ADSC); //wait until ADC conversion is done		
	potentiometerValue= ADC; //read ADC value in
	if (potentiometerValue > threshold_level +20) { 
		PORTC = (1<<PC5); //turn on LED attached to port PC5
		//PORTC.5 =1;
	}
	else if(potentiometerValue< threshold_level){
		PORTC = (0<<PC5); //turn off LED attached to port PC5
		//PORTC.5 = 0;
		}
	}
	vTaskDelay(50);
}
