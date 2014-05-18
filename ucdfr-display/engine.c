#include "engine.h"




void engine_init(Data *data)
{
	DDRD |= (1 << 6); // LED TEST
	data->last_button_num = data->state_level= 0;
	//*usart_data_current = usart_data;
	int i =0;
	for(i = 0; i<256; i++)
		data->test_string[i] = 0;
} // engine_init()



void engine_get_inputs(Data *data, Inputs *inputs)
{
	char usart_string[USART_STRING_LENGTH];
	*inputs = get_inputs();
	pop_usart(usart_string);

	int i;
	for(i = 0; i < USART_STRING_LENGTH; i++)
		data->test_string[i] = usart_string[i];
	data->test_string[32] = '\0'; // chop string


// parse usart
} // engine_get_inputs()



void engine_logic(Data *data, Inputs *inputs)
{
	// translate input to user intention
	if(inputs->num_button > data->last_button_num)
	{
		data->last_button_num = inputs->num_button;

		// traverse states
		if(data->state_level == 0)
			data->state_level = 1;
		else
			if(data->state_level == 1)
				data->state_level = 0;

	// write data to ram
	// write data to eeprom

	} // if button increased
} // engine_process_data()



void engine_put_outputs(){}


void engine_graphics(uint8_t lcdBuffer[2][8][64], Data *data)
{
	//Inputs inputs = get_inputs();
	char string[128];
	graphics_clear_buffer(lcdBuffer);
	/*
	graphics_clear_buffer(lcdBuffer);
	sprintf(string, "%2d.%d", 64,2);
	graphics_num(lcdBuffer, 0, 0, string);
	graphics_print(lcdBuffer, 105, 20, "mph");
	graphics_print(lcdBuffer, 16, 46, "Temp: 40C (ok)\nTorque Bias: 60%");
	*/

/*	
	if(inputs.button)
	{
		graphics_print(lcdBuffer, 64, 20, "Button");
	}

	sprintf(string,
		"millis: %lu\nnum_buttons: %d\nrotations: %d\n",
		millis(), inputs.num_button, inputs.detent);
	
	graphics_print(lcdBuffer, 0, 0, string);
*/

	if(data->state_level == 0)
		sprintf(string, "State 0");

	if(data->state_level == 1)
		//sprintf(string, "State aasdf");
		sprintf(string, "usart: %s", data->test_string);
	
	graphics_print(lcdBuffer, 0, 0, string);

} // engine_graphics()

