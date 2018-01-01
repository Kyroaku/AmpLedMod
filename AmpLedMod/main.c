/*
* AmpLedMod.c
*
* Created: 2017-12-25 21:18:42
* Author : Marcin Dziedzic
*/

#include "main.h"

// ----------------------------------------------------------------------- DEFINES

// ----------------------------------------------------------- STATIC DECLARATIONS

static color_t leds[MAX_LEDS];		/**< Array of colors for each physical rgb led. */
static uint8_t numLeds = 39;		/**< Actual number of ws2812b physical leds. */

static void handleFrame(frame_t *frame);

// ------------------------------------------------------------------- DEFINITIONS

int main(void)
{
	/* Buffer for frame received from controller. */
	frame_t frame;
	
	/* Debug pin indicating that whether module is running. */
	DDRC |= (1<<PC5);
	PORTC |= (1<<PC5);
	
	/* Reset leds colors. */
	for(int i = 0; i < MAX_LEDS; i++)
	leds[i].val = 0;
	
	/* Init USART for bluetooth driver with 38400 baudrate. */
	bluetoothInit(38400);
	
	/* Init ws2812b driver. */
	wsInit();
	
	/* Seq leds animation. */
	seqSetFunction(eSeqPingPong);
	
	while (1)
	{
		/* Send colors to ws2812b leds. */
		wsSend(leds, numLeds);
		
		/* Update color array (animate). */
		seqUpdate(leds, numLeds);
		
		/* Handle command, if new frame has been received. */
		if(bluetoothGetFrame(&frame, sizeof(frame))) {
			/* New frame received. */
			handleFrame(&frame);
		}
		/* Delay to reach appropiate animation speed. */
		/* Minimum 50us is needed for ws2812b.*/
		_delay_ms(10);
	}
}

static void handleFrame(frame_t *frame)
{
	switch(frame->command)
	{
		case eSetAnimation:
		seqSetFunction(frame->data[0]);
		break;
		
		case eSetAnimationColors:
		for(int i = 0; i < frame->data[0]; i++)
		{
			seqSetColorRGB(i, frame->data[i*3+1], frame->data[i*3+2], frame->data[i*3+3]);
		}
		break;
		
		case eSetAnimationSpeed:
		break;
		
		case eSetLedsCount:
		numLeds = frame->data[0];
		break;
	}
}