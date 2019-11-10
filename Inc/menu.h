/*
 * menu.h
 *
 *  Created on: 05.11.2019
 *      Author: zxvcv
 */

#ifndef MENU_H_
#define MENU_H_

#include "ModKB4x4.h"

typedef struct PrinterSettings {
	double maxSpeed;
	double maxPosX;
	double maxPosY;
	double maxPosZ;
	double minStep;

	double position[3];
	int speed[3];
} PrinterSettings;

extern PrinterSettings printerSettings;

void menu_init();
void menu_run(KeyboardButtons buttState);

#endif /* MENU_H_ */
