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
		case 2:
			configuration_logic(data, inputs);
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

		switch(data->state[data->state_level])
		{
			case S2_BACK:
				data->state_level = 0;
				data->state[data->state_level] = S1_SCREEN_ONE;
				break;
			case S2_1_CONF:
			case S2_2_CONF:
			case S2_3_CONF:
			case S2_MISC:
				data->state_level = 2;
				break;
		};
		
		data->state[data->state_level] = 0; // set to first option
	} // if button increased

	rotary_logic(data, inputs, data->state_level, 4);
} // selection_logic()



void configuration_logic(Data *data, Inputs *inputs)
{
	if(inputs->num_button > data->last_button_num)
	{
		data->last_button_num = inputs->num_button;

		switch(data->state[data->state_level])
		{
			case F_BACK:
				data->state_level = 1;
				data->state[data->state_level] = S1_SCREEN_ONE;
				break;
			case F_LED: case F_BIG: case F_ONE: case F_TWO: case F_THREE:
			case F_FOUR: case F_FIVE:case F_SIX: case F_SEVEN:
				data->state_level = 2;
				break;
		};

		data->state[data->state_level] = 0; // set to first option
	} // if button increased

	rotary_logic(data, inputs, data->state_level, F_SEVEN);
} // configuration_logic()



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



void engine_put_outputs()
{
	put_leds(0x0000);
} // engine_put_outputs()



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
	} // LEVEL 0

	if(data->state_level == 1)
	{
		graphics_print(lcdBuffer, 0, 0,  "    CONFIGURATION");
		graphics_print(lcdBuffer, 0, 9,  " < Back");
		graphics_print(lcdBuffer, 0, 18, "   Screen 1 Config >");
		graphics_print(lcdBuffer, 0, 27, "   Screen 2 Config >");
		graphics_print(lcdBuffer, 0, 36, "   Screen 3 Config >");
		graphics_print(lcdBuffer, 0, 45, "   Miscellaneous   >");

		graphics_xor_rect(lcdBuffer,
			12, 9*(1 + data->state[data->state_level]), 102, 8);

		sprintf(string, "usart: %s", data->test_string);
		graphics_print(lcdBuffer, 0, 54, string);
	} // LEVEL 1



	if(data->state_level == 2)
	{
		uint8_t i = 1;
		uint8_t j;
		char number[2] = {0,0};

		if(data->state[1] == S2_MISC)
		{
			sprintf(string, "usart: %s", data->test_string);
			graphics_print(lcdBuffer, 0, 0, string);
		} // if MISC
		else
		{
			switch(data->state[1])
			{
				case S2_1_CONF:
					graphics_print(lcdBuffer, 0, 0,  "   SCREEN 1 CONFIG");
					break;
				case S2_2_CONF:
					graphics_print(lcdBuffer, 0, 0,  "   SCREEN 2 CONFIG");
					break;
				case S2_3_CONF:
					graphics_print(lcdBuffer, 0, 0,  "   SCREEN 3 CONFIG");
					break;
			};
			i = 1;

			graphics_print(lcdBuffer, 0, 9*i++,  " < Back");
			graphics_print(lcdBuffer, 0, 9*i++, "   LED Bar         >");
			graphics_print(lcdBuffer, 0, 9*i++, "   Big Field       >");


			for(j=1; j<8; j++)
			{
				*number = '0' + j;
				graphics_print(lcdBuffer, 0, 9*i, "   Field ");
				graphics_print(lcdBuffer, 54, 9*i, number);
				graphics_print(lcdBuffer, 114, 9*i++, ">");
			}
/*

			graphics_print(lcdBuffer, 0, 36, "   Field 2    >");
			graphics_print(lcdBuffer, 0, 36, "   Field 3    >");
			graphics_print(lcdBuffer, 0, 36, "   Field 4    >");
			graphics_print(lcdBuffer, 0, 36, "   Field 5    >");
			graphics_print(lcdBuffer, 0, 36, "   Field 6    >");
			graphics_print(lcdBuffer, 0, 36, "   Field 7    >");
*/

			graphics_xor_rect(lcdBuffer,
				12, 9*(1 + data->state[data->state_level]), 102, 8);
		} // if not MISC
	} // LEVEL 2
	

} // engine_graphics()

