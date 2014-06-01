#include "engine.h"



uint8_t scroll_level = 0;



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
			engine_logic_0(data, inputs);
			break;
		case 1:
			engine_logic_1(data, inputs);
			break;
		case 2:
			engine_logic_2(data, inputs);
			break;
	};
} // engine_process_data()




void engine_logic_0(Data *data, Inputs *inputs)
{
	if(inputs->num_button > data->last_button_num)
	{
		data->last_button_num = inputs->num_button;
		data->state_level = 1;
		data->state[data->state_level] = 0;
	} // if button increased

	rotary_logic(data, inputs, data->state_level, 2);
} // engine_logic_0()




void engine_logic_1(Data *data, Inputs *inputs)
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
} // engine_logic_1()



void engine_logic_2(Data *data, Inputs *inputs)
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
			case F_FOUR: case F_FIVE: case F_SIX: case F_SEVEN:
				data->state_level = 2;
				scroll_level = 0;
				break;
		};

		data->state[data->state_level] = 0; // set to first option
	} // if button increased

	rotary_logic(data, inputs, data->state_level, F_SEVEN);
} // engine_logic_2()



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



/* BEGIN GRAPHICS*/



void engine_put_outputs()
{
	put_leds(0x0000);
} // engine_put_outputs()



void engine_graphics(uint8_t lcdBuffer[2][8][64], Data *data)
{
	graphics_clear_buffer(lcdBuffer);
	
	if(data->state_level == 0)
		engine_graphics_0(lcdBuffer, data);

	if(data->state_level == 1)
		engine_graphics_1(lcdBuffer, data);

	if(data->state_level == 2)
		engine_graphics_2(lcdBuffer, data);
} // engine_graphics()



void engine_graphics_0(uint8_t lcdBuffer[2][8][64], Data *data)
{
	char string[256];
	sprintf(string, "%d", data->usart_data.speed);
	graphics_num(lcdBuffer, 0, 0, string);
	graphics_print(lcdBuffer, 105, 20, "mph");
	graphics_print(lcdBuffer, 0, 46, "Temp: 40C (ok)");
	sprintf(string, "Screen: %d", data->state[data->state_level]);
	graphics_print(lcdBuffer, 0, 55, string);
} // engine_graphics_0



void engine_graphics_1(uint8_t lcdBuffer[2][8][64], Data *data)
{
	char string[256];
	graphics_print(lcdBuffer, 0, 0,  "    CONFIGURATION");
	graphics_print(lcdBuffer, 0, 9,  " < Back");
	graphics_print(lcdBuffer, 0, 18, "   Screen 1 Config >");
	graphics_print(lcdBuffer, 0, 27, "   Screen 2 Config >");
	graphics_print(lcdBuffer, 0, 36, "   Screen 3 Config >");
	graphics_print(lcdBuffer, 0, 45, "   Miscellaneous   >");

	graphics_rect(lcdBuffer,
		12, 9*(1 + data->state[data->state_level]), 102, 8, XOR);

	sprintf(string, "usart: %s", data->test_string);
	graphics_print(lcdBuffer, 0, 54, string);
} // engine_graphics_1()



void engine_graphics_2(uint8_t lcdBuffer[2][8][64], Data *data)
{
	char string[256];
	uint8_t i;
	uint8_t j;
	char number[2] = {0,0};

	if(data->state[1] == S2_MISC)
	{
		sprintf(string, "usart: %s", data->test_string);
		graphics_print(lcdBuffer, 0, 0, string);
	} // if MISC
	else
	{
		// Scroll
		if(data->state[data->state_level] > 5 + scroll_level)
			scroll_level++;
		else
			if(data->state[data->state_level] < scroll_level)
				scroll_level--;


		// Text

		i = 1;
		graphics_print(lcdBuffer, 0, 9*(i++-scroll_level), " < Back");
		graphics_print(lcdBuffer, 0, 9*(i++-scroll_level), "   LED Bar         >");
		graphics_print(lcdBuffer, 0, 9*(i++-scroll_level), "   Big Field       >");

		for(j=1; j<8; j++)
		{
			*number = '0' + j;
			graphics_print(lcdBuffer, 0, 9*(i-scroll_level), "   Field ");
			graphics_print(lcdBuffer, 54, 9*(i-scroll_level), number);
			graphics_print(lcdBuffer, 114, 9*(i++-scroll_level), ">");
		}

		graphics_rect(lcdBuffer,
			12, 9*(1 + data->state[data->state_level]-scroll_level), 102, 8, XOR);

		graphics_rect(lcdBuffer, 0, 0, 128, 8, ZERO);	// wipe title background

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
	} // if not MISC
} // engine_graphics_2()
