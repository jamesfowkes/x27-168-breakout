#include <avr/io.h>
#include <avr/interrupt.h>

#include "x27.h"

static bool s_new_reading_ready = false;
static uint16_t s_high_capture = 0;
static uint16_t s_low_capture = 0;
static bool capturing_high = false;

static void setup()
{
	TCCR1A = 0;
	TCCR1B = _BV(CS11) | _BV(CS10);
	TCCR1C = 0;
	TIMSK1 = _BV(ICIE1);
}

void run_pwm()
{
	position_command cmd;

	setup();
	while(true)
	{
		if (s_new_reading_ready)
		{
			cmd.position = (s_low_capture * MAX_STEPS) / (s_low_capture + s_high_capture);
			cmd.type = POSITION_TYPE_STEPS;
			x27_set_position(&cmd);
			s_new_reading_ready = false;
		}
	}
}

ISR(TIMER1_CAPT_vect)
{
	uint16_t count = TCNT1;

	TCNT1 = 0; // Reset counter for next edge
	TCCR1B ^= (1 << ICES1); // Toggle edge sensitivity
	
	if (capturing_high) {
		s_high_capture = count;	
	}
	else
	{
		s_low_capture = count;
	}

	s_new_reading_ready = true;

	capturing_high = !capturing_high;
}
