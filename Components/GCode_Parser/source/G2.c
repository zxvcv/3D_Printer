/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: GCode_Parser
 *      [[COMPONENT_DESCRIPTION]]
 ************************************************************************************************/


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include "_commands.h"
/*[[COMPONENT_INCLUDES_C]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

Std_Err step_G2(GCode_Settings* settings, GCodeCommand* cmd)
{
    if(!(*(settings->motors_are_on)))
    {
        if(compare_points(/*current_point*/, /*end_point*/, ((double)1)/ACCURACY))
        {
            cmd->step = NULL;
            return STD_OK;
        }

        if(/*fifo_size*/ > 0)
        {
            // get data to ste the move
            // begin move
        }
    }

    if(/*fifo_size*/ < /*MAX_FIFO_SIZE*/ && /*move data not ends yet*/)
    {
        // get next circle line
        // calculate move values
        // add calculated move values to fifo
    }
}


Std_Err init_G2(GCode_Settings* settings, GCodeCommand* cmd)
{
    Std_Err stdErr = STD_OK;

    cmd->remove = NULL;
    cmd->step = step_G2;

    // check length of radius
    // init fifo

    return stdErr;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/
