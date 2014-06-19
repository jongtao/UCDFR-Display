#ifndef UTILITIES_H
#define UTILITIES_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <util/delay.h>

#define F_CPU 16000000UL // 16 MHz
#define CTC_COUNT (((F_CPU/1000)/8) - 1)
#define USART_BAUD 9600
#define USART_BAUDRATE ((F_CPU/(USART_BAUD * 16UL)) - 1)

#define USART_STRING_LENGTH 14
#define USART_SEND_LENGTH 14



/* Rotary Input
Button	PD1
A				PD0
B				PD7
*/

/* Buzzer
F5
*/

/* LED (left to right)
D4-D7
E0-E1
C0-C7
*/



enum usart_state
{
	USART_WAIT,
	USART_ACCEPT,
	USART_ESCAPE
}; // usart state



typedef struct
{
	int num_button, detent;
} Inputs;



/* Interrupts

ISR(TIMER1_COMPA_vect); // Timer

ISR(INT0_vect);					// Button
ISR(INT1_vect);					// Rotary

ISR(USART1_RX_vect); 		// USART

*/



void utilities_init();				// Initialize low level functions
unsigned long millis();				// Returns ms since utility init
Inputs get_inputs();					// Gets physical user inputs
uint8_t* get_usart(uint8_t *string);// Copies buffer into string
void wait_usart_empty();			// Wait for empty usart buffer
void put_usart(uint8_t *string);	// Sends string through USART
void put_leds(uint16_t leds);	// 14 LED bargraph. LSB is right. Right aligned

#endif
