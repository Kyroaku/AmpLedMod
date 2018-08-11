#include "Bluetooth.h"

// ----------------------------------------------------------------------- Defines

#define RING_BUFFER_SIZE	128

#define RING_BUF_AT(i)		(ringBuffer[i%RING_BUFFER_SIZE])
#define RING_BUF_INC(x)		(x = (x+1)%RING_BUFFER_SIZE)

// ----------------------------------------------------------------------- Static declarations

static volatile uint8_t ringBuffer[RING_BUFFER_SIZE] = { 0 };
static volatile uint8_t ringBufferReadPos = 0;
static volatile uint8_t ringBufferWritePos = 0;
static volatile uint8_t ringBufferTmp = 0;

// ----------------------------------------------------------------------- Definitions

int8_t bluetoothGetFrame(void *frame, uint8_t max_len)
{
	uint8_t *data = frame;
	/* Calculate number of bytes in ring buffer. */
	int len = (int)ringBufferWritePos - ringBufferReadPos;
	if(len < 0) {
		len += RING_BUFFER_SIZE;
	}
	if(len < 2) {
		return 0;
	}
	
	/* We need to read first two bytes, to know length of the frame. */
	while(len >= 2)
	{
		/* Look for start byte. */
		if(RING_BUF_AT(ringBufferReadPos) == 0xFF) {
			/* Check if whole frame is in ring buffer. */
			if(len-2 >= RING_BUF_AT(ringBufferReadPos+1)) {
				break;
			} else {
				/* Frame is not received yet. */
				return 0;
			}
		} else {
			RING_BUF_INC(ringBufferReadPos);
			len--;
		}
	}
	
	len = RING_BUF_AT(ringBufferReadPos+1);
	RING_BUF_INC(ringBufferReadPos);
	RING_BUF_INC(ringBufferReadPos);
	if(len > max_len)
		return 0;
	
	uint8_t checksum = len;
	uint8_t tmp;
	for(int i = 0; i < len; i++)
	{
		tmp = RING_BUF_AT(ringBufferReadPos);
		checksum += tmp;
		data[i] = tmp;
		RING_BUF_INC(ringBufferReadPos);
	}
	if(checksum == RING_BUF_AT(ringBufferReadPos)) {
		RING_BUF_INC(ringBufferReadPos);
		return 1;
	} else {
		RING_BUF_INC(ringBufferReadPos);
		return 0;
	}
}

ISR(USART_RX_vect)
{
	ringBufferTmp = (ringBufferWritePos+1)%RING_BUFFER_SIZE;
	if(ringBufferTmp != ringBufferReadPos)
	{
		ringBuffer[ringBufferWritePos] = UDR0;
		ringBufferWritePos = ringBufferTmp;
	}
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