/*
 * LcdConfig.h
 *
 * Created: 2018-04-07 12:45:26
 *  Author: Marcin Dziedzic
 */ 


#ifndef LCDCONFIG_H_
#define LCDCONFIG_H_

/* ------------------------------------------------------ Data bits.

 * To enable 4-bit data mode, define LCD_DATA_8_BIT_MODE as a '0' value.
 * To enable 8-bit data mode, define LCD_DATA_8_BIT_MODE as a '1' value.
 */
#define LCD_8_BIT_DATA_MODE		1	/**< Specifies number of bits used in communication (4 or 8 bits). */

/* ------------------------------------------------------ Control pins.

 * All control pins must be on the same port.
 * Besides that, there is no specified rules for choosing pins.
 *
 * Note: LCD_XX is a pin number, for example: PC0.
 */
#define LCD_DDR_C		DDRC		/**< DDRX for LCD control pins. */
#define LCD_PORT_C		PORTC		/**< PORTX for LCD control pins. */ 
#define LCD_PIN_C		PINC		/**< PINX for LCD control pins. */

#define LCD_RS			PC0			/**< LCD register select (RS) pin number. */
#define LCD_E			PC1			/**< LCD operation enable (E) pin number. */

/* ------------------------------------------------------ Data pins.

 * All data pins must be on the same port.
 * Order and location of data pins can be arbitrary (data hard mode), but can also
 * affect performance. Best performance is reached, when data pins
 * are placed next to each other, with ascending pin's number (data simple mode). For example:
 * LCD_DB4 - PC2
 * LCD_DB5 - PC3
 * LCD_DB6 - PC4
 * LCD_DB7 - PC5
 *
 * Note: LCD_DBX is a pin number, for example: PC0.
 */
#define LCD_DDR_D		DDRD		/**< DDRX for LCD data pins. */
#define LCD_PORT_D		PORTD		/**< PORTX for LCD data pins. */
#define LCD_PIN_D		PIND		/**< PINX for LCD data pins. */

#define LCD_DB0			PD0			/**< LCD data bit 0 (DB0) pin number. Not used in 4 bit mode. */
#define LCD_DB1			PD1			/**< LCD data bit 1 (DB1) pin number. Not used in 4 bit mode. */
#define LCD_DB2			PD2			/**< LCD data bit 2 (DB2) pin number. Not used in 4 bit mode. */
#define LCD_DB3			PD3			/**< LCD data bit 3 (DB3) pin number. Not used in 4 bit mode. */
#define LCD_DB4			PD4			/**< LCD data bit 4 (DB4) pin number. */
#define LCD_DB5			PD5			/**< LCD data bit 5 (DB5) pin number. */
#define LCD_DB6			PD6			/**< LCD data bit 6 (DB6) pin number. */
#define LCD_DB7			PD7			/**< LCD data bit 7 (DB7) pin number. */


#endif /* LCDCONFIG_H_ */