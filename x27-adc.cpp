#include <avr/io.h>
#include <avr/interrupt.h>

#include "x27.h"

static bool s_new_reading_ready = false;
static uint16_t s_adc_value = 0;

static void setup()
{
	// Enable ADC and interrupt, set /64 prescaler
	ADCSRA |= _BV(ADEN) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1);

	// Select external reference, channel 0
	ADMUX = _BV(REFS0);
}

void run_adc()
{

	position_command cmd;

	setup();

	while(true)
	{
		if (s_new_reading_ready)
		{
			cmd.position = s_adc_value * MAX_STEPS / 1024U;
			cmd.type = POSITION_TYPE_STEPS;
			x27_set_position(&cmd);
			s_new_reading_ready = false;
		}

		x27_service();
		
	}
}

ISR(ADC_vect)
{
	uint16_t result = 0;

	result = ADCL;
	result |= ADCH << 8;

	s_adc_value = result;
	s_new_reading_ready = true;
}

