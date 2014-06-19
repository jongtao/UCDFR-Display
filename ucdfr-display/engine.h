#ifndef ENGINE_H
#define ENGINE_H

#include <stdint.h>
#include <stdio.h>
#include <util/atomic.h>
#include <util/delay.h>

#include <stdlib.h>

#include "graphics.h"
#include "utilities.h"

#define PACK_MAX_MV 116000



// State Levels: Primary -> Selection -> Screen -> Things
enum Primary
{
	S1_SCREEN_ONE,
	S1_SCREEN_TWO,
	S1_SCREEN_THREE,
}; // Primary



enum Selection
{
	S2_BACK,
	S2_1_CONF,
	S2_2_CONF,
	S2_3_CONF,
	S2_MISC
}; // Selection



enum Field
{
	F_BACK,
	F_LED,
	F_BIG,
	F_ONE,
	F_TWO,
	F_THREE,
	F_FOUR,
	F_FIVE,
	F_SIX,
	F_SEVEN
};


// Things to display
enum SmallThings
{
	S_MPH,
	S_KPH,
	S_RAW_WHEEL_SPEED,
	S_TORQUE_RATIO,
	S_RAW_MOTOR_CURRENT,
	S_BATTERY_SPENT_CHARGE,
	S_RAW_BATTERY,
	S_THROTTLE,
	S_BRAKE,
	S_MAG_ACCEL,
	S_RAW_ACCEL,
	S_RAW_YAW
};



enum BigThings
{
	B_MPH,
	B_KPH,
	B_TORQUE_RATIO,
	B_BATTERY_SPENT_CHARGE,
	B_MAG_ACCELL,
	B_OFF
};



enum LEDThings
{
	L_SPEED,
	L_TORQUE_RATIO,
	L_BATTERY,
	L_THROTTLE_BRAKE,
	L_LINEAR_ACCEL,
	L_LATERAL_ACCEL,
	L_YAW,
	L_OFF
};


/*
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
		gyro_yaw,
		ir_temp_left,
		ir_temp_right;
} UsartData;
*/

typedef struct
{
	uint8_t state_of_charge;
	uint16_t status;
	uint16_t balance_mV;
	uint16_t remaining_AH;
	short current_A;
	uint8_t temp_C;
	uint32_t voltage_mV;
} UsartData; // struct Pack



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
	uint8_t test_string[256]; // FIXME


} Data;



void engine_init();
void engine_get_inputs(Data *data, Inputs *inputs);

void engine_logic(Data *data, Inputs *inputs);
void engine_logic_0(Data *data, Inputs *inputs);
void engine_logic_1(Data *data, Inputs *inputs);
void engine_logic_2(Data *data, Inputs *inputs);

void flappy_init(Data *data, Inputs *inputs);
void flappy_logic(Data *data, Inputs *inputs);

void rotary_logic(Data *data, Inputs *inputs, uint8_t state_level,
	uint8_t most);

void engine_put_outputs();
void engine_graphics(uint8_t lcdBuffer[2][8][64], Data *data);
void engine_graphics_0(uint8_t lcdBuffer[2][8][64], Data *data);
void engine_graphics_1(uint8_t lcdBuffer[2][8][64], Data *data);
void flappy_graphics(uint8_t lcdBuffer[2][8][64], Data *data);
void engine_graphics_2(uint8_t lcdBuffer[2][8][64], Data *data);



#endif
