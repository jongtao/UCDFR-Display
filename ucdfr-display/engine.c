#include "engine.h"




void engine_init(Data *data)
{
	DDRD |= (1 << 6); // LED TEST
	data->last_button_num = data->last_detent_num = data->state_level = 0;
	data->state[0] = 0;

	int i =0;
	for(i = 0; i<256; i++)
		data->test_string[i] = 0; // FIXME

} // engine_init()



void engine_get_inputs(Data *data, Inputs *inputs)
{
	unsigned int i;
	char usart_string[USART_STRING_LENGTH];
	*inputs = get_inputs();
	get_usart(usart_string);

	
	for(i = 0; i < USART_STRING_LENGTH; i++)
		data->test_string[i] = usart_string[i];

	data->test_string[255] = 0;
	data->usart_data.speed = usart_string[0]; // FIXME test speed

// parse usart
} // engine_get_inputs()



void engine_logic(Data *data, Inputs *inputs)
{
	switch(data->state_level)
	{
		case 0:
			primary_logic(data, inputs);
			break;
		case 1:
			selection_logic(data, inputs);
			break;
	};
} // engine_process_data()




void primary_logic(Data *data, Inputs *inputs)
{
	if(inputs->num_button > data->last_button_num)
	{
		data->last_button_num = inputs->num_button;
		data->state_level = 1;
		data->state[data->state_level] = 0;
	} // if button increased

	rotary_logic(data, inputs, data->state_level, 2);
} // primary_logic()




void selection_logic(Data *data, Inputs *inputs)
{
	if(inputs->num_button > data->last_button_num)
	{
		data->last_button_num = inputs->num_button;

		if(data->state[data->state_level] == S2_BACK)
			data->state_level = 0;
	} // if button increased

	rotary_logic(data, inputs, data->state_level, 4);
} // selection_logic()



void rotary_logic(Data *data, Inputs *inputs, uint8_t state_level,
	uint8_t most) // scrolls through state in current level
{
	if(inputs->detent > data->last_detent_num)
	{
		data->last_detent_num = inputs->detent ;

		if(data->state[state_level] >= most)
			data->state[state_level] = 0;
		else
			data->state[state_level]++;
	} // anticlockwise
	else
		if(inputs->detent < data->last_detent_num)
		{
			 data->last_detent_num = inputs->detent;

			if(data->state[state_level] <= 0)
				data->state[state_level] = most;
			else
				data->state[state_level]--;
		} // clockwise
} // rotary_logic()



void engine_put_outputs(){}



void engine_graphics(uint8_t lcdBuffer[2][8][64], Data *data)
{
	//Inputs inputs = get_inputs();
	char string[256];
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
	{
		//sprintf(string, "State 0: %d", data->state[0]);
		sprintf(string, "%d", data->usart_data.speed);
		graphics_num(lcdBuffer, 0, 0, string);
		graphics_print(lcdBuffer, 105, 20, "mph");
		graphics_print(lcdBuffer, 0, 46, "Temp: 40C (ok)\nTorque Bias: 60%");
	}

	if(data->state_level == 1)
	{
		graphics_print(lcdBuffer, 0, 0,  "    CONFIGURATION");
		graphics_print(lcdBuffer, 0, 9,  " < Back");
		graphics_print(lcdBuffer, 0, 18, "   Screen 1 Config >");
		graphics_print(lcdBuffer, 0, 27, "   Screen 2 Config >");
		graphics_print(lcdBuffer, 0, 36, "   Screen 3 Config >");
		graphics_print(lcdBuffer, 0, 45, "   Miscellaneous   >");

		graphics_blit(lcdBuffer, 12, 9*(1 + data->state[data->state_level]),
			NULL, 0, 0, 102, 8, 0, XOR);

		sprintf(string, "usart: %s", data->test_string);
		graphics_print(lcdBuffer, 0, 54, string);
	}
	

} // engine_graphics()

