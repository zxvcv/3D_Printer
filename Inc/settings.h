#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <stdbool.h>

typedef struct PrinterSettings{
	//Positioning Mode:  Absolute-true, Relative-false
	bool posMode;

	//Speed Value
	double speed;
} PrinterSettings;

extern PrinterSettings printerSettings;

#endif // SETTINGS_H_
