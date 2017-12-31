/*
 * ws2812b.c
 *
 * Created: 2017-12-25 21:19:44
 *  Author: Marcin Dziedzic
 */ 

#include "ws2812b.h"

// ------------------------------------------------------------------- DEFINITIONS

void wsInit()
{
	WS_DDR |= WS_BIT;
}

void wsSend(color_t *leds, int num_leds)
{
	for(uint8_t j = 0; j < num_leds; j++)
	{
		for(uint8_t i = 0; i < 24; i++)
		{
			if(leds[j].val & ((uint_fast32_t)1<<(23-i))) {
				WS_PORT |= WS_BIT;
				WS_PORT |= WS_BIT;
				WS_PORT |= WS_BIT;
				WS_PORT |= WS_BIT;
				WS_PORT |= WS_BIT;
				//WS_PORT |= WS_BIT;
				
				//WS_PORT &= ~WS_BIT;
				//WS_PORT &= ~WS_BIT;
				WS_PORT &= ~WS_BIT;
				} else {
				//WS_PORT |= WS_BIT;
				//WS_PORT |= WS_BIT;
				WS_PORT |= WS_BIT;
				
				WS_PORT &= ~WS_BIT;
				WS_PORT &= ~WS_BIT;
				WS_PORT &= ~WS_BIT;
				WS_PORT &= ~WS_BIT;
				WS_PORT &= ~WS_BIT;
				//WS_PORT &= ~WS_BIT;
			}
		}
	}
	WS_PORT = 0;
}