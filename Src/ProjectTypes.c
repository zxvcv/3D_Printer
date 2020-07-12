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



/* #######################################################################################################
 *											INCLUDES
 * ####################################################################################################### */

#include "ProjectTypes.h"



/* #######################################################################################################
 *											DEFINES
 * ####################################################################################################### */



/* #######################################################################################################
 *											EXTERNS
 * ####################################################################################################### */



/* #######################################################################################################
 *											DATA TYPES
 * ####################################################################################################### */



/* #######################################################################################################
 *										PUBLIC DEFINITIONS
 * ####################################################################################################### */

Std_Err translate_error_hal_to_project(HAL_StatusTypeDef halStatus){
	Std_Err retVal;

	switch(halStatus){
	case HAL_OK: retVal = STD_OK; break;
	case HAL_ERROR: retVal = STD_ERROR; break;
	case HAL_BUSY: retVal = STD_BUSY_ERROR; break;
	case HAL_TIMEOUT: retVal = STD_TIMEOUT_ERROR; break;
	default: retVal = STD_ERROR; break;
	}
	return retVal;
}
