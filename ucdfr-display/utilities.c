#include "utilities.h"


volatile Inputs inputs;




ISR(TIMER1_COMPA_vect)
{
	timer1_millis++;
} // ISR(TIMER1)



ISR(PCINT0_vect)
{
		
} // IST(PCINT0)



void utilities_init()
{
	// Enable Input
	DDRD &= ~(3);				// PD0 to PD1
	DDRB &= ~(1 << 7);	// PB7

	// Enable Input Pullup
	PORTD	|= (3);				// PD0 to PD1
	PORTB |= (1 <<7);		// PB7

	// Input interrupt
	PCMSK0 |= (3);	// PCIE0 PCIE1
	PCICR |= (3);	// PCINT0 PCINT1

	// Timer interrupt
	TCCR1B |= (1 << WGM12) | (1 << CS11);
	OCR1AH = (CTC_MATCH_OVERFLOW >> 8);
	OCR1AL = 0xFF & CTC_MATCH_OVERFLOW;
	TIMSK1 |= (1 << OCIE1A);

	

	sei();
} // timer_init()



unsigned long millis()
{
	unsigned long time;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		time = timer1_millis;
	} // ATOMIC FORCEON

	return time;
} // millis()

