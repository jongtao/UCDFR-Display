#ifndef ENGINE_H
#define ENGINE_H
/* Button Pins

Button	PC0
A				PE0
B				PE1

*/

#include <stdint.h>
#include <stdio.h>
#include "graphics.h"
#include "utilities.h"


// State Levels: Primary -> Selection -> Screen -> Things
enum Primary
{
	S1_SCREEN_ONE,
	S1_SCREEN_TWO,
	S1_SCREEN_THREE,
	S1_MENU
}; // Primary



enum Selection
{
	S2_BACK,
	S2_S1_CONF,
	S2_S2_CONF,
	S2_S3_CONF,
	S2_MISC
}; // Selection


// Things to display
enum Things
{
	T_SCREEN,
	T_TORQUE
};



typedef struct
{
	int
		speed,
		motor_speed_left,
		motor_speed_right,
		motor_current_left,
		motor_current_right,
		battery_current,
		battery_voltage,
		throttle,
		brake_pressure,
		accel_x,
		accel_y,
		accel_z,
		gyro_x,
		ir_temp_left,
		ir_temp_right;
} UsartData;



typedef struct
{
	int last_button_num, last_detent_num;

	/* Format
	Nibble 1: Bar
	Nibble 2: Area
	Nibble 3: Row 1
	Nibble 4: Row 2
	*/

	unsigned int first_screen, second_screen, third_screen;

	uint8_t state_level;
	uint8_t state[4];

	UsartData usart_data;
	//uint8_t usart_data_current;
	char test_string[256]; // FIXME


} Data;







void engine_init();
void engine_get_inputs(Data *data, Inputs *inputs);

void engine_logic(Data *data, Inputs *inputs);
void primary_logic(Data *data, Inputs *inputs);
void selection_logic(Data *data, Inputs *inputs);
void rotary_logic(Data *data, Inputs *inputs, uint8_t state_level,
	uint8_t most);

void engine_put_outputs();
void engine_graphics(uint8_t lcdBuffer[2][8][64], Data *data);



#endif
