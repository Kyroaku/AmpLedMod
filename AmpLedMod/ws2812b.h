/*
 * ws2812b.h
 *
 * Created: 2017-12-25 21:19:54
 *  Author: Marcin Dziedzic
 */ 


#ifndef WS2812B_H_
#define WS2812B_H_

#include <avr/io.h>
#include <util/delay.h>
#include <ctype.h>

#define WS_DDR		(DDRD)
#define WS_PORT		(PORTD)
#define WS_BIT		(1<<PD3)

typedef struct
{
	uint_fast8_t b, r, g;
} rgb_t;

typedef union
{
	rgb_t rgb;
	uint_fast32_t val;
} color_t;

void wsInit();
void wsSend(color_t *leds, int num_leds);

#endif /* WS2812B_H_ */