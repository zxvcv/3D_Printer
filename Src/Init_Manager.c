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
#include "Manager_EEPROM.h"
#include "Command_Parser.h"
#include "Manager_Communication.h"
#include "Project_Config.h"
/*[[COMPONENT_INCLUDES_C]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      EXTERNS                                                 *
 * ############################################################################################ */

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim16;



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

Std_Err _init_motors(DeviceSettings* settings)
{
    Std_Err stdErr;
    MotorData_EEPROM eeprom_data;

    // MOTOR_X
    IO_Pin reset_pins_X = { .PORT = MOT1_RESET_GPIO_Port, .PIN = MOT1_RESET_Pin };
    IO_Pin sleep_pins_X = { .PORT = MOT1_SLEEP_GPIO_Port, .PIN = MOT1_SLEEP_Pin };
    IO_Pin direction_pins_X = { .PORT = MOT1_DIRECTION_GPIO_Port, .PIN = MOT1_DIRECTION_Pin };
    IO_Pin step_pins_X = { .PORT = MOT1_STEP_GPIO_Port, .PIN = MOT1_STEP_Pin };
    // IO_Pin step_pins_X = { .PORT = LD2_GPIO_Port, .PIN = LD2_Pin };
    motor_init(
        settings->motors[MOTOR_X], 1000,
        reset_pins_X, sleep_pins_X, direction_pins_X, step_pins_X
    );

    // MOTOR_Y
    IO_Pin reset_pins_Y = { .PORT = MOT2_RESET_GPIO_Port, .PIN = MOT2_RESET_Pin };
    IO_Pin sleep_pins_Y = { .PORT = MOT2_SLEEP_GPIO_Port, .PIN = MOT2_SLEEP_Pin };
    IO_Pin direction_pins_Y = { .PORT = MOT2_DIRECTION_GPIO_Port, .PIN = MOT2_DIRECTION_Pin };
    IO_Pin step_pins_Y = { .PORT = MOT2_STEP_GPIO_Port, .PIN = MOT2_STEP_Pin };
    // IO_Pin step_pins_Y = { .PORT = LD2_GPIO_Port, .PIN = LD2_Pin };
    motor_init(
        settings->motors[MOTOR_Y], 1000,
        reset_pins_Y, sleep_pins_Y, direction_pins_Y, step_pins_Y
    );

    // MOTOR_Z
    IO_Pin reset_pins_Z = { .PORT = MOT3_RESET_GPIO_Port, .PIN = MOT3_RESET_Pin };
    IO_Pin sleep_pins_Z = { .PORT = MOT3_SLEEP_GPIO_Port, .PIN = MOT3_SLEEP_Pin };
    IO_Pin direction_pins_Z = { .PORT = MOT3_DIRECTION_GPIO_Port, .PIN = MOT3_DIRECTION_Pin };
    IO_Pin step_pins_Z = { .PORT = MOT3_STEP_GPIO_Port, .PIN = MOT3_STEP_Pin };
    // IO_Pin step_pins_Z = { .PORT = LD2_GPIO_Port, .PIN = LD2_Pin };
    motor_init(
        settings->motors[MOTOR_Z], 1000,
        reset_pins_Z, sleep_pins_Z, direction_pins_Z, step_pins_Z
    );

    // MOTOR_E
    IO_Pin reset_pins_E = { .PORT = MOT4_RESET_GPIO_Port, .PIN = MOT4_RESET_Pin };
    IO_Pin sleep_pins_E = { .PORT = MOT4_SLEEP_GPIO_Port, .PIN = MOT4_SLEEP_Pin };
    IO_Pin direction_pins_E = { .PORT = MOT4_DIRECTION_GPIO_Port, .PIN = MOT4_DIRECTION_Pin };
    IO_Pin step_pins_E = { .PORT = MOT4_STEP_GPIO_Port, .PIN = MOT4_STEP_Pin };
    motor_init(
        settings->motors[MOTOR_E], 1000,
        reset_pins_E, sleep_pins_E, direction_pins_E, step_pins_E
    );

    /* eeprom data setup */
    for(int i=0; i<MOTORS_NUM; ++i)
    {
        stdErr = get_motor_data_EEPROM(settings->eeprom, settings->motor_data_addresses[i],
            &eeprom_data);
        if(stdErr != STD_OK)
        {
            return stdErr;
        }

        settings->motors[i]->settings.step_size = eeprom_data.step_size;
        settings->motors[i]->settings.max_speed = eeprom_data.max_speed;
        settings->motors[i]->settings.position_zero = eeprom_data.position_zero;
        settings->motors[i]->settings.position_end = eeprom_data.position_end;
    }

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
    HAL_TIM_Base_Start_IT(&htim16);

    EEPROM_init(settings->eeprom, &hi2c1);

    for(int i=0; i<MOTORS_NUM; ++i)
    {
        settings->motor_data_addresses[i] = 0x00 + i * sizeof(MotorData_EEPROM);
    }

    _init_motors(settings);
    init_boundariesDetector_manager(settings->boundaryDetection,
        BOUNDARY_BUTTON_X_MIN_GPIO_Port, BOUNDARY_BUTTON_X_MIN_Pin,
        BOUNDARY_BUTTON_X_MAX_GPIO_Port, BOUNDARY_BUTTON_X_MAX_Pin,
        BOUNDARY_BUTTON_Y_MIN_GPIO_Port, BOUNDARY_BUTTON_Y_MIN_Pin,
        BOUNDARY_BUTTON_Y_MAX_GPIO_Port, BOUNDARY_BUTTON_Y_MAX_Pin,
        BOUNDARY_BUTTON_Z_MIN_GPIO_Port, BOUNDARY_BUTTON_Z_MIN_Pin,
        BOUNDARY_BUTTON_Z_MAX_GPIO_Port, BOUNDARY_BUTTON_Z_MAX_Pin);

    stdErr = init_buffered_communication(settings->buff_comm, &huart2);
    if(stdErr != STD_OK) { return stdErr; }

    HAL_GPIO_WritePin(SDSPI_CS_GPIO_Port, SDSPI_CS_Pin, GPIO_PIN_SET);
    stdErr = init_manager_SDcard(settings->sd, settings->motors, settings->buff_comm,
        &(settings->motors_are_on));
    if(stdErr != STD_OK) { return stdErr; }

    init_communication_manager(settings->communication, settings->buff_comm,
        settings->motors, settings->eeprom, settings->sd, settings->motor_data_addresses);

    return STD_OK;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/
