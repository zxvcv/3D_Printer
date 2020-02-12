#ifndef _SETTINGS_H
#define _SETTINGS_H

typedef struct PrinterSettings {
	double maxSpeed;
	double maxPos[3];
	double minStep;

	double position[3];
	int speed[3];
} PrinterSettings;

extern PrinterSettings printerSettings;

#endif
