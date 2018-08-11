/*
* sequences.c
*
* Created: 2017-12-26 14:31:16
*  Author: Marcin Dziedzic
*/

#include "sequences.h"

#include <math.h>

// ----------------------------------------------------------------------- Defines

// ----------------------------------------------------------------------- Static declarations

static void seqPingPongFunc(color_t *leds, const int num_leds);
static void seqDoublePingPongFunc(color_t *leds, const int num_leds);
static void seqFadeFunc(color_t *leds,const  int num_leds);
static void seqSoftSwitchFunc(color_t *leds, const int num_leds);
static void seqStaticColor(color_t *leds, const int num_leds);
static void seqRunning2(color_t *leds, const int num_leds);
static void seqParticles(color_t *leds, const int num_leds);
static void seqParticlesDark(color_t *leds, const int num_leds);
static void seqRunningParticle(color_t *leds, const int num_leds);

static uint8_t seqSpeed = 50;			/**< Sequence speed: 0-100. */
static uint8_t seqSoftness = 3;		/**< Sequence softness: 0-100. */
static uint8_t seqSize = 2;			/**< Sequence size: 0-20. */
static uint8_t numColors = 7;			/**< Number of colors in sequences: 0-SEQ_MAX_COLORS. */

/**
* Array of colors used in animations.
*/
static uint32_t colors[SEQ_MAX_COLORS] = {
	0x00FF00, 0x00FFFF, 0x0000FF, 0xFFFF00, 0xFF0000, 0xFF00FF, 0xFFFFFF
};

/**
* Array of animation algorithm functions.
*/
static seqFunction_t functions[eSeqCount] = {
	seqPingPongFunc,
	seqDoublePingPongFunc,
	seqFadeFunc,
	seqSoftSwitchFunc,
	seqStaticColor,
	seqRunning2,
	seqParticles,
	seqParticlesDark,
	seqRunningParticle
};

static seqType_t seqType = eSeqRunningParticle;	/**< Current animation type. */

// ----------------------------------------------------------------------- Definitions

void seqSetFunction(seqType_t type)
{
	seqType = type;
}

void seqSetSpeed(uint8_t speed)
{
	if(speed > SEQ_MAX_SPEED) seqSpeed = SEQ_MAX_SPEED;
	else if(speed < SEQ_MIN_SPEED) seqSpeed = SEQ_MIN_SPEED;
	else seqSpeed = speed;
}

void seqSetSoftness(uint8_t softness)
{
	if(softness > SEQ_MAX_SOFTNESS) seqSoftness = SEQ_MAX_SOFTNESS;
	else if(softness < SEQ_MIN_SOFTNESS) seqSoftness = SEQ_MIN_SOFTNESS;
	else seqSoftness = softness;
}

void seqSetSize(uint8_t size)
{
	if(size > SEQ_MAX_SIZE) seqSize = SEQ_MAX_SIZE;
	else if(size < SEQ_MIN_SIZE) seqSize = SEQ_MIN_SIZE;
	seqSize = size;
}

void seqUpdate(color_t *leds, int num_leds)
{
	functions[seqType](leds, num_leds);
}

void seqSetColor(uint8_t i, uint32_t c)
{
	if(i >= numColors)
	return;
	
	colors[i] = c;
}

void seqSetColorRGB(uint8_t i, uint8_t r, uint8_t g, uint8_t b)
{
	if(i >= numColors)
	return;
	
	colors[i] = (int32_t)r | ((int32_t)g<<8) | ((int32_t)b<<16);
}

void seqSetColors(uint32_t *c, uint8_t num)
{
	if(num > SEQ_MAX_COLORS)
	return;
	
	/* Copy colors to specified sequence. */
	for(int i = 0; i < numColors; i++)
	colors[i] = c[i];
	/* Set new colors count. */
	numColors = num;
}

void seqSetColorCount(uint8_t num)
{
	if(num > SEQ_MAX_COLORS)
	return;
	
	numColors = num;
}

// ----------------------------------------------------------------------- Static definitions

static void seqPingPongFunc(color_t *leds, const int num_leds)
{
	static float pos = 0;
	static int8_t led_dir = 0;
	static uint8_t color_i = 0;
	
	uint8_t softness = seqSoftness;
	float speed = seqSpeed / 50.0f;
	
	for(int i = 0; i < num_leds; i++)
	{
		float k;
		if(!led_dir) {
			if(i < pos) k = 1.0f;
			else if(i < pos + softness) k = 1.0f - (i-pos) / (softness);
			else k = 0.0f;
		}
		else {
			if(i >= pos) k = 1.0f;
			else if(i >= pos - softness) k = 1.0f - (pos-i) / (softness);
			else k = 0.0f;
		}
		
		//k=k*k;
		uint32_t color0 = colors[color_i];
		uint32_t color1 = colors[color_i+1 < numColors ? color_i+1 : 0];
		leds[i].rgb.r = k*((color1>>0)&0xFF) + (1.0f-k)*((color0>>0)&0xFF);
		leds[i].rgb.g = k*((color1>>8)&0xFF) + (1.0f-k)*((color0>>8)&0xFF);
		leds[i].rgb.b = k*((color1>>16)) + (1.0f-k)*((color0>>16));
	}
	
	if(!led_dir) pos += speed;
	else pos -= speed;
	if(pos >= num_leds + softness || pos < -softness) {
		color_i = (color_i+1)%numColors;
		led_dir = led_dir == 1 ? 0 : 1;
		if(!led_dir) pos = -softness;
		else pos = num_leds + softness - 1;
	}
}

