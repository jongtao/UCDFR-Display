#include "utilities.h"



volatile Inputs inputs;
volatile unsigned long timer1_millis = 0;
volatile unsigned long last_button = 0;
volatile unsigned long last_rev = 0;



ISR(TIMER1_COMPA_vect)
{
	timer1_millis++;
} // ISR(TIMER1)



ISR(INT0_vect)
{
	//inputs.button ^= 1;
	if((millis() - last_button) > 1)
	{
			//PORTD &= ~(1 << 6);
		if(PIND & (1 << 0))
		{
			PORTD &= ~(1 << 6);
			inputs.button = 0;
		} // OFF

		if(!(PIND & (1 << 0)) && !inputs.button)
		{
			PORTD |= (1 << 6);
			inputs.button = 1;
			inputs.num_button++;

				PORTF |= (1 << 5);
				_delay_ms(10);
				PORTF &= ~(1 << 5);

		} // ON

		last_button = millis();
	} // Debounce
	
	//EIFR = 0; 
} // ISR(INT0) Button



ISR(INT1_vect)
{
	EIMSK ^= (2);
	_delay_us(100);
	//uint8_t B = !!(PINB&(1 << 7));

	if(((millis() - last_rev) > 50))
	{
			if(!(PINB&(1<<7)))
				inputs.detent--;
			else
					inputs.detent++;
		
			last_rev = millis();
	}

	EIMSK |= (2);
} // ISR(INT1) A pin



void utilities_init()
{
	// Enable Input
	DDRD &= ~(3);				// PD0 to PD1
	DDRB &= ~(1 << 7);	// PB7

	// Enable Input Pullup
	PORTD	|= (3);				// PD0 to PD1
	PORTB |= (1 << 7);	// PB7

	//PCICR |= (1);				// Enable pin change interrupt
	//PCMSK0 |= (3);			// PCIE0 PCIE1

	EICRA |= (1 << ISC00) | (1 << ISC11);
	EIMSK |= (3);

	DDRF |= (1 << 5); // buzzer

	// Timer interrupt
	TCCR1B |= (1 << WGM12) | (1 << CS10);
	OCR1A = CTC_COUNT;
	TIMSK1 |= (1 << OCIE1A);


	// zero inputs
	inputs.button = inputs.num_button = 0;
	inputs.detent = 0;

	sei();							// Enable interrupts
} // timer_init()



unsigned long millis()
{
	unsigned long time;

	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		time = timer1_millis;
	} // ATOMIC FORCEON

	return time;
} // millis()



Inputs get_inputs()
{
	Inputs tmp;

	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		tmp = inputs;
	} // ATOMIC FORCEON

	return tmp;
} // get_inputs()

