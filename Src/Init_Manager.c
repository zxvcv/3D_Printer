/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: Init_Manager
 *      [[COMPONENT_DESCRIPTION]]
 ************************************************************************************************/


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include "Init_Manager.h"
#include "Project_Objects.h"
#include "Manager_EEPROM.h"
#include "Command_Parser.h"
/*[[COMPONENT_INCLUDES_C]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

#ifdef USE_INTERRUPTS
#define IRQ_ENABLE __enable_irq()
#define IRQ_DISABLE __disable_irq()
#endif /* USE_INTERRUPTS */
/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      EXTERNS                                                 *
 * ############################################################################################ */

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim6;



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

Std_Err _init_motors(DeviceSettings* settings)
{
    Std_Err stdErr;
    MotorData_EEPROM eeprom_data;

    for(int i=0; i<MOTORS_NUM; ++i)
    {
        motor_init(settings->motors[i]);
    }

    /* pin setup */
    // MOTOR_X
    settings->motors[0].IOreset.PORT = MOT1_RESET_GPIO_Port;
    settings->motors[0].IOreset.PIN = MOT1_RESET_Pin;
    settings->motors[0].IOsleep.PORT = MOT1_SLEEP_GPIO_Port;
    settings->motors[0].IOsleep.PIN = MOT1_SLEEP_Pin;
    settings->motors[0].IOdirection.PORT = MOT1_DIRECTION_GPIO_Port;
    settings->motors[0].IOdirection.PIN = MOT1_DIRECTION_Pin;
    settings->motors[0].IOstep.PORT = MOT1_STEP_GPIO_Port;
    settings->motors[0].IOstep.PIN = MOT1_STEP_Pin;
    //settings->motors[0].IOstep.PORT = LD2_GPIO_Port;  //[DEBUG]
    //settings->motors[0].IOstep.PIN = LD2_Pin;         //[DEBUG]

    // MOTOR_Y
    settings->motors[1].IOreset.PORT = MOT2_RESET_GPIO_Port;
    settings->motors[1].IOreset.PIN = MOT2_RESET_Pin;
    settings->motors[1].IOsleep.PORT = MOT2_SLEEP_GPIO_Port;
    settings->motors[1].IOsleep.PIN = MOT2_SLEEP_Pin;
    settings->motors[1].IOdirection.PORT = MOT2_DIRECTION_GPIO_Port;
    settings->motors[1].IOdirection.PIN = MOT2_DIRECTION_Pin;
    settings->motors[1].IOstep.PORT = MOT2_STEP_GPIO_Port;
    settings->motors[1].IOstep.PIN = MOT2_STEP_Pin;

    // MOTOR_Z
    settings->motors[2].IOreset.PORT = MOT3_RESET_GPIO_Port;
    settings->motors[2].IOreset.PIN = MOT3_RESET_Pin;
    settings->motors[2].IOsleep.PORT = MOT3_SLEEP_GPIO_Port;
    settings->motors[2].IOsleep.PIN = MOT3_SLEEP_Pin;
    settings->motors[2].IOdirection.PORT = MOT3_DIRECTION_GPIO_Port;
    settings->motors[2].IOdirection.PIN = MOT3_DIRECTION_Pin;
    settings->motors[2].IOstep.PORT = MOT3_STEP_GPIO_Port;
    settings->motors[2].IOstep.PIN = MOT3_STEP_Pin;

    // MOTOR_E
    settings->motors[3].IOreset.PORT = MOT4_RESET_GPIO_Port;
    settings->motors[3].IOreset.PIN = MOT4_RESET_Pin;
    settings->motors[3].IOsleep.PORT = MOT4_SLEEP_GPIO_Port;
    settings->motors[3].IOsleep.PIN = MOT4_SLEEP_Pin;
    settings->motors[3].IOdirection.PORT = MOT4_DIRECTION_GPIO_Port;
    settings->motors[3].IOdirection.PIN = MOT4_DIRECTION_Pin;
    settings->motors[3].IOstep.PORT = MOT4_STEP_GPIO_Port;
    settings->motors[3].IOstep.PIN = MOT4_STEP_Pin;


    /* eeprom data setup */
    for(int i=0; i<MOTORS_NUM; ++i)
    {
        stdErr = get_motor_data_EEPROM(settings->eeprom, settings->motor_data_addresses[i], eeprom_data);
        if(stdErr != STD_OK)
        {
            return stdErr;
        }

        settings->motors[i].settings.step_size = eeprom_data.step_size;
        settings->motors[i].settings.max_speed = eeprom_data.max_speed;
        settings->motors[i].settings.position_zero = eeprom_data.position_zero;
        settings->motors[i].settings.position_end = eeprom_data.position_end;
    }


    /* others data setup */
    // MOTOR_X
    settings->motors[0].settings.timer_frequency = 1000;

    // MOTOR_Y
    settings->motors[1].settings.timer_frequency = 1000;

    // MOTOR_Z
    settings->motors[2].settings.timer_frequency = 1000;

    // MOTOR_E
    settings->motors[3].settings.timer_frequency = 1000;

    return STD_OK;
}
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

Std_Err init_manager(DeviceSettings* settings)
{
    Std_Err stdErr;

    init_deviceSettings(settings);

    HAL_TIM_Base_Start_IT(&htim6);

    init_motors(settings);

    EEPROM_init(settings->eeprom, &hi2c1);

    stdErr = init_communication_manager(settings->buff_comm, &huart2);
    if(stdErr != STD_OK)
    {
        return stdErr;
    }

    HAL_GPIO_WritePin(SDSPI_CS_GPIO_Port, SDSPI_CS_Pin, GPIO_PIN_SET);
    stdErr = init_manager_SDcard(settings->sd, settings->file, settings->motors);
    if(stdErr != STD_OK)
    {
        return stdErr;
    }

    return STD_OK;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/
