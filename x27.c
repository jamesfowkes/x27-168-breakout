#include <stdbool.h>

#include <avr/io.h>
#include <util/delay.h>

#include "x27.h"

#define COIL1A_PORT PORTD
#define COIL1B_PORT PORTD
#define COIL2A_PORT PORTC
#define COIL2B_PORT PORTC

#define COIL1A_DDR DDRD
#define COIL1B_DDR DDRD
#define COIL2A_DDR DDRC
#define COIL2B_DDR DDRC

enum coil_polarity
{
	COIL_POL_1,
	COIL_POL_2,
	COIL_POL_BOTH_HIGH,
	COIL_POL_BOTH_LOW
};

static const int COIL1A_PIN = 5;
static const int COIL1B_PIN = 4;
static const int COIL2A_PIN = 0;
static const int COIL2B_PIN = 1;

static const int STEP_DELAY = 3200;

static int s_steps = 0;
static int s_target = 0;

static inline void coil1(enum coil_polarity pol)
{
	switch(pol)
	{
	case COIL_POL_2:
		COIL1A_PORT |= _BV(COIL1A_PIN);
		COIL1B_PORT &= ~_BV(COIL1B_PIN);
		break;
	case COIL_POL_1:
		COIL1A_PORT &= ~_BV(COIL1A_PIN);
		COIL1B_PORT |= _BV(COIL1B_PIN);
		break;
	case COIL_POL_BOTH_LOW:
		COIL1A_PORT &= ~_BV(COIL1A_PIN);
		COIL1B_PORT &= ~_BV(COIL1B_PIN);
		break;
	case COIL_POL_BOTH_HIGH:
		COIL1A_PORT |= _BV(COIL1A_PIN);
		COIL1B_PORT |= _BV(COIL1B_PIN);
		break;
	}
}

static inline void coil2(enum coil_polarity pol)
{
	switch(pol)
	{
	case COIL_POL_2:
		COIL2A_PORT |= _BV(COIL2A_PIN);
		COIL2B_PORT &= ~_BV(COIL2B_PIN);
		break;
	case COIL_POL_1:
		COIL2A_PORT &= ~_BV(COIL2A_PIN);
		COIL2B_PORT |= _BV(COIL2B_PIN);
		break;
	case COIL_POL_BOTH_LOW:
		COIL2A_PORT &= ~_BV(COIL2A_PIN);
		COIL2B_PORT &= ~_BV(COIL2B_PIN);
		break;
	case COIL_POL_BOTH_HIGH:
		COIL2A_PORT |= _BV(COIL2A_PIN);
		COIL2B_PORT |= _BV(COIL2B_PIN);
		break;
	}
}

static inline int clip_steps(int steps)
{
	steps = (steps > MAX_STEPS) ? MAX_STEPS : steps;
	steps = (steps < 0) ? 0 : steps;
	return steps;
}

static inline int clip_percent(int steps)
{
	steps = (steps > 100) ? 100 : steps;
	steps = (steps < 0) ? 0 : steps;
	return steps;
}

static inline int percent_to_steps(int percent)
{
	percent = clip_percent(percent);
	return percent * 6;
}

static void step(char direction)
{
	static int8_t step_no = 0;

	step_no += direction;

	if (step_no == -1)
	{
		step_no = 5;	
	}
	else if (step_no == 6)
	{
		step_no = 0;
	}

	switch(step_no)
	{
	case 0:
		coil1(COIL_POL_1);
		coil2(COIL_POL_2);
	    break;
	case 1:
		coil1(COIL_POL_1);
		coil2(COIL_POL_BOTH_LOW);
		break;
	case 2:
		coil1(COIL_POL_BOTH_HIGH);
		coil2(COIL_POL_1);
		break;
	case 3:
		coil1(COIL_POL_2);
		coil2(COIL_POL_1);
		break;
	case 4:
		coil1(COIL_POL_2);
		coil2(COIL_POL_BOTH_HIGH);
		break;
	case 5:
		coil1(COIL_POL_BOTH_LOW);
		coil2(COIL_POL_2);
		break;
	}
}

static void go_to_low_limit()
{
	int i;
	for (i = 0; i < MAX_STEPS; i++)
	{
		step(-1);
		_delay_us(STEP_DELAY);
	}
}

static void go_to_high_limit()
{
	int i;
	for (i = 0; i < MAX_STEPS; i++)
	{
		step(1);
		_delay_us(STEP_DELAY);
	}
}


void x27_initialise()
{
	COIL1A_DDR |= _BV(COIL1A_PIN);
	COIL1B_DDR |= _BV(COIL1B_PIN);
	COIL2A_DDR |= _BV(COIL2A_PIN);
	COIL2B_DDR |= _BV(COIL2B_PIN);

	go_to_high_limit();

	s_target = 0;
	s_steps = 0;
}

void x27_set_position(struct position_command const * const cmd)
{
	int target = 0;

	switch (cmd->type)
	{
	case POSITION_TYPE_PERCENT:
		target = percent_to_steps(cmd->position);
		break;
	case POSITION_TYPE_STEPS:
		target = clip_steps(cmd->position);
		break;
	default:
		return;
	}

	if (s_target != target)
	{
		s_target = target;
	}
}

void x27_service()
{
	if (s_steps < s_target)
	{
		step(-1);
	}
	else if (s_target > s_steps)
	{
		step(1);
	}
}

void x27_test()
{
	while(true)
	{
		go_to_low_limit();
		_delay_ms(500);
		PINC |= _BV(PINC2);
		go_to_high_limit();
		_delay_ms(500);
		PINC |= _BV(PINC2);
	}
}