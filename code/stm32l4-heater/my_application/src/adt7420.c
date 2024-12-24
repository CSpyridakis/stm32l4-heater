/**
 * \author: Spyridakis Christos
 * \date: 2024-12-24
 * \license: MIT License
 * \version: 1.0
 */

#include "adt7420.h"

ADT7420_STATUS adt7420_verify_setup(){
	uint8_t data;

	// [Extracted from the Technical Documentation]
	// 		1. After powering up the ADT7420, verify the setup by reading the device ID (Register Address 0x0B). It should read 0xCB.
	if ( HAL_I2C_Mem_Read(&I2C_USED, ADT_I2C_ADDRESS, ID_REG, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY) != HAL_OK ) {
		return ADT7420_ERROR;
	}
	if ( data != 0xCB ){
		return ADT7420_ERROR;
	}
	// TODO: repeat this multiple times

	// [Extracted from the Technical Documentation]
	// 		2. After consistent consecutive readings are obtained from Step 1, proceed to read the configuration register (0x03),
	// 		TCRIT (0x08, 0x09), THIGH (0x04, 0x05), and T LOW (0x06, 0x07) registers. Compare to the specified defaults in Table 6.
	// 		If all the readings match, the interface is operational

	if ( HAL_I2C_Mem_Read(&I2C_USED, ADT_I2C_ADDRESS, CONF_REG, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY) != HAL_OK ) {
		return ADT7420_ERROR;
	}
	if ( data != 0x00 ){
		return ADT7420_ERROR;
	}
	// TODO: we should also do the same for the rest of them
	// ...

	// [Extracted from the Technical Documentation]
    // 		3. Write to the configuration register to set the ADT7420 to the desired configuration.
    //			- Operation mode: Continues conversion is set 0x00
	//			- Resolution: 16-bit resolution (resolution of 0.0078°C)
	data = 0b10000000;
    if ( HAL_I2C_Mem_Write(&I2C_USED, ADT_I2C_ADDRESS, CONF_REG, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY) != HAL_OK ) {
    	return ADT7420_ERROR;
    }

    return ADT7420_SUCCESS;
}

ADT7420_STATUS adt7420_read_temperature(float* temp){
    uint8_t temperature_data[2];

    // [Extracted from the Technical Documentation]
    // 		Read the temperature value MSB register, followed by the temperature value LSB register. Both registers should
    // 		produce a valid temperature measurement.
    if ( HAL_I2C_Mem_Read(&I2C_USED, ADT_I2C_ADDRESS, TEMP_REG, I2C_MEMADD_SIZE_8BIT, temperature_data, 2, HAL_MAX_DELAY) != HAL_OK) {
        return ADT7420_ERROR;
    }

    // Merge data
    uint16_t temperature_raw = (uint16_t)( (temperature_data[0]<<8) | temperature_data[1] );

    // Convert to Celsius
    if (temperature_raw & 0x8000) {
        // MSB is set -> Negative Temperature
    	// Negative Temperature = (ADC Code (dec) – 65,536)/128
    	*temp = (float)( (temperature_raw - 65536) / 128 );
    } else {
        // MSB is not set -> Positive Temperature
    	// Positive Temperature = ADC Code (dec)/128
    	*temp = (float)(temperature_raw / 128);
    }

    return ADT7420_SUCCESS;
}
