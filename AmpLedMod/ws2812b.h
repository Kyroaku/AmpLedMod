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

// ----------------------------------------------------------------------- Defines

#define WS_DDR		(DDRD)
#define WS_PORT		(PORTD)
#define WS_BIT		(8)

// ----------------------------------------------------------------------- Structures

typedef struct
{
	uint8_t b, r, g;
} rgb_t;

typedef union
{
	rgb_t rgb;
	uint32_t val;
} color_t;

// ----------------------------------------------------------------------- Declarations

void wsInit();
void wsSend(color_t *leds, const int num_leds);

#endif /* WS2812B_H_ */