#include <avr/io.h>
#include <avr/interrupt.h>

#include "x27.h"

static const uint16_t BAUDRATE_CONFIG_9600 = (F_CPU / (4 * 9599U)) / 2;

static char s_command_buffer[16];
static int s_buffer_index = 0;
static bool s_new_command_ready = false;

static void handle_command(char const * buffer, struct position_command * command)
{
	uint16_t val = 0;
	
	while (*buffer >= '0' && *buffer <= '9')
	{
		val += *buffer - '0';
		val *= 10;
		buffer++;
	}

	command->position = val / 10;
	command->type = *buffer == '%' ? POSITION_TYPE_PERCENT : POSITION_TYPE_STEPS;
}

static void setup()
{
	// Enable U2X mode
	UCSR0A = _BV(U2X0);
	// Enable rx and tx interrupts, enable transmitter and receiver
	UCSR0B = _BV(RXCIE0) | _BV(TXCIE0) | _BV(RXEN0) | _BV(TXEN0);

	// Set 8N1, no parity
	UCSR0C = _BV(USBS0) | _BV(UCSZ01) | _BV(UCSZ00);

	// Set 9600 baud
	UBRR0H = BAUDRATE_CONFIG_9600 >> 8;
	UBRR0L = BAUDRATE_CONFIG_9600 & 0xFF;
}

void run_serial()
{
	position_command cmd;

	setup();

	while(true)
	{
		if (s_new_command_ready)
		{
			handle_command(s_command_buffer, &cmd);
			x27_set_position(&cmd);
			s_buffer_index = 0;
			s_new_command_ready = false;
		}

		x27_service();
		
	}
}

ISR(USART0_RX_vect)
{
	if (s_new_command_ready) { return; }

	char c = UDR0;
	if (s_buffer_index < 12)
	{
		s_command_buffer[s_buffer_index++] = c;
	}
	else if (c == '\n')
	{
		s_new_command_ready = true;
	}
}

ISR(USART0_TX_vect)
{

}
