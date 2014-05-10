#ifndef UTILITIES_H
#define UTILITIES_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <util/delay.h>

//#define F_CPU 16000000UL // 16 MHz
#define CTC_COUNT ((F_CPU/1000)/8)


/* Rotary input
Button	PD0
A				PD1
B				PB7
*/

/* Buzzer
F5

*/

typedef struct
{
	int button, num_button, detent;
} Inputs;


void utilities_init();
unsigned long millis();
Inputs get_inputs();

#endif
