#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include <avr/io.h>
#include <util/delay.h>
#include <ctype.h>
#include <avr/interrupt.h>

int8_t ringBufGetFrame(uint8_t *frame);

/**
 * Initializes USART with 8 bit size data, no parity, 1 bit stop.
 * @param baudrate Baudrate used to communicate.
 */
void bluetoothInit(unsigned int baudrate);

/**
 * Sends 1 byte via USART. Blocking function.
 * @param c Character to be sent.
 */
void bluetoothSend(unsigned char c);

/**
 * Sends string terminated by zero ('\0').
 * @param buf String to be sent.
 */
void bluetoothSendStr(const char *str);

/**
 * Receives 1 byte from USART. Blocking function.
 * @return Received character.
 */
unsigned char bluetoothRecv(void);

/**
 * Receives string terminated by zero, or with length specified by len and writes it to buf. Blocking function.
 * @param buf Buffer for received data.
 * @param len Maximum length of received data (in bytes).
 */
void bluetoothRecvStr(char *buf, int len);

/**
 * Receives data with length specified by len and writes it to buf. Blocking function.
 * @param buf Buffer for received data.
 * @param len Maximum length of received data (in bytes).
 */
void bluetoothRecvData(char *buf, int len);

/**
 * Sends AT commands to restore default bluetooth settings.
 */
void bluetoothSetATSettings(void);

#endif /* BLUETOOTH_H_ */