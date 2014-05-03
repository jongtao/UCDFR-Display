#include "engine.h"



void engine_get_inputs(Inputs inputs)
{
	//Button
	//UART
} // engine_get_inputs()



void engine_process_data(uint8_t lcdBuffer[2][8][64])
{
	char string[8];
	graphics_clear_buffer(lcdBuffer);
	sprintf(string, "%2d.%d", 64,2);
	graphics_num(lcdBuffer, 0, 0, string);
	graphics_print(lcdBuffer, 105, 20, "mph");
	graphics_print(lcdBuffer, 16, 46, "Temp: 40C (ok)\nTorque Bias: 60%");
} // engine_process_data()



void engine_put_outputs(){}
