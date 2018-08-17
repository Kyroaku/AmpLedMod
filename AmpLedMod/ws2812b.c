/*
* ws2812b.c
*
* Created: 2017-12-25 21:19:44
*  Author: Marcin Dziedzic
*/

#include "ws2812b.h"

// ----------------------------------------------------------------------- Definitions

void wsInit()
{
	WS_DDR |= WS_BIT;
}

//#pragma GCC push_options
//#pragma GCC optimize ("O1")
void __attribute__((optimize("O1"))) wsSend(color_t *leds, const int num_leds)
{
	for(uint8_t j = 0; j < num_leds; j++)
	{
		for(uint8_t i = 0; i < 24; i++)
		{
			#if F_CPU == 8000000
			
			if(leds[j].val & ((uint_fast32_t)1<<(23-i))) {
				WS_PORT |= WS_BIT;
				_delay_loop_1(1);
				WS_PORT &= ~WS_BIT;
			}
			else {
				WS_PORT |= WS_BIT;
				WS_PORT &= ~WS_BIT;
				_delay_loop_1(1);
			}
			
			#elif F_CPU == 12000000
			
			if(leds[j].val & ((uint_fast32_t)1<<(23-i))) {
				WS_PORT |= WS_BIT;
				_delay_loop_1(3);
				WS_PORT &= ~WS_BIT;
				_delay_loop_1(1);
			}
			else {
				WS_PORT |= WS_BIT;
				_delay_loop_1(1);
				WS_PORT &= ~WS_BIT;
				_delay_loop_1(3);
			}
			
			#elif F_CPU == 24000000
			
			if(leds[j].val & ((uint_fast32_t)1<<(23-i))) {
				WS_PORT |= WS_BIT;
				_delay_loop_1(6);
				WS_PORT &= ~WS_BIT;
				_delay_loop_1(3);
			}
			else {
				WS_PORT |= WS_BIT;
				_delay_loop_1(3);
				WS_PORT &= ~WS_BIT;
				_delay_loop_1(6);
			}
			
			#endif
		}
	}
	WS_PORT = 0;
}
//#pragma GCC pop_options