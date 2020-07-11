/* #######################################################################################################
 *											INTRODUCTION
 * ####################################################################################################### */
/* *********************************************************************************************************
 *
 * =======================================================================================================
 * COMMENTS:
 *
 * =======================================================================================================
 * EXAMPLE:
 *
 ********************************************************************************************************** */

#ifndef USER_INTERFACE_H_
#define USER_INTERFACE_H_



/* #######################################################################################################
 *											INCLUDES
 * ####################################################################################################### */

#include "ST7565.h"



/* #######################################################################################################
 *											DEFINES
 * ####################################################################################################### */



/* #######################################################################################################
 *											DATA TYPES
 * ####################################################################################################### */

typedef struct InterfaceValues{
	int instruction;
	int numOfInstructions;
	float posX;
	float posY;
	float posZ;
	float tmpH;
	float tmpB;
} InterfaceValues;



/* #######################################################################################################
 *											EXTERNS
 * ####################################################################################################### */



/* #######################################################################################################
 *										PUBLIC DECLARATIONS
 * ####################################################################################################### */

void drawInterface(ST7565R_Settings* settings);
void updateValues(InterfaceValues *val, ST7565R_Settings* settings);



#endif /*USER_INTERFACE_H_*/
