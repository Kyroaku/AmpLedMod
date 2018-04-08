/*
* Lcd.h
*
* Created: 2018-04-03 00:21:13
*  Author: Marcin Dziedzic
*/

#include <avr/io.h>
#include <util/delay.h>

#include "LcdConfig.h"

#ifndef LCD_H_
#define LCD_H_

// ----------------------------------------------------------------------- Defines

/* Clear. */
#define LCD_CMD_CLEAR			0x01
/* Return home. */
#define LCD_CMD_RETURN_HOME		0x02
/* Entry mode set. */
#define LCD_CMD_ENTRY_MODE		0x04
#define LCD_INCREMENT			0x02
#define LCD_DECREMENT			0x00
#define LCD_SHIFT_ENABLE		0x01
/* Display on/off control. */
#define LCD_CMD_DISPLAY			0x08
#define LCD_DISPLAY_ENABLE		0x04
#define LCD_CURSOR_ENABLE		0x02
#define LCD_CURSOR_BLINKING		0x01
/* Cursor/display shift. */
#define LCD_CMD_SHIFT			0x10
#define LCD_SHIFT_LEFT			0x00
#define LCD_SHIFT_RIGHT			0x04
#define LCD_SHIFT
/* Function set. */
#define LCD_CMD_FUNCTION_SET	0x20

// ----------------------------------------------------------------------- Structures

typedef enum {
	eLcdLeft = 0,
	eLcdRight = 1
} ELcdDirection;

typedef enum {
	eDisabled = 0,
	eEnabled = 1
} ELcdState;

typedef enum {
	eLcd4Bits = 0,
	eLcd8Bits = 1
} ELcdDataBits;

typedef enum {
	eLcd1Line = 0,
	eLcd2Lines = 1
} ELcdLines;

typedef enum {
	eLcdFont5x8 = 0,
	eLcdFont5x11 = 1
} ELcdFont;

// ----------------------------------------------------------------------- Declarations

/**
* Initializes lcd display. Must be called before any operation on LCD.
*/
void lcdInit();
/**
* Sends command to LCD display.
* @param cmd Data to send in command mode.
*/
void lcdCommand(uint8_t cmd);
/**
* Clears LCD display and returns cursor to the home position.
*/
void lcdClear();
/**
* Returns cursor to the home position.
*/
void lcdReturnHome();
/**
* Sets cursor moving direction and enables or disables whole display
* shifting.
* @param dir	Specifies move direction of cursor.
* @param shift	Enables or disables shifting whole display.
*/
void lcdSetEntryMode(ELcdDirection dir, ELcdState shift);
/**
* Enables/disables display, cursor or cursor blinking.
* @param disp_on	Enables or disables whole display.
* @param cursor_on	Enables or disables cursor showing.
* @param blink_on	Enables or disables cursor blinking.
*/
void lcdSetDisplay(ELcdState disp_on, ELcdState cursor_on, ELcdState blink_on);
/**
* Shifts cursor or whole display.
* @param dir		Shifting direction.
* @param shift_disp	Specifies whether shift cursor only (eDisabled) or whole display (eEnabled).
*/
void lcdShift(ELcdDirection dir, ELcdState shift_disp);
/**
* Sets low level informations about display: number of data bits,
* number of dispay lines and font to use.
* eLcdFont5x11 font type can not be used with eLcd2Lines mode.
* @param bits8	Number of bits used in communication.
* @param line2	Number of lines on display.
* @param font	Specifies font  of the text.
*/
void lcdFunctionSet(ELcdDataBits bits8, ELcdLines line2, ELcdFont font);
/**
* Prints one character and increments cursor.
* @param c Character to print.
*/
void lcdPrintChar(const char c);
/**
* Prints string terminated by \0 code.
* @param str String terminated by \0 code.
*/
void lcdPrintStr(const char *str);

#endif /* LCD_H_ */