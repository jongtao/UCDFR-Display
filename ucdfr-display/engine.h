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
	/* Format
	Nibble 1: Area
	Nibble 2: Row 1
	Nibble 3: Row 2
	Nibble 4: Bar
	*/

	unsigned int first_screen, second_screen, third_screen;
	uint8_t state_level;
	uint8_t state[3];

	/*
	retreived data
	*/

} Data;




void engine_init();
void engine_get_inputs(Inputs *inputs, uint8_t *usart_byte);
void engine_logic(Data *data, Inputs *inputs, uint8_t *usart_byte);
void engine_put_outputs();
void engine_graphics(uint8_t lcdBuffer[2][8][64]);



#endif
