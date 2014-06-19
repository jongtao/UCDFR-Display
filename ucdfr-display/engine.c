#include "engine.h"



uint8_t scroll_level = 0;


// Flappy
#define GRAV 1
#define SCROLL_SPEED 1
#define JUMP -4
#define PLAYER_SIZE 5
#define NUM_PILLARS 2
#define PILLAR_WIDTH 5
#define PILLAR_GAP 20
#define PILLAR_DENSITY 64
#define SCROLL_DELAY 50
#define DEATH_MARGIN 15
#define PLAYER_HORI 2
int player_vert;
uint32_t player_score;
int player_velocity;
int pillars_vert[NUM_PILLARS];
int pillars_hori[NUM_PILLARS];
unsigned long last_millis = 0;
unsigned long last_rot;
uint8_t gameover;
// End Flappy



void engine_init(Data *data)
{
	DDRD |= (1 << 6); // LED TEST
	data->last_button_num = data->last_detent_num = data->state_level = 0;
	data->state[0] = 0;

/*
	int i =0;
	for(i = 0; i<256; i++)
		data->test_string[i] = 0; // FIXME
*/
} // engine_init()



void engine_get_inputs(Data *data, Inputs *inputs)
{
	unsigned int i;
	uint8_t usart_string[USART_STRING_LENGTH];
	*inputs = get_inputs();
	get_usart(usart_string);

/*	
	for(i = 0; i < USART_STRING_LENGTH; i++)
		data->test_string[i] = usart_string[i];
*/
	/*data->test_string[USART_STRING_LENGTH - 1] = 0;// FIXME end string
	data->usart_data.speed = usart_string[0]; // FIXME test speed*/

	// parse usart
	i = 0;

	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		data->usart_data.state_of_charge = usart_string[i++];

		data->usart_data.status = (uint16_t)usart_string[i++] << 8;
		data->usart_data.status |= (uint16_t)usart_string[i++];

		data->usart_data.balance_mV = (uint16_t)usart_string[i++] << 8;
		data->usart_data.balance_mV |= (uint16_t)usart_string[i++];

		data->usart_data.remaining_AH = (uint16_t)usart_string[i++] << 8;
		data->usart_data.remaining_AH |= (uint16_t)usart_string[i++];

		short tmp = 0;
		tmp = (uint16_t)usart_string[i++] << 8;
		tmp |= (uint16_t)usart_string[i++];
		data->usart_data.current_A = (short)tmp;
		/*
		data->usart_data.current_A = (uint16_t)usart_string[i++] << 8;
		data->usart_data.current_A |= (uint16_t)usart_string[i++];
		data->usart_data.current_A = (short)data->usart_data.current_A;
*/
		data->usart_data.temp_C = usart_string[i++];

		data->usart_data.voltage_mV = (uint32_t)usart_string[i++] << 24;
		data->usart_data.voltage_mV |= ((uint32_t)usart_string[i++]) << 16;
		data->usart_data.voltage_mV |= ((uint32_t)usart_string[i++]) << 8;
		data->usart_data.voltage_mV |= ((uint32_t)usart_string[i++]);
	} // ATOMIC BLOCK
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
		flappy_init(data, inputs);
	} // if button increased

	rotary_logic(data, inputs, data->state_level, 2);
} // engine_logic_0()




void engine_logic_1(Data *data, Inputs *inputs)
{
	if(inputs->num_button > data->last_button_num)
	{
		data->last_button_num = inputs->num_button;
		
		data->state_level = 0;
		/*
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
		*/
		
		data->state[data->state_level] = 0; // set to first option
	} // if button increased

	flappy_logic(data, inputs);
	rotary_logic(data, inputs, data->state_level, 4);
} // engine_logic_1()



void flappy_init(Data *data, Inputs *inputs)
{
	uint8_t i;
	uint16_t j;
	srand(millis());

	player_vert = 24;
	player_score = 0;
	last_rot = inputs->detent;
	gameover = 0;

	for(i=0; i<NUM_PILLARS; i++)
		pillars_vert[i] = rand()%(65-2*PILLAR_GAP) + PILLAR_GAP;
	
	j=128;
	for(i=0; i<NUM_PILLARS; i++)
	{
		pillars_hori[i] = j;
		j+= PILLAR_DENSITY;
	}
} // flappy_init()



