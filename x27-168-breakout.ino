#include <avr/io.h>
#include <util/delay.h>

#include "x27.h"

static void run_dmc()
{
	// Mode selection is tricky
}

static void signal_fault()
{
	DDRB |= _BV(DDB2);

	while(true)	
	{
		_delay_ms(200);
		PINB |= _BV(PINB2);
		_delay_ms(200);
		PINB |= _BV(PINB2);
	}
}

void setup()
{

	x27_initialise();

	// Mode pins as input, no pullup
	DDRB &= ~(_BV(DDB0) | _BV(DDB1));
	PORTB &= ~(_BV(PORTB0) | _BV(PORTB1));

	int mode = (PINB & 0x01) & (PINB & 0x02);

	switch(mode)
	{
	case 0:
		run_adc();
		break;
	case 1:
		run_serial();
		break;
	case 2:
		run_spi();
		break;
	case 3:
		run_pwm();
		break;
	default:
		run_dmc();
		break;
	}

	// If execution gets to this point, something's gone very wrong
	signal_fault();
}

void loop()
{
	
}