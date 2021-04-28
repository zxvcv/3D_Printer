/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: Project_Config
 *      [[COMPONENT_DESCRIPTION]]
 * ============================================================================================
 * COMMENTS:
 *      [[COMPONENT_COMMENTS]]
 * ============================================================================================
 * EXAMPLE:
 *      [[COMPONENT_EXAMPLE]]
 ************************************************************************************************/

#ifndef PROJECT_CONFIG_H_
#define PROJECT_CONFIG_H_


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

/*[[COMPONENT_INCLUDES_H]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

#define ACCURACY 1000
#define MOTORS_NUM 4

#define MOTOR_X     0
#define MOTOR_Y     1
#define MOTOR_Z     2
#define MOTOR_E     3

#ifndef TESTS
#define USE_INTERRUPTS
#endif
//#define LOG_ENABLE

#ifdef USE_INTERRUPTS
#define IRQ_ENABLE __enable_irq()
#define IRQ_DISABLE __disable_irq()
#endif /* USE_INTERRUPTS */
/*[[COMPONENT_DEFINES_H]]*/



/* ############################################################################################ *
 *                                      EXTERNS                                                 *
 * ############################################################################################ */

extern char motor_indentyficator[MOTORS_NUM];
/*[[COMPONENT_EXTERNS_H]]*/



/* ############################################################################################ *
 *                                      DATA TYPES                                              *
 * ############################################################################################ */

/*[[COMPONENT_DATA_TYPES_H]]*/



/* ############################################################################################ *
 *                                      PUBLIC DECLARATIONS                                     *
 * ############################################################################################ */

/*[[COMPONENT_PUBLIC_DECLARATIONS]]*/



#endif /* PROJECT_CONFIG_H_ */