/*
* AmpLedMod.c
*
* Created: 2017-12-25 21:18:42
* Author : Marcin Dziedzic
*/

#include <string.h>

#include "main.h"

// ----------------------------------------------------------------------- Defines

// ----------------------------------------------------------------------- Static declarations

static color_t leds[MAX_LEDS];		/**< Array of colors for each physical rgb led. */
static uint8_t numLeds = 21;		/**< Actual number of ws2812b physical leds. */

static void handleFrame(frame_t *frame);

// ----------------------------------------------------------------------- Definitions

int main(void)
{
	/* Buffer for frame received from controller. */
	frame_t frame;
	
	/* Reset leds colors. */
	for(int i = 0; i < MAX_LEDS; i++)
	leds[i].val = 0;
	
	/* Init USART for bluetooth driver with 38400 baudrate. */
	bluetoothInit(38400);
	
	/* Init ws2812b driver. */
	wsInit();
	
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
		/* Delay to reach appropriate animation speed. */
		/* Minimum 50us is needed for ws2812b.*/
		_delay_ms(2);
	}
}

// ----------------------------------------------------------------------- Static declarations

static void handleFrame(frame_t *frame)
{
	switch(frame->command)
	{
		case eSetAnimation:
		seqSetFunction(frame->data[0]);
		break;
		
		case eSetAnimationColors:
		seqSetColorCount(frame->data[0]);
		for(int i = 0; i < frame->data[0]; i++)
		{
			seqSetColorRGB(i, frame->data[i*3+1], frame->data[i*3+2], frame->data[i*3+3]);
		}
		break;
		
		case eSetAnimationSpeed:
		seqSetSpeed(frame->data[0]);
		break;
		
		case eSetAnimationSoftness:
		seqSetSoftness(frame->data[0]);
		break;
		
		case eSetAnimationSize:
		seqSetSize(frame->data[0]);
		break;
		
		case eSetLedsCount:
		numLeds = frame->data[0];
		break;
		
		case eSetAll:
		seqSetFunction(frame->data[0]);
		seqSetSpeed(frame->data[1]);
		seqSetSoftness(frame->data[2]);
		seqSetSize(frame->data[3]);
		break;
	}
}