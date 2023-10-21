/*
 * bme280_interface.h
 *
 *  Created on: Oct 21, 2023
 *      Author: Damian
 */

#ifndef INC_BME280_INTERFACE_H_
#define INC_BME280_INTERFACE_H_

#include "weather_data.h"

int8_t BME280_init(void);
int8_t BME280_read_data(struct weather_data *data);

void user_delay_ms(uint32_t period);
int8_t user_i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data,
		uint16_t len);
int8_t user_i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data,
		uint16_t len);

#endif /* INC_BME280_INTERFACE_H_ */
