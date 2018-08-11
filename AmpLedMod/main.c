/*
* AmpLedMod.c
*
* Created: 2017-12-25 21:18:42
* Author : Marcin Dziedzic
*/

#include <avr/eeprom.h>

#include "main.h"

// ----------------------------------------------------------------------- Defines

#define EEPROM_ANIMATION_ADDRESS		((uint8_t*)0)
#define EEPROM_LEDS_COUNT_ADDRESS		((uint8_t*)1)
#define EEPROM_SPEED_ADDRESS			((uint8_t*)2)
#define EEPROM_SOFTNESS_ADDRESS			((uint8_t*)3)
#define EEPROM_SIZE_ADDRESS				((uint8_t*)4)
#define EEPROM_COLORS_COUNT_ADDRESS		((uint8_t*)5)
#define EEPROM_COLORS_ADDRESS			((uint8_t*)6)

// ----------------------------------------------------------------------- Static declarations

static color_t leds[MAX_LEDS];		/**< Array of colors for each physical rgb led. */
static uint8_t numLeds = 21;		/**< Actual number of ws2812b physical leds. */

static void loadSettings();

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
	
	loadSettings();
	
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
		_delay_us(50);
	}
}

// ----------------------------------------------------------------------- Static declarations

void loadSettings() {
	uint8_t val;
	
	#define EEPROM_ANIMATION_ADDRESS		((uint8_t*)0)
	#define EEPROM_LEDS_COUNT_ADDRESS		((uint8_t*)1)
	#define EEPROM_SPEED_ADDRESS			((uint8_t*)2)
	#define EEPROM_SOFTNESS_ADDRESS			((uint8_t*)3)
	#define EEPROM_SIZE_ADDRESS				((uint8_t*)4)
	#define EEPROM_COLORS_COUNT_ADDRESS		((uint8_t*)5)
	#define EEPROM_COLORS_ADDRESS			((uint8_t*)6)
	
	val = eeprom_read_byte(EEPROM_ANIMATION_ADDRESS);
	if(val >= 0 && val <= eSeqCount) seqSetFunction(val);
	
	val = eeprom_read_byte(EEPROM_LEDS_COUNT_ADDRESS);
	if(val > 0 && val <= MAX_LEDS) numLeds = val;
	
	val = eeprom_read_byte(EEPROM_SPEED_ADDRESS);
	if(val > 0 && val <= SEQ_MAX_SPEED) seqSetSpeed(val);
	
	val = eeprom_read_byte(EEPROM_SOFTNESS_ADDRESS);
	if(val > 0 && val <= SEQ_MAX_SOFTNESS) seqSetSoftness(val);
	
	val = eeprom_read_byte(EEPROM_SIZE_ADDRESS);
	if(val > 0 && val <= SEQ_MAX_SIZE) seqSetSize(val);
	
	val = eeprom_read_byte(EEPROM_COLORS_COUNT_ADDRESS);
	if(val > 0 && val <= SEQ_MAX_COLORS) {
		seqSetColorCount(val);
		for(int i = 0; i < val; i++) {
			uint8_t r = eeprom_read_byte(EEPROM_COLORS_ADDRESS + i*3 + 0);
			uint8_t g = eeprom_read_byte(EEPROM_COLORS_ADDRESS + i*3 + 1);
			uint8_t b = eeprom_read_byte(EEPROM_COLORS_ADDRESS + i*3 + 2);
			seqSetColorRGB(i, r, g, b);
		}
	}
}

void handleFrame(frame_t *frame)
{
	switch(frame->command)
	{
		case eSetAnimation:
		seqSetFunction(frame->data[0]);
		eeprom_write_byte(EEPROM_ANIMATION_ADDRESS, frame->data[0]);
		break;
		
		case eSetAnimationColors:
		if(frame->data[0] > SEQ_MAX_COLORS) break;
		
		seqSetColorCount(frame->data[0]);
		eeprom_write_byte(EEPROM_COLORS_COUNT_ADDRESS, frame->data[0]);
		for(int i = 0; i < frame->data[0]; i++)
		{
			seqSetColorRGB(i, frame->data[i*3+1], frame->data[i*3+2], frame->data[i*3+3]);
			eeprom_write_byte(EEPROM_COLORS_ADDRESS + i*3+0, frame->data[i*3+1]);
			eeprom_write_byte(EEPROM_COLORS_ADDRESS + i*3+1, frame->data[i*3+2]);
			eeprom_write_byte(EEPROM_COLORS_ADDRESS + i*3+2, frame->data[i*3+3]);
		}
		break;
		
		case eSetAnimationSpeed:
		seqSetSpeed(frame->data[0]);
		eeprom_write_byte(EEPROM_SPEED_ADDRESS, frame->data[0]);
		break;
		
		case eSetAnimationSoftness:
		seqSetSoftness(frame->data[0]);
		eeprom_write_byte(EEPROM_SOFTNESS_ADDRESS, frame->data[0]);
		break;
		
		case eSetAnimationSize:
		seqSetSize(frame->data[0]);
		eeprom_write_byte(EEPROM_SIZE_ADDRESS, frame->data[0]);
		break;
		
		case eSetLedsCount:
		numLeds = frame->data[0];
		eeprom_write_byte(EEPROM_LEDS_COUNT_ADDRESS, frame->data[0]);
		break;
		
		case eSetAll:
		seqSetFunction(frame->data[0]);
		seqSetSpeed(frame->data[1]);
		seqSetSoftness(frame->data[2]);
		seqSetSize(frame->data[3]);
		eeprom_write_byte(EEPROM_ANIMATION_ADDRESS, frame->data[0]);
		eeprom_write_byte(EEPROM_SPEED_ADDRESS, frame->data[1]);
		eeprom_write_byte(EEPROM_SOFTNESS_ADDRESS, frame->data[2]);
		eeprom_write_byte(EEPROM_SIZE_ADDRESS, frame->data[3]);
		break;
	}
}