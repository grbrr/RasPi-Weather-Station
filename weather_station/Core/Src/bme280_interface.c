/*
 * bme280_interface.c
 *
 *  Created on: Oct 21, 2023
 *      Author: Damian
 */

#include "bme280.h"
#include "bme280_interface.h"
#include "i2c.h"
#include <stdio.h>

static struct bme280_dev dev;
static struct bme280_data comp_data;
static int8_t init_done;

int8_t BME280_init(void)
{
	int8_t rslt = BME280_OK;
	uint8_t settings_sel;
	init_done = BME280_E_DEV_NOT_FOUND;

	dev.dev_id = (BME280_I2C_ADDR_PRIM << 1);
	dev.intf = BME280_I2C_INTF;
	dev.read = user_i2c_read;
	dev.write = user_i2c_write;
	dev.delay_ms = user_delay_ms;

	rslt = bme280_init(&dev);

	/* Recommended mode of operation: Indoor navigation */
	dev.settings.osr_h = BME280_OVERSAMPLING_1X;
	dev.settings.osr_p = BME280_OVERSAMPLING_16X;
	dev.settings.osr_t = BME280_OVERSAMPLING_2X;
	dev.settings.filter = BME280_FILTER_COEFF_16;
	dev.settings.standby_time = BME280_STANDBY_TIME_62_5_MS;

	if (rslt == BME280_OK)
	{
		settings_sel = BME280_OSR_PRESS_SEL;
		settings_sel |= BME280_OSR_TEMP_SEL;
		settings_sel |= BME280_OSR_HUM_SEL;
		settings_sel |= BME280_STANDBY_SEL;
		settings_sel |= BME280_FILTER_SEL;

		rslt = bme280_set_sensor_settings(settings_sel, &dev);

		if (rslt == BME280_OK)
		{
			rslt = bme280_set_sensor_mode(BME280_NORMAL_MODE, &dev);
			init_done = rslt;
		}
	}

	return rslt;
}

/* read data in normal mode */
int8_t BME280_read_data(struct weather_data *data)
{
	int8_t rslt = BME280_E_COMM_FAIL;

	if (init_done == BME280_OK)
	{
		rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, &dev);
		dev.delay_ms(70);
		data->humidity = comp_data.humidity;
		data->pressure = comp_data.pressure;
		data->temperature = comp_data.temperature;
	}

	return rslt;
}

void user_delay_ms(uint32_t period)
{
	/*
	 * Return control or wait,
	 * for a period amount of milliseconds
	 */
	HAL_Delay(period);
}

int8_t user_i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data,
		uint16_t len)
{
	int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */

	/*
	 * The parameter dev_id can be used as a variable to store the I2C address of the device
	 */

	/*
	 * Data on the bus should be like
	 * |------------+---------------------|
	 * | I2C action | Data                |
	 * |------------+---------------------|
	 * | Start      | -                   |
	 * | Write      | (reg_addr)          |
	 * | Stop       | -                   |
	 * | Start      | -                   |
	 * | Read       | (reg_data[0])       |
	 * | Read       | (....)              |
	 * | Read       | (reg_data[len - 1]) |
	 * | Stop       | -                   |
	 * |------------+---------------------|
	 */

	HAL_I2C_Mem_Read(&hi2c1, dev_id, reg_addr, 1, reg_data, len, 100);
	return rslt;
}

int8_t user_i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data,
		uint16_t len)
{
	int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */

	/*
	 * The parameter dev_id can be used as a variable to store the I2C address of the device
	 */

	/*
	 * Data on the bus should be like
	 * |------------+---------------------|
	 * | I2C action | Data                |
	 * |------------+---------------------|
	 * | Start      | -                   |
	 * | Write      | (reg_addr)          |
	 * | Write      | (reg_data[0])       |
	 * | Write      | (....)              |
	 * | Write      | (reg_data[len - 1]) |
	 * | Stop       | -                   |
	 * |------------+---------------------|
	 */

	HAL_I2C_Mem_Write(&hi2c1, dev_id, reg_addr, 1, reg_data, len, 100);
	return rslt;
}

