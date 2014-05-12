#include "engine.h"






void engine_init()
{
	DDRD |= (1 << 6); // LED TEST
} // engine_init()



void engine_get_inputs(Inputs *inputs, uint8_t *usart_byte)
{
	*inputs = get_inputs();
	*usart_byte = pop_usart();
} // engine_get_inputs()



void engine_logic(Data *data, Inputs *inputs, uint8_t *usart_byte)
{
	// translate input to user intention
	// traverse states
	// write data to ram
	// write data to eeprom
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
		graphics_print(lcdBuffer, 64, 20, "Button");
	}

	sprintf(string,
		"millis: %lu\nnum_buttons: %d\nrotations: %d\n",
		millis(), inputs.num_button, inputs.detent);
	
	graphics_print(lcdBuffer, 0, 0, string);
} // engine_graphics()

