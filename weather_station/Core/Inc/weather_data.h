/*
 * output_structure.h
 *
 *  Created on: Oct 21, 2023
 *      Author: Damian
 */

#ifndef INC_WEATHER_DATA_H_
#define INC_WEATHER_DATA_H_

#include <stdio.h>
#include <stdint.h>

struct weather_data
{
	uint32_t pressure;
	int32_t temperature;
	uint32_t humidity;
	uint16_t ambient_light;
};

extern struct weather_data data_collection;

#endif /* INC_WEATHER_DATA_H_ */
