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
typedef enum mode MODE;

static void run_dmc()
{
	// Mode selection is tricky
}

static MODE get_mode()
{
	uint8_t mode = 0;
	// Mode 0 pin is PD2
	// Mode 1 pin is PD3

	_delay_ms(30); // Let the pin voltage stabilise after turning on pullups
	
	mode |= (PIND & _BV(PIND2)) ? 1 : 0;
	mode |= (PIND & _BV(PIND3)) ? 2 : 0;

	return (MODE)mode;
}

static void setup_io()
{
	// LED pin as output
	DDRC |= _BV(DDC2);

	// Mode pins as input
	DDRD &= ~(_BV(DDD2) | _BV(DDD3));
	// with pullup
	PORTD |= _BV(PORTD2) | _BV(PORTD3);
}

static void signal_fault()
{
	while(true)	
	{
		_delay_ms(200);
		PINC |= _BV(PINC2);
		_delay_ms(200);
		PINC |= _BV(PINC2);
	}
}

static void signal_mode(MODE mode)
{
	int c = (int)mode + 1;

	while(c--)
	{
		_delay_ms(200);
		PINC |= _BV(PINC2);
		_delay_ms(200);
		PINC |= _BV(PINC2);
	}
}

int main()
{

	setup_io();

	x27_initialise();

	x27_test();
	
	//MODE mode = get_mode();
//
	//signal_mode(mode);
//
	//switch(mode)
	//{
	//case MODE_ADC:
	//	run_adc();
	//	break;
	//case MODE_SERIAL:
	//	run_serial();
	//	break;
	//case MODE_SPI:
	//	run_spi();
	//	break;
	//case MODE_PWM:
	//	run_pwm();
	//	break;
	//default:
	//	run_dmc();
	//	break;
	//}
//
	//// If execution gets to this point, something's gone very wrong
	//signal_fault();

	return 0;
}
