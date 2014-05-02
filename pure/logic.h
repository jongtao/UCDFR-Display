#ifndef LOGIC_H 
#define LOGIC_H
#define F_CPU 16000000UL

#include <Arduino.h>
#include <stdio.h>
#include <util/delay.h>
#include "lcd.h"
#include "graphics.h"

enum States
{
	DRIVING, MENU
};

void logic(uint8_t lcdBuffer[2][8][64]);
void logic_splash(uint8_t lcdBuffer[2][8][64]);
void logic_menu(uint8_t lcdBuffer[2][8][64]);
void logic_demo(uint8_t lcdBuffer[2][8][64]);


#endif
