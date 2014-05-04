#ifndef UTILITIES_H
#define UTILITIES_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#define F_CPU 16000000UL // 16 MHz
#define CTC_MATCH_OVERFLOW ((F_CPU / 1000) / 64)

/* Rotary input
Button	PD1
A				PD0
B				PB7
*/

typedef struct
{
	uint8_t button, angle_segment;
} Inputs;


extern volatile Inputs inputs;
volatile unsigned long timer1_millis;

void utilities_init();
unsigned long millis();

#endif
