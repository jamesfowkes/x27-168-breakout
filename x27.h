#ifndef _X27_H_
#define _X27_H_

#define MAX_STEPS (600)

enum position_type
{
	POSITION_TYPE_STEPS,
	POSITION_TYPE_PERCENT
};

struct position_command
{
	uint16_t position;
	enum position_type type;
};

void x27_initialise();
void x27_set_position(struct position_command const * const cmd);
void x27_service();
void x27_test();

void run_serial();
void run_adc();
void run_spi();
void run_pwm();


#endif
