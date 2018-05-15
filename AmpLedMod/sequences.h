/*
 * sequences.h
 *
 * Created: 2017-12-26 14:31:23
 *  Author: Marcin Dziedzic
 */ 


#ifndef SEQUENCES_H_
#define SEQUENCES_H_

#include "ws2812b.h"

// ----------------------------------------------------------------------- Defines

#define SEQ_MAX_COLORS			(16)		/**< Maximum number of colors in sequence. */
#define SEQ_MAX_SPEED			(100)		/**< Maximum animation speed value. */
#define SEQ_MIN_SPEED			(0)			/**< Minimum animation speed value. */
#define SEQ_MAX_SOFTNESS		(100)		/**< Maximum animation softness value. */
#define SEQ_MIN_SOFTNESS		(0)			/**< Minimum animation softness value. */
#define SEQ_MAX_SIZE			(20)		/**< Maximum animation size value. */
#define SEQ_MIN_SIZE			(0)			/**< Minimum animation size value. */

typedef void (*seqFunction_t)(color_t*, int);

// ----------------------------------------------------------------------- Structures

/**
 * Sequence animation types.
 */
typedef enum
{
	eSeqPingPong = 0,
	eSeqDoublePingPong,
	eSeqFade,
	eSeqSoftSwitch,
	eSeqStaticColor,
	eSeqRunning2,
	eSeqParticles,
	eSeqParticlesDark,
	eSeqCount /**< Specifies the number of animations. Must be at the end. */
} seqType_t;

// ----------------------------------------------------------------------- Declarations

/**
 * Sets new leds animation type.
 * @param type Leds animation type.
 */
void seqSetFunction(seqType_t type);

/**
 * Sets animation speed.
 * @param speed New animation speed.
 */
void seqSetSpeed(uint8_t speed);

/**
 * Sets animation softness.
 * @param softness New animation softness.
 */
void seqSetSoftness(uint8_t softness);

/**
 * Sets animation size.
 * @param size New animation size.
 */
void seqSetSize(uint8_t size);

/**
 * Updates array to animate leds.
 * Function animates colors in leds array in way specified by seqSetFunction() function. It is
 * recommended to call this function with ~10ms delay.
 * @param leds Array representing physical ws2812b colors.
 * @param num_leds Number of leds in array.
 */
void seqUpdate(color_t *leds, int num_leds);

/**
 * Sets color with the specified index in the specified sequence.
 * @param type Type of animation in which the color will be set.
 * @param i ID of the color that will be set.
 * @param color Value of the color to set.
 */
void seqSetColor(uint8_t i, uint32_t color);

/**
 * Sets color with the specified index in the specified sequence.
 * @param type Type of animation in which the color will be set.
 * @param i ID of the color that will be set.
 * @param r Value of red component of the color.
 * @param g Value of green component of the color.
 * @param b Value of blue component of the color.
 */
void seqSetColorRGB(uint8_t i, uint8_t r, uint8_t g, uint8_t b);

/**
 * Sets colors sequence in the specified animation.
 * @param type Type of the animation.
 * @param colors Colors sequence.
 * @param numColors Number of colors in the sequence.
 */
void seqSetColors(uint32_t *colors, uint8_t numColors);

/**
 * Sets number of colors in the specified sequence.
 * @param type Type of the animation.
 * @param numColors New number of colors;
 */
void seqSetColorCount(uint8_t numColors);


#endif /* SEQUENCES_H_ */