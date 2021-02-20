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

Std_Err command_G1(GCodeCommand* cmd, DeviceSettings* settings)
{
    /*TODO: check gCode state before start executing command*/
    Std_Err stdErr = STD_OK;
    vect3D_d move;
    move.x = 0;
    move.y = 0;
    move.z = 0;

    if(settings->positioningMode == RELATIVE)
    {
        move.x = cmd->data.x + (double)settings->motors[MOTOR_X]->data.err.moveError / ACCURACY;
        move.y = cmd->data.y + (double)settings->motors[MOTOR_Y]->data.err.moveError / ACCURACY;
        move.z = cmd->data.z + (double)settings->motors[MOTOR_Z1]->data.err.moveError / ACCURACY;
    }
    else if(settings->positioningMode == ABSOLUTE)
    {
        move.x = cmd->usedFields._x == 1 ? cmd->data.x - (double)settings->motors[MOTOR_X]->data.position / ACCURACY
                // + (double)motors[0].data.err.roundingMoveError / ACCURACY
                : (double)settings->motors[MOTOR_X]->data.err.moveError / ACCURACY;

        move.y = cmd->usedFields._y == 1 ? cmd->data.y - (double)settings->motors[MOTOR_Y]->data.position / ACCURACY
                // + (double)motors[1].data.err.roundingMoveError / ACCURACY
                : (double)settings->motors[MOTOR_Y]->data.err.moveError / ACCURACY;

        move.z = cmd->usedFields._z == 1 ? cmd->data.z - (double)settings->motors[MOTOR_Z1]->data.position / ACCURACY
                // + (double)motors[2].data.err.roundingMoveError / ACCURACY
                : (double)settings->motors[MOTOR_Z1]->data.err.moveError / ACCURACY;

        //clearAllMotorsRoundingErrors(&printerSettings);
    }

    if(cmd->usedFields._f == 1)
    {
        settings->speed = cmd->data.f;
    }

    vect3D_d velocity = getVelocity3D(move, settings->speed);

    settings->motors[MOTOR_X]->data.speed = fabs(velocity.x);
    settings->motors[MOTOR_Y]->data.speed = fabs(velocity.y);
    settings->motors[MOTOR_Z1]->data.speed = fabs(velocity.z);
    settings->motors[MOTOR_Z2]->data.speed = fabs(velocity.z);

    #ifdef LOG_ENABLE
        #include "FIFO_void.h"
        extern List* BuffOUT_logs;
        uint8_t data[100], sizee;
        sizee = sprintf(data, "$CntVel: %10.5f, %10.5f, %10.5f, %10.5f\r\n", velocity.x, velocity.y, velocity.z, velocity.z);
        List_Push_C(BuffOUT_logs, data, sizee);
        sizee = sprintf(data, "$CntMov: %10.5f, %10.5f, %10.5f, %10.5f\r\n", move.x, move.y, move.z, move.z);
        List_Push_C(BuffOUT_logs, data, sizee);
    #endif /*LOG_ENABLE*/

    /* DEBUG TEMP CODE [0] */
    uint8_t data[100], dataSize;
    dataSize = sprintf((char*)data, "BEFORE POSITION: %10d, %10d, %10d, %10d\n", settings->motors[MOTOR_X]->data.position,
                                                                            settings->motors[MOTOR_Y]->data.position,
                                                                            settings->motors[MOTOR_Z1]->data.position,
                                                                            settings->motors[MOTOR_Z2]->data.position);
    HAL_UART_Transmit(settings->outComm->huart, data, dataSize, 1000);
    /* END DEBUG TEMP CODE [1] */

    /* DEBUG TEMP CODE [1] */
    dataSize = sprintf((char*)data, "DATA_MOVE: %10.5f, %10.5f, %10.5f, %10.5f\n", move.x, move.y, move.z, move.z);
    HAL_UART_Transmit(settings->outComm->huart, data, dataSize, 1000);
    dataSize = sprintf((char*)data, "DATA_VELOCITY: %10.5f, %10.5f, %10.5f, %10.5f\n", velocity.x, velocity.y, velocity.z, velocity.z);
    HAL_UART_Transmit(settings->outComm->huart, data, dataSize, 1000);
    //fifo_push_C(settings->outComm->Buff_OUT, data, dataSize);
    /* END DEBUG TEMP CODE [1] */

    stdErr = motorSetMove(settings->motors[MOTOR_X], move.x, &(settings->motors[MOTOR_X]->data.err));
    if(stdErr != STD_OK)
    {
        return stdErr;
    }

    stdErr = motorSetMove(settings->motors[MOTOR_Y], move.y, &(settings->motors[MOTOR_Y]->data.err));
    if(stdErr != STD_OK)
    {
        return stdErr;
    }

    stdErr = motorSetMove(settings->motors[MOTOR_Z1], move.z, &(settings->motors[MOTOR_Z1]->data.err));
    if(stdErr != STD_OK)
    {
        return stdErr;
    }

    stdErr = motorSetMove(settings->motors[MOTOR_Z2], move.z, &(settings->motors[MOTOR_Z2]->data.err));
    if(stdErr != STD_OK)
    {
        return stdErr;
    }

    /* DEBUG TEMP CODE [2] */
    dataSize = sprintf((char*)data, "ERROR_MOVE: %10d, %10d, %10d, %10d\n", settings->motors[MOTOR_X]->data.err.moveError,
                                                                        settings->motors[MOTOR_Y]->data.err.moveError,
                                                                        settings->motors[MOTOR_Z1]->data.err.moveError,
                                                                        settings->motors[MOTOR_Z2]->data.err.moveError);
    HAL_UART_Transmit(settings->outComm->huart, data, dataSize, 1000);
    //fifo_push_C(settings->outComm->Buff_OUT, data, dataSize);
    /* END DEBUG TEMP CODE [2] */

    #ifdef USE_INTERRUPTS
        IRQ_DISABLE;
    #endif /* USE_INTERRUPTS */

    stdErr = motorStart(settings->motors[MOTOR_X]);
    if(stdErr != STD_OK && stdErr != STD_PARAMETER_ERROR)
    {
        for(int j=0; j < MOTORS_NUM; ++j)
        {
            motorStop(settings->motors[j]);
        }

        return stdErr;
    }

    stdErr = motorStart(settings->motors[MOTOR_Y]);
    if(stdErr != STD_OK && stdErr != STD_PARAMETER_ERROR)
    {
        for(int j=0; j < MOTORS_NUM; ++j)
        {
            motorStop(settings->motors[j]);
        }

        return stdErr;
    }

    stdErr = motorStart(settings->motors[MOTOR_Z1]);
    if(stdErr != STD_OK && stdErr != STD_PARAMETER_ERROR)
    {
        for(int j=0; j < MOTORS_NUM; ++j)
        {
            motorStop(settings->motors[j]);
        }

        return stdErr;
    }

    stdErr = motorStart(settings->motors[MOTOR_Z2]);
    if(stdErr != STD_OK && stdErr != STD_PARAMETER_ERROR)
    {
        for(int j=0; j < MOTORS_NUM; ++j)
        {
            motorStop(settings->motors[j]);
        }

        return stdErr;
    }

    stdErr = STD_OK;

    #ifdef USE_INTERRUPTS
        IRQ_ENABLE;
    #endif /* USE_INTERRUPTS */

    bool state;
    do
    {
        state = motorIsOn(settings->motors[MOTOR_X]) | 
                motorIsOn(settings->motors[MOTOR_Y]) | 
                motorIsOn(settings->motors[MOTOR_Z1]) | 
                motorIsOn(settings->motors[MOTOR_Z2]);
    }while(state);

    /* DEBUG TEMP CODE [4] */
    dataSize = sprintf((char*)data, "AFTER POSITION: %10d, %10d, %10d, %10d\n", settings->motors[MOTOR_X]->data.position,
                                                                            settings->motors[MOTOR_Y]->data.position,
                                                                            settings->motors[MOTOR_Z1]->data.position,
                                                                            settings->motors[MOTOR_Z2]->data.position);
    HAL_UART_Transmit(settings->outComm->huart, data, dataSize, 1000);
    dataSize = sprintf((char*)data, "--------------------------\n");
    HAL_UART_Transmit(settings->outComm->huart, data, dataSize, 1000);
    //fifo_push_C(settings->outComm->Buff_OUT, data, dataSize);
    /* END DEBUG TEMP CODE [4] */

    /*TODO: check is it correct*/
    settings->sdCommandState = BUSY;
    return stdErr;
}
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/
