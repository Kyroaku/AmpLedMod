/*
 * AmpLedMod.c
 *
 * Created: 2017-12-25 21:18:42
 * Author : Marcin Dziedzic
 */ 

#include "main.h"

// ----------------------------------------------------------------------- DEFINES

#define NUM_LEDS	39			/**< Number of ws2812b physical leds. */

// ----------------------------------------------------------- STATIC DECLARATIONS

static color_t leds[NUM_LEDS];	/**< Array of colors for each physical rgb led. */

// ------------------------------------------------------------------- DEFINITIONS

int main(void)
{
	uint8_t frame[3]; /**< Buffer for frame received from controller. */
	
	/* Debug pin indicating that whether module is running. */
	DDRC |= (1<<PC5);
	PORTC |= (1<<PC5);
	
	/* Reset leds colors. */
	for(int i = 0; i < NUM_LEDS; i++)
		leds[i].val = 0;
	
	/* Init USART for bluetooth driver with 38400 baudrate. */
	bluetoothInit(38400);
	
	/* Init ws2812b driver. */
	wsInit();
	
	/* Seq leds animation. */
	seqSetFunction(eSeqStaticColor);
	
	while (1)
	{
		/* Send colors to ws2812b leds. */
		wsSend(leds, NUM_LEDS);
		
		/* Update color array (animate). */
		seqUpdate(leds, NUM_LEDS);
		
		int8_t s = 0;
		s = bluetoothGetFrame(frame);
		if(s) {
			/* New frame received. */
			if(s == 123)
			seqSetColorRGB(0, 1,1,1);
			else
			seqSetColorRGB(0, frame[0], frame[1], frame[2]);
		}
		
		/* Delay to reach appropiate animation speed. */
		/* Minimum 50us is needed for ws2812b.*/
		_delay_ms(10);
	}
}