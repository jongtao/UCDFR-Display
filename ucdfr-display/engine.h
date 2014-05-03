#ifndef ENGINE_H
#define ENGINE_H

//#include <stdlib.h>
#include <stdint.h>

#include "graphics.h"



typedef struct
{
	uint8_t button, angle_segment;
} Inputs;



void engine_get_inputs();
void engine_process_data(uint8_t lcdBuffer[2][8][64]);
void engine_put_outputs();



#endif
