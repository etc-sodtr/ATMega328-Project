/* Demo application for SODTR lab using FreeRTOS
@2016 KCs <csaba.kertesz@etc.unitbv.ro>
See documentation of FreeRTOS <http://www.freertos.org> for further details
*/

// include AVR headers
#include <avr/io.h>

// include the operating system
#include "FreeRTOS.h"
#include "task.h"

// tasks
static void blink( void *pvParameters );


/*-----------------------------------------------------------*/

int main( void )
{
	// initialize port
	PORTC = 0x01;
	DDRC = 0xFF;

	// create blinking task
	xTaskCreate( blink, "Blinky", configMINIMAL_STACK_SIZE, NULL, 1, NULL );

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
			dir = PORTC != 0x80;
		}
		else
		{
			PORTC >>= 1;
			dir = PORTC == 0x01;
		}
	}
}
