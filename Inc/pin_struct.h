/*
 * pin_struct.h
 *
 *  Created on: 25.10.2019
 *      Author: zxvcv
 */

#ifndef PIN_STRUCT_H_
#define PIN_STRUCT_H_

#include "main.h"

typedef struct{
	GPIO_TypeDef* const PORT;
	const uint16_t PIN;
}IO_Pin;

#endif /* PIN_STRUCT_H_ */
