#include <stdbool.h>

#include <avr/io.h>
#include <util/delay.h>

#include "x27.h"

enum mode
{
	MODE_SPI,
	MODE_SERIAL,
	MODE_PWM,
	MODE_ADC
};

static void run_dmc()
{
	// Mode selection is tricky
}

static int get_mode()
{
	// Mode 0 pin is PD1
	// Mode 1 pin is PD2

	// Mode pins as input
	DDRD &= ~(_BV(DDD1) | _BV(DDD2));
	// with pullup
	PORTD |= _BV(PORTD1) | _BV(PORTD2);

	_delay_ms(30); // Let the pin voltage stabilise after turning on pullups
	return (enum mode)( (PIND & 0x02) | (PIND & 0x04));
}

static void signal_fault()
{
	DDRC |= _BV(DDC2);

	while(true)	
	{
		_delay_ms(200);
		PINC |= _BV(PINC2);
		_delay_ms(200);
		PINC |= _BV(PINC2);
	}
}

int main()
{

	x27_initialise();

	int mode = get_mode();
	
	switch(mode)
	{
	case MODE_ADC:
		run_adc();
		break;
	case MODE_SERIAL:
		run_serial();
		break;
	case MODE_SPI:
		run_spi();
		break;
	case MODE_PWM:
		run_pwm();
		break;
	default:
		run_dmc();
		break;
	}

	// If execution gets to this point, something's gone very wrong
	signal_fault();

	return 0;
}
