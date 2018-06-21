/*
 * seeed_bme680.h
 * Library for BME680
 *
 * Copyright (c) 2013 Seeed Technology Co., Ltd.
 * Author        :   downey
 * Create Time   :   2017/12/08
 * Change Log    :
*/
#ifndef _SEEED_BME680_H
#define _SEEED_BME680_H

/**@file seeed_bme680.h
 * @author downey
 * @data 2017/12/08
 * @brief Include file.
 */


#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "bme680.h"



#define BME680_DEFAULT_SPIFREQ               (1000000)

/**@brief Result of sensor's value.
 *
 */
typedef struct Result
{
    float temperature;  
    
    float pressure;
    
    float humidity;
    
    float gas;
	
}sensor_result_t;


/**
 *
 */
class Seeed_BME680
{
  public:
    Seeed_BME680(uint8_t iic_addr);
    Seeed_BME680(int8_t cs=10,int8_t mosi=11,int8_t miso=12,int8_t sck=13);
    bool  init();
	
    int8_t read_sensor_data(void);
	float read_temperature(void);
    float read_pressure(void);
    float read_humidity(void);
    float read_gas(void);

	sensor_result_t sensor_result_value;
	
  private:       
  	
    struct bme680_dev sensor_param;      /**< Official LIB structure.*/
};

#endif
