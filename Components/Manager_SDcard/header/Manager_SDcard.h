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
#include "A4988_stepstick.h"
#include "FIFO_void.h"
#include "SD.h"
#include "GCode_Parser.h"
#include "Buffered_Communication.h"
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
    FATFS* fatfs;
    FIL* file;
    BuffCommunication_Settings* buff_comm;
    GCode_Settings gcode;

    Fifo_C* BuffIN_SDcmd;
    GCodeCommand executingCmd;

    uint8_t activeTab;
    uint8_t unactiveTab;
    uint8_t dataSDin[2][BYTES_TO_READ];
    uint8_t counterTab[2];
    UINT bytesRead;
    uint8_t cnt;
    int program_line_counter;

    struct{
        bool eofRecieved;
        bool end_program;
        bool executing_program;
        bool executing_command;
    }flags;
}SDCard_Settings;
/*[[COMPONENT_DATA_TYPES_H]]*/



/* ############################################################################################ *
 *                                      PUBLIC DECLARATIONS                                     *
 * ############################################################################################ */

Err_Msg translate_error_sdcard_to_project(FRESULT sdStatus);

Std_Err init_manager_SDcard(SDCard_Settings* settings, Motor** motors,
    BuffCommunication_Settings* buff_comm, bool* motors_are_on);

Std_Err parse_command_SDcard(SDCard_Settings* settings);

Std_Err execute_command_SDcard(SDCard_Settings* settings);

Err_Msg sdcard_open_file(SDCard_Settings* settings, const char* file_name, BYTE mode);

Err_Msg sdcard_close_file(SDCard_Settings* settings);
/*[[COMPONENT_PUBLIC_DECLARATIONS]]*/



#endif /* MANAGER_SDCARD_H_ */
