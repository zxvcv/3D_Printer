/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: Manger_SDcard
 *      [[COMPONENT_DESCRIPTION]]
 * ============================================================================================
 * COMMENTS:
 *      [[COMPONENT_COMMENTS]]
 * ============================================================================================
 * EXAMPLE:
 *      [[COMPONENT_EXAMPLE]]
 ************************************************************************************************/

#ifndef MANAGER_SDCARD_H_
#define MANAGER_SDCARD_H_


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include <stdint.h>
#include <stdbool.h>
#include "Error_Codes.h"
#include "IOpin.h"
#include "FIFO_void.h"
#include "SD.h"
/*[[COMPONENT_INCLUDES_H]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

#define BYTES_TO_READ 50
/*[[COMPONENT_DEFINES_H]]*/



/* ############################################################################################ *
 *                                      EXTERNS                                                 *
 * ############################################################################################ */

/*[[COMPONENT_EXTERNS_H]]*/



/* ############################################################################################ *
 *                                      DATA TYPES                                              *
 * ############################################################################################ */

typedef struct SDCard_Settings_Tag{
    FIL* file;

    Fifo_C* BuffIN_SDcmd;

    uint8_t activeTab;
    uint8_t unactiveTab;
    uint8_t dataSDin[2][BYTES_TO_READ];
    uint8_t counterTab[2];
    UINT bytesRead;
    uint8_t cnt;
}SDCard_Settings;

typedef struct SDCard_Flags_Tag{
    bool eofRecieved;
    bool end_program;
    bool executing_program;
    bool executing_command;
}SDCard_Flags;
/*[[COMPONENT_DATA_TYPES_H]]*/



/* ############################################################################################ *
 *                                      PUBLIC DECLARATIONS                                     *
 * ############################################################################################ */

Std_Err init_manager_SDcard(SDCard_Settings* settings, FIL* file, Motor* motors);

Std_Err parse_command_SDcard(SDCard_Settings* settings);

Std_Err execute_command_SDcard(DeviceSettings* settings);
/*[[COMPONENT_PUBLIC_DECLARATIONS]]*/



#endif /* MANAGER_SDCARD_H_ */
