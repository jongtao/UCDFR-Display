#include "engine.h"


//volatile Inputs inputs;

void engine_init()
{
	DDRD |= (1 << 6); // LED TEST

	// FIXME these inputs are depreciated.
	// Enable input
	DDRC &= ~(1); // PC0
	DDRE &= ~(3); // PE0 to PE1

	// Enable Pullup
	PORTC |= (1); // Pullup PC0
	PORTE |= (3);	// Pullup PE0 to PE1

	//:w


} // engine_init()



void engine_get_inputs(Inputs inputs)
{
	//FIXME depreciated Button
	/*
	if(PINC & (1))
		PORTD |= (1 << 6);
	else
		PORTD &= ~(1 << 6);
*/

	//UART

	
} // engine_get_inputs()



void engine_process_data()
{
} // engine_process_data()



void engine_put_outputs(){}


void engine_graphics(uint8_t lcdBuffer[2][8][64])
{
	Inputs inputs = get_inputs();
	char string[64];
	/*
	graphics_clear_buffer(lcdBuffer);
	sprintf(string, "%2d.%d", 64,2);
	graphics_num(lcdBuffer, 0, 0, string);
	graphics_print(lcdBuffer, 105, 20, "mph");
	graphics_print(lcdBuffer, 16, 46, "Temp: 40C (ok)\nTorque Bias: 60%");
	*/

	graphics_clear_buffer(lcdBuffer);
	
	if(inputs.button)
	{
		graphics_print(lcdBuffer, 105, 20, "Button");
	}
	//unsigned long time = millis();
	sprintf(string,
		"millis: %lu\nnum_buttons: %d\nrotations: %d\n",
		millis(), inputs.num_button, inputs.revolutions);
	graphics_print(lcdBuffer, 0, 0, string);

} // engine_graphics()