static void seqDoublePingPongFunc(color_t *leds, const int num_leds)
{
	static float pos = 0;
	static int8_t led_dir = 0;
	static uint8_t color_i = 0;
	
	uint8_t softness = seqSoftness;
	float speed = seqSpeed / 100.0f;
	
	for(int i = 0; i < num_leds / 2 + 1; i++)
	{
		float k;
		if(!led_dir) {
			if(i < pos) k = 1.0f;
			else if(i < pos + softness) k = 1.0f - (i-pos) / (softness);
			else k = 0.0f;
		}
		else {
			if(i >= pos) k = 1.0f;
			else if(i >= pos - softness) k = 1.0f - (pos-i) / (softness);
			else k = 0.0f;
		}
		
		leds[i].rgb.r = k*((colors[(color_i+1)%numColors]>>0)&0xFF) + (1.0f-k)*((colors[color_i]>>0)&0xFF);
		leds[i].rgb.g = k*((colors[(color_i+1)%numColors]>>8)&0xFF) + (1.0f-k)*((colors[color_i]>>8)&0xFF);
		leds[i].rgb.b = k*((colors[(color_i+1)%numColors]>>16)&0xFF) + (1.0f-k)*((colors[color_i]>>16)&0xFF);
		
		leds[num_leds-i-1].rgb.r = k*((colors[(color_i+1)%numColors]>>0)&0xFF) + (1.0f-k)*((colors[color_i]>>0)&0xFF);
		leds[num_leds-i-1].rgb.g = k*((colors[(color_i+1)%numColors]>>8)&0xFF) + (1.0f-k)*((colors[color_i]>>8)&0xFF);
		leds[num_leds-i-1].rgb.b = k*((colors[(color_i+1)%numColors]>>16)&0xFF) + (1.0f-k)*((colors[color_i]>>16)&0xFF);
	}
	
	if(!led_dir) pos += speed;
	else pos -= speed;
	if(pos >= (num_leds/2+1) + softness || pos < -softness) {
		color_i = (color_i+1)%numColors;
		led_dir = led_dir == 1 ? 0 : 1;
		if(!led_dir) pos = -softness;
		else pos = (num_leds/2+1) + softness - 1;
	}
}

static void seqFadeFunc(color_t *leds, const int num_leds)
{
	static uint8_t color_i = 0;
	static int8_t led_dir = 0;
	static float k = 0.0f;
	
	float speed = seqSpeed / 5000.0f;
	
	for(int i = 0; i < num_leds; i++)
	{
		leds[i].rgb.r = k*((colors[color_i]>>0)&0xFF);
		leds[i].rgb.g = k*((colors[color_i]>>8)&0xFF);
		leds[i].rgb.b = k*((colors[color_i]>>16)&0xFF);
	}
	
	if(!led_dir) k+=speed;
	else k-=speed;
	if(k >= 1.0f) {
		led_dir = 1;
		k = 1.0f;
	}
	if(k <= 0.0f && led_dir) {
		color_i = (color_i+1)%numColors;
		led_dir = 0;
		k = 0.0f;
	}
}

static void seqSoftSwitchFunc(color_t *leds, const int num_leds)
{
	static uint8_t color_i = 0;
	static float k = 0.0f;
	
	float speed = seqSpeed / 5000.0f;
	
	for(int i = 0; i < num_leds; i++)
	{
		leds[i].rgb.r = (1.0f-k)*((colors[color_i]>>0)&0xFF) + k*((colors[(color_i+1)%numColors]>>0)&0xFF);
		leds[i].rgb.g = (1.0f-k)*((colors[color_i]>>8)&0xFF) + k*((colors[(color_i+1)%numColors]>>8)&0xFF);
		leds[i].rgb.b = (1.0f-k)*((colors[color_i]>>16)&0xFF) + k*((colors[(color_i+1)%numColors]>>16)&0xFF);
	}
	
	k += speed;
	if(k >= 1.0f) {
		color_i = (color_i+1)%numColors;
		k = 0;
	}
}

static void seqStaticColor(color_t *leds, const int num_leds)
{
	for(int i = 0; i < num_leds; i++)
	{
		leds[i].rgb.r = colors[0]&0xFF;
		leds[i].rgb.g = (colors[0]>>8)&0xFF;
		leds[i].rgb.b = (colors[0]>>16)&0xFF;
	}
}

