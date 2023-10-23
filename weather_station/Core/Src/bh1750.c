/*
 * bh1750.c
 *
 *  Created on: Oct 23, 2023
 *      Author: Damian
 */

#include "bh1750.h"
#include "i2c.h"
#include <stdio.h>


int8_t BH1750_read_data(struct weather_data *data)
{
	int8_t rslt = HAL_ERROR;

	uint8_t i2c_data[2];

	rslt = HAL_I2C_Mem_Read(&hi2c1, BH1750_ADDR, ONE_TIME_HIGH_RES_MODE_1, 1,
			i2c_data, 2, 100);

	data->ambient_light = (uint16_t) (i2c_data[0] << 8 | i2c_data[1]);

	return rslt;
}
