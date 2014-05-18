#include "utilities.h"



volatile Inputs inputs;
volatile unsigned long timer1_millis = 0;
volatile unsigned long last_button = 0;
volatile unsigned long last_rev = 0;

volatile uint8_t usart_queue[USART_QUEUE_LENGTH][USART_STRING_LENGTH];
volatile uint8_t string_end = 0;
volatile uint8_t usart_head = 0;
volatile uint8_t usart_tail = 0;



ISR(TIMER1_COMPA_vect)
{
	timer1_millis++;
} // ISR(TIMER1)



ISR(INT0_vect)
{
	EIMSK ^= (1);

	if((millis() - last_button) > 10)
	{
		if(PIND & (1 << 0))
		{
			PORTD &= ~(1 << 6);
			inputs.button = 0;
		} // OFF

		if(!(PIND & (1 << 0)) && !inputs.button)
		{
			inputs.button = 1;
			inputs.num_button++;
		} // ON

		last_button = millis();
	} // Debounce

	EIMSK |= (1);
} // ISR(INT0) Button



ISR(INT1_vect)
{
	EIMSK ^= (2);

	if(((millis() - last_rev) > 50))
	{
			if(!(PINB&(1<<7)))
				inputs.detent--;
			else
				inputs.detent++;
		
			last_rev = millis();
	}// Debounce

	EIMSK |= (2);
} // ISR(INT1) A pin



ISR(USART1_RX_vect, ISR_BLOCK)
{
	PORTD |= (1<<6);
	usart_queue[usart_tail][string_end] = UDR1;
	/*
	usart_queue[usart_tail][string_end] = 'a';
	usart_queue[usart_tail][string_end + 1] = '\n';
	usart_queue[usart_tail][string_end + 2] = 'a';
	*/
	

	if(usart_queue[usart_tail][string_end] == '\n')
	{
		//usart_queue[usart_tail][string_end + 1] = '\0';
		//if(string_end + 1 >= USART_STRING_LENGTH)
		usart_tail++;
		string_end = 0;

		if(usart_tail >= USART_QUEUE_LENGTH) // roll queue
			usart_tail = 0;

		if(usart_head == usart_tail) // overwrite unread data if overflow
		{
			usart_head++;

			if(usart_head >= USART_QUEUE_LENGTH)
				usart_head = 0;
		}

	} // new line
	else
		if((string_end + 1) >= USART_STRING_LENGTH)
		{
			string_end = 0;	
		} // error: no newline found. discard data
		else
			string_end++;
			
	PORTD &= ~(1<<6);
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
	EICRA |= (1 << ISC00) | (1 << ISC11);
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



void pop_usart(char *string)
{
	uint8_t i;

	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		
		for(i = 0; i < 10; i++)
			string[i] = usart_queue[usart_head][i];

/*
		if(usart_head != usart_tail)
		{
			
			for(i = 0; i < USART_STRING_LENGTH - 1; i++)
				string[i] = usart_queue[usart_head][i];

			//usart_head++;

			if(usart_head >= USART_QUEUE_LENGTH)
				usart_head = 0;
				
		} // queue is not empty
		else
			for(i = 0; i < USART_STRING_LENGTH - 1; i++)
				string[i] = 0; // zero if empty
	*/		
	} // atomic
} // pop_usart()