static void seqRunning2(color_t *leds, const int num_leds)
{
	static float pos = 0;
	
	float speed = seqSpeed / 500.0f;
	float size = seqSize;
	
	for(int i = 0; i < num_leds; i++)
	{
		float a = i*(1.0f/size)+pos;
		float k = (sin(a)+1.0f) * 0.5f;
		
		leds[i].rgb.r = k*((colors[0]>>0)&0xFF) +  (1.0f-k)*((colors[1]>>0)&0xFF);
		leds[i].rgb.g = k*((colors[0]>>8)&0xFF) +  (1.0f-k)*((colors[1]>>8)&0xFF);
		leds[i].rgb.b = k*((colors[0]>>16)&0xFF) +  (1.0f-k)*((colors[1]>>16)&0xFF);
	}
	
	pos += speed;
	if(pos > 2*M_PI)
	pos -= 2*M_PI;
}

static void seqParticles(color_t *leds, const int num_leds)
{
	static float pos = 0;
	static int length = 0;
	static int color_id = 0;
	static uint8_t decremented = 0;
	
	float speed = seqSpeed / 50.0f;
	float size = seqSize;
	
	for(int i = 0; i < length; i++) {
		float dist = pos-i;
		dist = dist < 0 ? -dist : dist;
		if(dist < size) {
			float k = dist / size;
			leds[i].rgb.r = k*((colors[color_id]>>0)&0xFF) + (1.0f-k)*((colors[(color_id+1)%numColors]>>0)&0xFF);
			leds[i].rgb.g = k*((colors[color_id]>>8)&0xFF) + (1.0f-k)*((colors[(color_id+1)%numColors]>>8)&0xFF);
			leds[i].rgb.b = k*((colors[color_id]>>16)&0xFF) + (1.0f-k)*((colors[(color_id+1)%numColors]>>16)&0xFF);
			} else {
			leds[i].rgb.r = ((colors[color_id]>>0)&0xFF);
			leds[i].rgb.g = ((colors[color_id]>>8)&0xFF);
			leds[i].rgb.b = ((colors[color_id]>>16)&0xFF);
		}
	}
	
	for(int i = length; i < num_leds; i++) {
		leds[i].rgb.r = ((colors[(color_id+1)%numColors]>>0)&0xFF);
		leds[i].rgb.g = ((colors[(color_id+1)%numColors]>>8)&0xFF);
		leds[i].rgb.b = ((colors[(color_id+1)%numColors]>>16)&0xFF);
	}
	
	pos += speed;
	if(pos >= length) {
		if(!decremented) {
			length--;
			decremented = 1;
		}
		if(pos >= length+size) {
			pos = -size;
			decremented = 0;
			
			if(length <= 0) {
				length = num_leds;
				color_id = (color_id+1) % numColors;
			}
		}
	}
}

static void seqParticlesDark(color_t *leds, const int num_leds)
{
	static float pos = 0;
	static int length = 0;
	static int color_id = 0;
	static uint8_t decremented = 0;
	
	float speed = seqSpeed / 50.0f;
	float size = seqSize;
	
	for(int i = 0; i < length; i++) {
		float dist = pos-i;
		dist = dist < 0 ? -dist : dist;
		if(dist < size) {
			float k = dist / size;
			leds[i].rgb.r = (1.0f-k)*((colors[(color_id+1)%numColors]>>0)&0xFF);
			leds[i].rgb.g = (1.0f-k)*((colors[(color_id+1)%numColors]>>8)&0xFF);
			leds[i].rgb.b = (1.0f-k)*((colors[(color_id+1)%numColors]>>16)&0xFF);
		}
		else {
			leds[i].rgb.r = 0;
			leds[i].rgb.g = 0;
			leds[i].rgb.b = 0;
		}
	}
	
	for(int i = length; i < num_leds; i++) {
		leds[i].rgb.r = ((colors[(color_id+1)%numColors]>>0)&0xFF);
		leds[i].rgb.g = ((colors[(color_id+1)%numColors]>>8)&0xFF);
		leds[i].rgb.b = ((colors[(color_id+1)%numColors]>>16)&0xFF);
	}
	
	pos += speed;
	if(pos >= length) {
		if(!decremented) {
			length--;
			decremented = 1;
		}
		if(pos >= length+size) {
			pos = -size;
			decremented = 0;
			
			if(length <= 0) {
				length = num_leds;
				color_id = (color_id+1) % numColors;
			}
		}
	}
}

static void seqRunningParticle(color_t *leds, const int num_leds)
{
	static float pos = 0;
	static uint8_t color_id = 0;
	
	float speed = seqSpeed / 1000.0f;
	uint8_t size = seqSize;
	uint8_t softness = seqSoftness;
	
	for(int i = 0; i < num_leds; i++)
	{
		float k;
		float d = i - pos;
		float d2 = i + num_leds - pos;
		d = d < 0.0f ? -d : d;
		if(d < size || d2 < size) k = 1.0f;
		else if(d < size + softness) k = (size + softness - d) / (float)softness;
		else if(d2 < size + softness) k = (size + softness - d2) / (float)softness;
		else k = 0.0f;
		
		leds[i].rgb.r = k*((colors[color_id]>>0)&0xFF);
		leds[i].rgb.g = k*((colors[color_id]>>8)&0xFF);
		leds[i].rgb.b = k*((colors[color_id]>>16)&0xFF);
	}
	
	pos += speed;
	if(pos >= num_leds + size + softness)
	pos = size + softness;
}