void flappy_logic(Data *data, Inputs *inputs)
{
	uint8_t i;
	srand(millis());
	rand();
	player_velocity += GRAV;
	player_vert += player_velocity;

	if(!gameover)
	{
		if(inputs->detent != last_rot)
		{
			last_rot = inputs->detent;
				player_velocity = JUMP;
		}
	}
	else
		player_vert = 70;
	if(player_velocity > 2)
		player_velocity = 2;

	if(player_vert < -DEATH_MARGIN || player_vert > 64+DEATH_MARGIN)
		gameover = 1;

	for(i=0; i<NUM_PILLARS; i++)
	{
		pillars_hori[i] -= SCROLL_SPEED;
		if(pillars_hori[i] < -PILLAR_WIDTH)
		{
			pillars_hori[i] = 129;
			pillars_vert[i] = rand()%(65-2*PILLAR_GAP) + PILLAR_GAP;

			if(!gameover)
				player_score++;
		}
	} // pillar horizontal

	for(i=0; i<NUM_PILLARS; i++)
	{
		if(PLAYER_HORI+PLAYER_SIZE > pillars_hori[i] &&
			PLAYER_HORI < pillars_hori[i] + PILLAR_WIDTH)
			if(player_vert < pillars_vert[i] ||
				player_vert + PLAYER_SIZE > pillars_vert[i] + PILLAR_GAP)
				gameover = 1;


	} // for every pillar

	_delay_ms(SCROLL_DELAY);
} // flappy_logic()



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
	
	//char *asdf = "ABCDEFGHIJKLMNOQQRSTUVWXYZ";
	//put_usart(asdf); // FIXME test usart send
		
	//put_leds(0xFFFF);
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
	uint16_t led_string = 0;
	uint8_t	led_shift = 0;
	uint8_t percent = 0;
	uint16_t i = 0;

	switch(data->state[data->state_level])
	{
		case 0:
			percent = (data->usart_data.voltage_mV*100)/PACK_MAX_MV;
			led_shift = percent / 14;

			for(i=13; i>13+led_shift; i--)
				led_string |= 1<<i;

			put_leds(led_string);

			sprintf(string, "%3lu", data->usart_data.voltage_mV/1000);
			graphics_num(lcdBuffer, 0, 0, string);
			graphics_print(lcdBuffer, 95, 20, "Volts");
			sprintf(string, "Current: %d A", data->usart_data.current_A);
			graphics_print(lcdBuffer, 0, 55, string);
			sprintf(string, "Pack Balance: %u mV", data->usart_data.balance_mV);
			graphics_print(lcdBuffer, 0, 46, string);
			//graphics_print(lcdBuffer, 0, 46, "Temp: 40C (ok)");
			break;
		case 1:
			put_leds(0);
			sprintf(string, "%3u%%",
				data->usart_data.state_of_charge);
			graphics_num(lcdBuffer, 0, 0, string);
			graphics_print(lcdBuffer, 110, 0, "SOC");
			sprintf(string, "Temperature: %u C", data->usart_data.temp_C);
			graphics_print(lcdBuffer, 0, 46, string);
			sprintf(string, "Voltage: %lu mV", data->usart_data.voltage_mV);
			graphics_print(lcdBuffer, 0, 55, string);
			break;
		case 2:
			put_leds(0);
			sprintf(string, "State of Charge: %u%%",data->usart_data.state_of_charge);
			graphics_print(lcdBuffer, 0, i, string);
			i += LINE_HEIGHT;
			sprintf(string, "Status: 0x%04X",data->usart_data.status);
			graphics_print(lcdBuffer, 0, i, string);
			i += LINE_HEIGHT;
			sprintf(string, "Balance: %u mV",data->usart_data.balance_mV);
			graphics_print(lcdBuffer, 0, i, string);
			i += LINE_HEIGHT;
			sprintf(string, "Remaining: %u AH",data->usart_data.remaining_AH);
			graphics_print(lcdBuffer, 0, i, string);
			i += LINE_HEIGHT;
			sprintf(string, "Current: %d A",data->usart_data.current_A);
			graphics_print(lcdBuffer, 0, i, string);
			i += LINE_HEIGHT;
			sprintf(string, "Temperature: %u C",data->usart_data.temp_C);
			graphics_print(lcdBuffer, 0, i, string);
			i += LINE_HEIGHT;
			sprintf(string, "Voltage: %lu mV",
				(unsigned long)data->usart_data.voltage_mV);
			graphics_print(lcdBuffer, 0, i, string);
			break;
	}; // switch state
	
} // engine_graphics_0



void engine_graphics_1(uint8_t lcdBuffer[2][8][64], Data *data)
{
	flappy_graphics(lcdBuffer, data);

	/*
	//char string[256];
	graphics_print(lcdBuffer, 0, 0,  "    CONFIGURATION");
	graphics_print(lcdBuffer, 0, 9,  " < Back");
	graphics_print(lcdBuffer, 0, 18, "   Screen 1 Config >");
	graphics_print(lcdBuffer, 0, 27, "   Screen 2 Config >");
	graphics_print(lcdBuffer, 0, 36, "   Screen 3 Config >");
	graphics_print(lcdBuffer, 0, 45, "   Miscellaneous   >");

	graphics_rect(lcdBuffer,
		12, 9*(1 + data->state[data->state_level]), 102, 8, XOR);

	//sprintf(string, "usart: %s", data->test_string);
	//graphics_print(lcdBuffer, 0, 54, string);
	*/
} // engine_graphics_1()



void flappy_graphics(uint8_t lcdBuffer[2][8][64], Data *data)
{
	uint8_t i;
	char string[32];
	//text
	sprintf(string, "Score: %lu", player_score);
	graphics_print(lcdBuffer, 0, 0, string);
	if(gameover)
	{
		sprintf(string, "GAME OVER");
		graphics_print(lcdBuffer, 37, 28, string);
	}

	//bird
	graphics_rect(lcdBuffer,
			PLAYER_HORI, player_vert,
			PLAYER_SIZE, PLAYER_SIZE, XOR);
	//pillar
	for(i=0; i<NUM_PILLARS; i++)
	{
		graphics_rect(lcdBuffer,
			pillars_hori[i], 0,
			PILLAR_WIDTH, pillars_vert[i], XOR);
		graphics_rect(lcdBuffer,
			pillars_hori[i], pillars_vert[i] + PILLAR_GAP,
			PILLAR_WIDTH, 64, XOR);
	} // for pillars
} // flappy_graphics()



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
} // engine_graphic`s_2()
