#include "utilities.h"



volatile Inputs inputs;
volatile unsigned long timer1_millis = 0;
volatile unsigned long last_button = 0;
volatile unsigned long last_rev = 0;

volatile uint8_t rotate_now = 1; // 0=cwise, 1=untouched, 2=countercwise

volatile uint8_t string_end = 0;
volatile uint8_t usart_tmp_buffer[USART_STRING_LENGTH];
volatile uint8_t usart_buffer[USART_STRING_LENGTH];




ISR(TIMER1_COMPA_vect)
{
	timer1_millis++;

	// Begin Scheduled events

	// poll for rotary state.
	if(rotate_now != 1)
		if(!(PINB&(1<<7)) && !(PIND&(1<<0)))
		{
			if(rotate_now == 2)
				inputs.detent++;
			else
				if(rotate_now == 0)
					inputs.detent--;

			rotate_now = 1;
		} // if back to normal zero state
} // ISR(TIMER1)



ISR(INT1_vect, ISR_BLOCK)
{
	PORTD &= ~(1 << 6);

	if((millis() - last_button) > 10)
	{
		if(PIND & (1 << 1))
		{
			PORTD |= (1 << 6);
			inputs.button = 0;
		} // OFF

		if(!(PIND & (1 << 1)) && !inputs.button)
		{
			inputs.button = 1;
			inputs.num_button++;
		} // ON

		last_button = millis();
	} // Debounce
} // ISR(INT0) Button



ISR(INT0_vect, ISR_BLOCK)
{
		if(!(PINB&(1<<7)))
			rotate_now = 0;
		else
			rotate_now = 2;

	/*
	if((millis() - last_rev) > 50)
	{
		_delay_ms(2);
		if(!(PINB&(1<<7)))
			inputs.detent--;
		else
			inputs.detent++;
	
		last_rev = millis();
	}// Debounce
	*/
} // ISR(INT1) A pin



ISR(USART1_RX_vect, ISR_BLOCK)
{
	unsigned int i;

	usart_tmp_buffer[string_end] = UDR1;
		
	if(usart_tmp_buffer[string_end] == '\n')
	{
		//if(string_end + 1 == USART_STRING_LENGTH) // copy completed string
			for(i = 0; i < USART_STRING_LENGTH; i++)
				usart_buffer[i] = usart_tmp_buffer[i];
			
		string_end = 0;
	}
	else
		if(string_end + 1 >= USART_STRING_LENGTH) // overflow
			string_end = 0;
		else
			string_end++;
	
} // ISR(USART1) USART



void utilities_init()
{
	CLKPR = 0x80, CLKPR = 0; // zero prescaler

	// Enable Input
	DDRD &= ~(3);				// PD0 to PD1
	DDRB &= ~(1 << 7);	// PB7

	// Enable Input Pullup
	PORTD	|= (3);				// PD0 to PD1
	PORTB |= (1 << 7);	// PB7

	// Input as external interrupts
	EICRA |= (1 << ISC01) | (1 << ISC10);
	EIMSK |= (3);

	DDRF |= (1 << 5); // buzzer

	// Timer interrupt
	TCCR1B |= (1 << WGM12) | (1 << CS11); // Prescale 8
	OCR1A = CTC_COUNT;
	TIMSK1 |= (1 << OCIE1A);

	// zero inputs struct
	inputs.button = inputs.num_button = 0;
	inputs.detent = 0;

	// USART1
	UBRR1H = (USART_BAUDRATE >> 8);					// Baudrate
	UBRR1L = USART_BAUDRATE;
	UCSR1C = (1 << UCSZ10) | (1 << UCSZ11); // 8-bit asynchronous no parity
	UCSR1B = (1 << RXCIE1) | (1 << RXEN1) | (1 << TXEN1);		// Enable

	// LEDs
	DDRD |= 0xF0;				// D4-D7
	DDRE |= 0x03;				// E0-E1
	DDRC |= 0xFF;				// C0-C7
/*
	//Test LEDs
	PORTD |= 0xF0;				// D4-D7
	PORTE |= 0x03;				// E0-E1
	PORTC |= 0xFF;				// C0-C7
*/
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



char* get_usart(char *string)
{
	unsigned int i;

	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		for(i = 0; i + 1 < USART_STRING_LENGTH; i++)
			string[i] = usart_buffer[i]; // Copy buffer to string
	} // atomic

	return string;
} // get_usart()



void put_leds(uint16_t leds)
{
	PORTD &= ~(0xF0);
	PORTE &= ~(0x03);
	//PORTC &= ~(0xFF); 			// whole PC is used

	PORTD |= ((0x3C00 & leds) >> 6);
	PORTE |= ((0x0300 & leds) >> 8);
	PORTC = (0x00FF & leds);	// Equals
} // put_leds()

