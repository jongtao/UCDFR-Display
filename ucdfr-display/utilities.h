#ifndef UTILITIES_H
#define UTILITIES_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <util/delay.h>

//#define F_CPU 16000000UL // 16 MHz
#define CTC_COUNT (((F_CPU/1000)/8) - 1)
#define USART_BAUD 9600
#define USART_BAUDRATE ((F_CPU/(USART_BAUD * 16UL)) - 1)
#define USART_QUEUE_LENGTH 8
#define USART_STRING_LENGTH 256


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



/* Interrupts

ISR(TIMER1_COMPA_vect); // Timer

ISR(INT0_vect);					// Button
ISR(INT1_vect);					// Rotary

ISR(USART1_RX_vect); 		// USART

*/



void utilities_init();
unsigned long millis();
Inputs get_inputs();
void pop_usart(char *string);

#endif
