/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: IOpin
 *      [[COMPONENT_DESCRIPTION]]
 * ============================================================================================
 * COMMENTS:
 *      [[COMPONENT_COMMENTS]]
 * ============================================================================================
 * EXAMPLE:
 *      [[COMPONENT_EXAMPLE]]
 ************************************************************************************************/

#ifndef IOPIN_H_
#define IOPIN_H_


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include <stdbool.h>
#include "stm32f3xx_hal.h"
/*[[COMPONENT_INCLUDES_H]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

/*[[COMPONENT_DEFINES_H]]*/



/* ############################################################################################ *
 *                                      EXTERNS                                                 *
 * ############################################################################################ */

/*[[COMPONENT_EXTERNS_H]]*/



/* ############################################################################################ *
 *                                      DATA TYPES                                              *
 * ############################################################################################ */

typedef struct IO_Pin_Tag{
    GPIO_TypeDef* PORT;
    uint16_t PIN;
}IO_Pin;

typedef struct IO_Pin_IT_Tag{
    GPIO_TypeDef* PORT;
    uint16_t PIN;

    enum{
        PRESSED,
        RELEASED
    }state;
    bool event_received;
    uint8_t vibrations_delay_counter;
}IO_Pin_IT;
/*[[COMPONENT_DATA_TYPES_H]]*/



/* ############################################################################################ *
 *                                      PUBLIC DECLARATIONS                                     *
 * ############################################################################################ */

void IOpin_init(IO_Pin* settings, GPIO_TypeDef* port, uint16_t pin);

void IOpin_IO_init(IO_Pin_IT* settings, GPIO_TypeDef* port, uint16_t pin);

void IOpin_set_state(IO_Pin_IT* settings, bool state);

void IOpin_set_event_received(IO_Pin_IT* settings, bool state);

void IOpin_set_vibrations_delay_counter(IO_Pin_IT* settings);

bool IOpin_subtract_vibrations_delay_counter(IO_Pin_IT* settings);

void IOpin_check_pin_IT(IO_Pin_IT* settings);
/*[[COMPONENT_PUBLIC_DECLARATIONS]]*/



#endif /* IOPIN_H_ */
