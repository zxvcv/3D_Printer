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

Std_Err command_G28(GCodeCommand* cmd, DeviceSettings* settings)
{
    /*TODO: check gCode state before start executing command*/
    Std_Err stdErr = STD_OK;
    bool motorErr = false;
    RoundingErrorData roundingError;

    //temporarty, in final version it could be maxSpeed of exry axis
    settings->motors[MOTOR_X]->data.speed = settings->speed;
    settings->motors[MOTOR_Y]->data.speed = settings->speed;
    settings->motors[MOTOR_Z1]->data.speed = settings->speed;
    settings->motors[MOTOR_Z2]->data.speed = settings->speed;

    if(cmd->usedFields._x == 1)
    {
        motorErr = motorSetMove(settings->motors[MOTOR_X], 
                                -((double)settings->motors[MOTOR_X]->data.position/ACCURACY),
                                &roundingError);
        if(motorErr)
        {
            return STD_ERROR;
        }
    }
        
    if(cmd->usedFields._y == 1)
    {
        motorErr = motorSetMove(settings->motors[MOTOR_Y], 
                                -((double)settings->motors[MOTOR_Y]->data.position/ACCURACY),
                                &roundingError);
        if(motorErr)
        {
            return STD_ERROR;
        }
    }
        
    if(cmd->usedFields._z == 1)
    {
        motorErr = motorSetMove(settings->motors[MOTOR_Z1],
                                    -((double)settings->motors[MOTOR_Z1]->data.position/ACCURACY),
                                    &roundingError);
        if(motorErr)
        {
            return STD_ERROR;
        }
        
        motorErr = motorSetMove(settings->motors[MOTOR_Z2], 
                                -((double)settings->motors[MOTOR_Z2]->data.position/ACCURACY),
                                &roundingError);
        if(motorErr)
        {
            return STD_ERROR;
        }
    }

    #ifdef USE_INTERRUPTS
        IRQ_DISABLE;
    #endif /* USE_INTERRUPTS */

    motorErr |= motorStart(settings->motors[MOTOR_X]);
    motorErr |= motorStart(settings->motors[MOTOR_Y]);
    motorErr |= motorStart(settings->motors[MOTOR_Z1]);
    motorErr |= motorStart(settings->motors[MOTOR_Z2]);

    if(motorErr)
    {
        #ifdef USE_INTERRUPTS
            IRQ_ENABLE;
        #endif /* USE_INTERRUPTS */
        return STD_ERROR;
    }

    #ifdef USE_INTERRUPTS
        IRQ_ENABLE;
    #endif /* USE_INTERRUPTS */

    settings->sdCommandState = BUSY;

    bool state;
    do{
        state = motorIsOn(settings->motors[MOTOR_X]) | 
                motorIsOn(settings->motors[MOTOR_Y]) | 
                motorIsOn(settings->motors[MOTOR_Z1]) | 
                motorIsOn(settings->motors[MOTOR_Z2]);
    }while(state);

    settings->sdCommandState = IDLE;

    return stdErr;
}
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/
