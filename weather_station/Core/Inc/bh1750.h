/*
 * bh1750.h
 *
 *  Created on: Oct 23, 2023
 *      Author: Damian
 */

#ifndef INC_BH1750_H_
#define INC_BH1750_H_

#include "weather_data.h"

#define BH1750_ADDR 0x23<<1

// Instruction set
#define POWER_DOWN 0b00000000					// No active state
#define POWER_ON 0b00000001						// Waiting for measurement command.
#define RESET_DATA_REGISTER_VAL 0b00000111		// Reset data register value. Reset command is not acceptable in Power Down mode.

#define CONTINUOUS_HIGH_RES_MODE_1 0b00010000	// Start measurement at 1lx resolution. Measurement Time is typically 120ms.
#define CONTINUOUS_HIGH_RES_MODE_2 0b00010001	// Start measurement at 0.5lx resolution. Measurement Time is typically 120ms.
#define CONTINUOUS_LOW_RES_MODE 0b00010011		// Start measurement at 4lx resolution. Measurement Time is typically 16ms.

#define ONE_TIME_HIGH_RES_MODE_1 0b00100000		// Start measurement at 1lx resolution. Measurement Time is typically 120ms. Device is automatically set to Power Down after measurement.
#define ONE_TIME_HIGH_RES_MODE_2 0b00100001		// Start measurement at 0.5lx resolution. Measurement Time is typically 120ms. Device is automatically set to Power Down after measurement.
#define ONE_TIME_LOW_RES_MODE 0b00100011		// Start measurement at 4lx resolution. Measurement Time is typically 16ms. Device is automatically set to Power Down after measurement.

int8_t BH1750_read_data(struct weather_data *data);

#endif /* INC_BH1750_H_ */
