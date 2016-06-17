#include <avr/io.h>
#include <avr/interrupt.h>

#include "x27.h"

#define SPI_SS_PIN PORTB2
#define SPI_SCK_PIN PORTB5
#define SPI_MOSI_PIN PORTB3
#define SPI_MISO_PIN PORTB4

static char s_buffer[3];
static int s_buffer_index;
static bool s_new_command_ready = false;

static void setup()
{
	PORTB = _BV(SPI_MISO_PIN);
	SPCR |= _BV(SPIE) | _BV(SPE);
}

void run_spi()
{
	position_command cmd; 
	while(true)
	{
		if (s_new_command_ready)
		{
			cmd.position = (s_buffer[1] << 8 & s_buffer[2]);
			cmd.type = s_buffer[0] & 0x80 ? POSITION_TYPE_PERCENT : POSITION_TYPE_STEPS;
			x27_set_position(&cmd);
			s_buffer_index = 0;
			s_new_command_ready = false;
		}
		x27_service();
	}
}

ISR(SPI_STC_vect)
{
	if (s_new_command_ready) { return; }

	s_buffer[s_buffer_index++] = SPDR;

	if (s_buffer_index == 3)
	{
		s_new_command_ready = true;
	}
}
