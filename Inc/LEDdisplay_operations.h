#ifndef _LED_DISPLAY_OPERATIONS_H
#define _LED_DISPLAY_OPERATIONS_H

#include "ST7565.h"

typedef struct InterfaceValues{
	int instruction;
	int numOfInstructions;
	float posX;
	float posY;
	float posZ;
	float tmpH;
	float tmpB;
} InterfaceValues;

void drawInterface();
void updateValues(InterfaceValues *val);

#endif //_LED_DISPLAY_OPERATIONS_H
