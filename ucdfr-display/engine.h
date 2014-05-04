#ifndef ENGINE_H
#define ENGINE_H
/* Button Pins

Button	PC0
A				PE0
B				PE1

*/

#include <stdint.h>
#include "graphics.h"



void engine_init();
void engine_get_inputs();
void engine_process_data();
void engine_put_outputs();
void engine_graphics(uint8_t lcdBuffer[2][8][64]);



#endif
