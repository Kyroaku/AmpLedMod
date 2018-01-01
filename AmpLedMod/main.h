/*
 * main.h
 *
 * Created: 2017-12-25 21:23:04
 *  Author: Marcin Dziedzic
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#include <avr/io.h>
#include <util/delay.h>
#include <ctype.h>

#include "ws2812b.h"
#include "Bluetooth.h"
#include "sequences.h"

// ----------------------------------------------------------------------- DEFINES

#define MAX_LEDS	40				/**< Maximum number of ws2812b physical leds. */

typedef enum
{
	eSetAnimation = 0,
	eSetAnimationColors,
	eSetAnimationSpeed,
	eSetLedsCount
} command_t;

typedef struct 
{
	uint8_t command;				/**< Command for driver. @see command_t. */
	uint8_t data[MAX_LEDS*3+1];		/**< Parameters for command (MAX_LEDS*3+1) is the maximum length of data. */
} frame_t;

#endif /* MAIN_H_ */