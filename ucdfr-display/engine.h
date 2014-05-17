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
	S2_SCREEN_ONE,
	S2_SCREEN_TWO,
	S2_SCREEN_THREE,
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
		time,
		motor_current_left,
		motor_current_right,
		motor_battery_current,
		motor_battery_voltage,
		throttle,
		brake_pressure,
		accel_x,
		accel_y,
		accel_z,
		gyro_x,
		gyro_y,
		gyro_z,
		ir_temp_left,
		ir_temp_right;
} UsartData;



typedef struct
{
	int last_button_num;

	/* Format
	Nibble 1: Bar
	Nibble 2: Area
	Nibble 3: Row 1
	Nibble 4: Row 2
	*/

	unsigned int first_screen, second_screen, third_screen;

	uint8_t state_level;
	uint8_t state[4];

	UsartData usart_data[8];
	//uint8_t usart_data_current;
	char test_string[256]; // FIXME


} Data;







void engine_init();
void engine_get_inputs(Data *data, Inputs *inputs);
void engine_logic(Data *data, Inputs *inputs);
void engine_put_outputs();
void engine_graphics(uint8_t lcdBuffer[2][8][64], Data *data);



#endif
