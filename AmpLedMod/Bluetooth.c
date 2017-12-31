#include "Bluetooth.h"

#define RING_BUFFER_SIZE	644

#define RING_BUF_AT(i)		(ringBuffer[i%RING_BUFFER_SIZE])

static uint8_t ringBuffer[RING_BUFFER_SIZE];
static uint8_t ringBufferReadPos = 0;
static uint8_t ringBufferWritePos = 0;
static uint8_t ringBufferTmp = 0;

ISR(USART_RX_vect)
{
	ringBufferTmp = (ringBufferWritePos+1)%RING_BUFFER_SIZE;
	if(ringBufferTmp != ringBufferReadPos)
	{
		ringBuffer[ringBufferWritePos] = UDR0;
		ringBufferWritePos = ringBufferTmp;
	}
}

int8_t ringBufGetFrame(uint8_t *frame)
{
	int len = (int)ringBufferWritePos - ringBufferReadPos;
	if(len < 0)
		len += RING_BUFFER_SIZE;
	if(len >= 6 &&
		RING_BUF_AT(ringBufferReadPos+1) == 0 &&
		RING_BUF_AT(ringBufferReadPos+3) == 0 &&
		RING_BUF_AT(ringBufferReadPos+5) == 0)
	{
		frame[0] = RING_BUF_AT(ringBufferReadPos+0);
		frame[1] = RING_BUF_AT(ringBufferReadPos+2);
		frame[2] = RING_BUF_AT(ringBufferReadPos+4);
		ringBufferReadPos = (ringBufferReadPos+6) % RING_BUFFER_SIZE;
		return 1;
	}
	else
		return 0;
}

void bluetoothInit(unsigned int baudrate)
{
	/* Set baudrate. */
	unsigned int ubrr = (F_CPU / 16 / baudrate - 1);
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)(ubrr);
	
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0); /* Enable USART RX, TX, interrupt for RX. */
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01); /* Set USART 8bit size data. */
	
	sei();
}

void bluetoothSend(unsigned char c)
{
	while(!(UCSR0A&(1<<UDRE0))) {}
	UDR0 = c;
}

void bluetoothSendStr(const char *buf)
{
	while(*buf)
		bluetoothSend(*buf++);
}

unsigned char bluetoothRecv(void)
{
	while(!(UCSR0A&(1<<RXC0))) {}
	return UDR0;
}

void bluetoothRecvStr(char *buf, int len)
{
	for(int i = 0; *buf && i<len; i++)
		*buf++ = bluetoothRecv();
}

void bluetoothRecvData(char *buf, int len)
{
	for(int i = 0; i<len; i++)
		*buf++ = bluetoothRecv();
}

void bluetoothSetATSettings(void)
{
	bluetoothSendStr("AT+ORGL\r\n");
	_delay_ms(100);
	bluetoothSendStr("AT+UART=115200,0,0\r\n");
	_delay_ms(100);
	bluetoothSendStr("AT+PSWD=7666\r\n");
	_delay_ms(100);
	bluetoothSendStr("AT+NAME=Sasha115200\r\n");
	_delay_ms(100);
}