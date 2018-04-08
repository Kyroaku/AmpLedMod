/*
* sequences.c
*
* Created: 2017-12-26 14:31:16
*  Author: Marcin Dziedzic
*/

#include "sequences.h"

// ----------------------------------------------------------------------- Defines

// ----------------------------------------------------------------------- Static declarations

static void seqPingPongFunc(color_t *leds, int num_leds);
static void seqDoublePingPongFunc(color_t *leds, int num_leds);
static void seqFadeFunc(color_t *leds, int num_leds);
static void seqSoftSwitchFunc(color_t *leds, int num_leds);
static void seqStaticColor(color_t *leds, int num_leds);

static uint8_t numColors = 7;
static uint32_t colors[SEQ_MAX_COLORS] = {
	0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0xFF00FF, 0x00FFFF, 0xFFFFFF
};
static seqFunction_t functions[eSeqCount] = {
	seqPingPongFunc,
	seqDoublePingPongFunc,
	seqFadeFunc,
	seqSoftSwitchFunc,
	seqStaticColor
};
static seqType_t seqType = eSeqPingPong;

// ----------------------------------------------------------------------- Definitions

void seqSetFunction(seqType_t type)
{
	seqType = type;
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
	
	color_t color;
	color.rgb.r = r;
	color.rgb.b = b;
	color.rgb.g = g;
	colors[i] = color.val;
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

static void seqPingPongFunc(color_t *leds, int num_leds)
{
	static int8_t led_i = 0;
	static int8_t led_dir = 0;
	static uint8_t color_i = 0;
	static uint8_t timer = 0;
	static uint8_t softness = 10;
	
	timer++;
	if(timer < 2) return;
	else timer = 0;
	
	//leds[led_i].val = colors[seq_i];
	
	for(int i = 0; i < softness; i++)
	{
		float k = (float)(i+1)/softness;
		if(!led_dir) {
			if(led_i-i < 0 || led_i-i >= num_leds) continue;
			leds[led_i-i].rgb.r = k*((colors[color_i]>>0)&0xFF) + (1.0f-k)*leds[led_i-i].rgb.r;
			leds[led_i-i].rgb.g = k*((colors[color_i]>>8)&0xFF) + (1.0f-k)*leds[led_i-i].rgb.g;
			leds[led_i-i].rgb.b = k*((colors[color_i]>>16)&0xFF) + (1.0f-k)*leds[led_i-i].rgb.b;
		}
		else {
			if(led_i+i >= num_leds || led_i+i < 0) continue;
			leds[led_i+i].rgb.r = k*((colors[color_i]>>0)&0xFF) + (1.0f-k)*leds[led_i+i].rgb.r;
			leds[led_i+i].rgb.g = k*((colors[color_i]>>8)&0xFF) + (1.0f-k)*leds[led_i+i].rgb.g;
			leds[led_i+i].rgb.b = k*((colors[color_i]>>16)&0xFF) + (1.0f-k)*leds[led_i+i].rgb.b;
		}
	}
	
	if(led_dir) led_i--;
	else led_i++;
	if(led_i >= num_leds + softness || led_i < -softness) {
		color_i = (color_i+1)%numColors;
		led_dir = led_dir == 1 ? 0 : 1;
		led_i = 0 + led_dir*num_leds-1;
	}
}

static void seqDoublePingPongFunc(color_t *leds, int num_leds)
{
	static int8_t led_i = 0;
	static int8_t led_dir = 0;
	static uint8_t color_i = 0;
	static uint8_t timer = 0;
	static uint8_t softness = 10;
	
	timer++;
	if(timer < 2) return;
	else timer = 0;
	
	//leds[led_i].val = colors[seq_i];
	//leds[num_leds-1-led_i].val = colors[seq_i];
	
	for(int i = 0; i < softness; i++)
	{
		float k = (float)(i+1)/softness;
		if(!led_dir) {
			if(led_i-i < 0 || led_i-i > num_leds/2) continue;
			leds[led_i-i].rgb.r = k*((colors[color_i]>>0)&0xFF) + (1.0f-k)*leds[led_i-i].rgb.r;
			leds[led_i-i].rgb.g = k*((colors[color_i]>>8)&0xFF) + (1.0f-k)*leds[led_i-i].rgb.g;
			leds[led_i-i].rgb.b = k*((colors[color_i]>>16)&0xFF) + (1.0f-k)*leds[led_i-i].rgb.b;
			
			leds[num_leds-1-led_i+i].rgb.r = k*((colors[color_i]>>0)&0xFF) + (1.0f-k)*leds[num_leds-1-led_i+i].rgb.r;
			leds[num_leds-1-led_i+i].rgb.g = k*((colors[color_i]>>8)&0xFF) + (1.0f-k)*leds[num_leds-1-led_i+i].rgb.g;
			leds[num_leds-1-led_i+i].rgb.b = k*((colors[color_i]>>16)&0xFF) + (1.0f-k)*leds[num_leds-1-led_i+i].rgb.b;
		}
		else {
			if(led_i+i > num_leds/2 || led_i+i < 0) continue;
			leds[led_i+i].rgb.r = k*((colors[color_i]>>0)&0xFF) + (1.0f-k)*leds[led_i+i].rgb.r;
			leds[led_i+i].rgb.g = k*((colors[color_i]>>8)&0xFF) + (1.0f-k)*leds[led_i+i].rgb.g;
			leds[led_i+i].rgb.b = k*((colors[color_i]>>16)&0xFF) + (1.0f-k)*leds[led_i+i].rgb.b;
			
			leds[num_leds-1-led_i-i].rgb.r = k*((colors[color_i]>>0)&0xFF) + (1.0f-k)*leds[num_leds-1-led_i-i].rgb.r;
			leds[num_leds-1-led_i-i].rgb.g = k*((colors[color_i]>>8)&0xFF) + (1.0f-k)*leds[num_leds-1-led_i-i].rgb.g;
			leds[num_leds-1-led_i-i].rgb.b = k*((colors[color_i]>>16)&0xFF) + (1.0f-k)*leds[num_leds-1-led_i-i].rgb.b;
		}
	}
	
	if(led_dir) led_i--;
	else led_i++;
	if(led_i >= num_leds/2 + softness || led_i < -softness) {
		color_i = (color_i+1)%numColors;
		led_dir = led_dir == 1 ? 0 : 1;
		led_i = 0 + led_dir*num_leds/2-1;
	}
}

static void seqFadeFunc(color_t *leds, int num_leds)
{
	static uint8_t color_i = 0;
	static uint8_t timer = 0;
	static uint8_t time = 50;
	static int8_t led_dir = 0;
	
	float k = (float)timer/time;
	for(int i = 0; i < num_leds; i++)
	{
		leds[i].rgb.r = k*((colors[color_i]>>0)&0xFF);
		leds[i].rgb.g = k*((colors[color_i]>>8)&0xFF);
		leds[i].rgb.b = k*((colors[color_i]>>16)&0xFF);
	}
	
	if(!led_dir)
	timer++;
	else
	timer--;
	if(timer >= time)
	led_dir = 1;
	if(timer <= 0 && led_dir) {
		color_i = (color_i+1)%numColors;
		led_dir = 0;
	}
}

static void seqSoftSwitchFunc(color_t *leds, int num_leds)
{
	static uint8_t color_i = 0;
	static uint8_t timer = 0;
	static uint8_t time = 50;
	
	float k = (float)timer/time;
	for(int i = 0; i < num_leds; i++)
	{
		leds[i].rgb.r = (1.0f-k)*((colors[color_i]>>0)&0xFF) + k*((colors[(color_i+1)%numColors]>>0)&0xFF);
		leds[i].rgb.g = (1.0f-k)*((colors[color_i]>>8)&0xFF) + k*((colors[(color_i+1)%numColors]>>8)&0xFF);
		leds[i].rgb.b = (1.0f-k)*((colors[color_i]>>16)&0xFF) + k*((colors[(color_i+1)%numColors]>>16)&0xFF);
	}
	
	timer++;
	if(timer > time) {
		color_i = (color_i+1)%numColors;
		timer = 0;
	}
}

static void seqStaticColor(color_t *leds, int num_leds)
{
	for(int i = 0; i < num_leds; i++)
	leds[i].val = colors[0];
}