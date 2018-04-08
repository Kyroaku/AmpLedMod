/*
* Lcd.c
*
* Created: 2018-04-03 00:21:06
*  Author: Marcin Dziedzic
*/

#include "Lcd.h"

// ----------------------------------------------------------------------- Defines

#if LCD_8_BIT_DATA_MODE == 0
#if ( (LCD_DB4 == LCD_DB5-1) && (LCD_DB5 == LCD_DB6-1) && (LCD_DB6 == LCD_DB7-1) )
#define LCD_4_BIT_SIMPLE_MODE
#else
#error 4-bit hard data mode not implemented. 4-bit simple mode must be used. See 'Data pins' section in LcdConfig.h.
#endif

#endif

#define LCD_RS_BIT		(1<<LCD_RS)
#define LCD_E_BIT		(1<<LCD_E)

#define LCD_DB0_BIT		(1<<LCD_DB0)
#define LCD_DB1_BIT		(1<<LCD_DB1)
#define LCD_DB2_BIT		(1<<LCD_DB2)
#define LCD_DB3_BIT		(1<<LCD_DB3)
#define LCD_DB4_BIT		(1<<LCD_DB4)
#define LCD_DB5_BIT		(1<<LCD_DB5)
#define LCD_DB6_BIT		(1<<LCD_DB6)
#define LCD_DB7_BIT		(1<<LCD_DB7)

#if LCD_8_BIT_DATA_MODE == 0
#define LCD_DB_MASK	(LCD_DB4_BIT | LCD_DB5_BIT | LCD_DB6_BIT | LCD_DB7_BIT)
#else
#define LCD_DB_MASK	(LCD_DB0_BIT | LCD_DB1_BIT | LCD_DB2_BIT | LCD_DB3_BIT | LCD_DB4_BIT | LCD_DB5_BIT | LCD_DB6_BIT | LCD_DB7_BIT)
#endif

#define LCD_FLUSH	{	\
	LCD_PORT_C |= LCD_E_BIT;	\
	_delay_ms(1);		\
	LCD_PORT_C &= ~LCD_E_BIT;	\
	_delay_ms(1);		\
}

// ----------------------------------------------------------------------- Static declarations

static void lcdSend(uint8_t data);

// ----------------------------------------------------------------------- Static definitions

void lcdSend(uint8_t data)
{
	#if LCD_8_BIT_DATA_MODE == 0
	/* For 4-bit simple mode. */
	LCD_PORT_D &= ~LCD_DB_MASK;
	LCD_PORT_D |= ((data>>4) & 0x0F) << LCD_DB4;
	LCD_FLUSH;
	LCD_PORT_D &= ~LCD_DB_MASK;
	LCD_PORT_D |= (data & 0x0F) << LCD_DB4;
	LCD_FLUSH;
	#else
	/* For 8-bit simple mode. */
	LCD_PORT_D = data;
	LCD_FLUSH;
	#endif
}

// ----------------------------------------------------------------------- Definitions

void lcdInit()
{
	LCD_DDR_D |= LCD_DB_MASK;
	LCD_DDR_C |= LCD_RS_BIT | LCD_E_BIT;
	
	/* Initialize LCD. */
	LCD_PORT_D &= ~(LCD_DB_MASK);
	LCD_PORT_C &= ~LCD_RS_BIT;
	LCD_PORT_D |= (LCD_8_BIT_DATA_MODE << LCD_DB4) | LCD_DB5_BIT;
	LCD_FLUSH;
	_delay_ms(5);
	LCD_FLUSH;
	_delay_us(200);
	LCD_FLUSH;
	_delay_us(200);
}

void lcdCommand(uint8_t cmd)
{
	LCD_PORT_C &= ~LCD_RS_BIT;
	lcdSend(cmd);
}

void lcdClear()
{
	lcdCommand(LCD_CMD_CLEAR);
	_delay_ms(2);
}

void lcdReturnHome()
{
	lcdCommand(LCD_CMD_RETURN_HOME);
	_delay_ms(2);
}

void lcdSetEntryMode(ELcdDirection dir, ELcdState shift)
{
	lcdCommand(LCD_CMD_ENTRY_MODE | (dir<<1) | shift);
	_delay_us(60);
}

void lcdSetDisplay(ELcdState disp_on, ELcdState cursor_on, ELcdState blink_on)
{
	lcdCommand(LCD_CMD_DISPLAY | (disp_on<<2) | (cursor_on<<1) | blink_on);
	_delay_us(60);
}

void lcdShift(ELcdDirection dir, ELcdState shift_disp)
{
	lcdCommand(LCD_CMD_SHIFT | (shift_disp<<3) | (dir<<2));
	_delay_us(60);
}

void lcdFunctionSet(ELcdDataBits bits8, ELcdLines line2, ELcdFont font)
{
	lcdCommand(LCD_CMD_FUNCTION_SET | (bits8<<4) | (line2<<3) | (font<<2));
	_delay_us(60);
}

void lcdPrintChar(const char c)
{
	LCD_PORT_C |= LCD_RS_BIT;
	lcdSend(c);
	_delay_us(60);
}

void lcdPrintStr(const char *str)
{
	while(*str) {
		lcdPrintChar(*str++);
	}
}