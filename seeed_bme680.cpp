/**
 * @file seeed_bme680.cpp
 * Library for BME680
 *
 * Copyright (c) 2013 Seeed Technology Co., Ltd.
 * @author        :   downey
 * @date Create Time   :   2017/12/08
 * Change Log    :
 */

/**
 * @brief The main driver file of BME680 sensor.
 *
 * BME680 support for temperature,humidity,indoor-air_quality(gas) and pressure value measurement,
 * The result of measurement is stored in class  Seeed_BME680->sensor_result_value.
 * BME680 support for two communication protocol-SPI and IIC,The different communication  protocol
 * corresponding different constructor.Furthermore,you can customize the pin when your development board's SPI
 * interface is mismatch with official.All you have to do is choose different ways to instantiate object.
 */
#include "seeed_bme680.h"


static int8_t spi_pinmap_cs;
static int8_t spi_pinmap_mosi;
static int8_t spi_pinmap_miso;
static int8_t spi_pinmap_sck;
static int8_t user_define_spi_pinmap_flag;

/**@brief constructor of IIC interface.
 * @param addr The BME680 device IIC address.
 * @return NONE.
 */
Seeed_BME680::Seeed_BME680(uint8_t addr)
{
	sensor_param.dev_id=addr;
	sensor_param.intf=BME680_I2C_INTF;
}


/**@brief constructor of SPI interface.
 * @param cs    The cs pin of SPI interface,Default value is 10;
 * @param mosi  The mosi pin of SPI interface,Default value is 11;
 * @param miso  The miso pin of SPI interface,Default value is 12;
 * @param sck  The sck pin of SPI interface,Default value is 13;
 * @note  It will call the different SPI transmission  API when user customizes SPI pin
 * @return NONE.
 */
Seeed_BME680::Seeed_BME680(int8_t cs,int8_t mosi,int8_t miso,int8_t sck) 
{
	sensor_param.intf=BME680_SPI_INTF;
	spi_pinmap_cs=cs;
	spi_pinmap_mosi = mosi;
	spi_pinmap_miso = miso;
	spi_pinmap_sck = sck;
  	if((10==cs)&&(11==mosi)&&(miso==12)&&(sck==13))
  	{
  		user_define_spi_pinmap_flag=0;
		Serial.println("normal spi pin map!!");
  	}
	else
	{
		user_define_spi_pinmap_flag=1;
		Serial.println("user define spi pin map!!");
	}
}


///@brief This function implements the temperature value of the read sensor
/// @param  NONE.
/// @return sensor_result_value.temperature  The result of temperature value.
float Seeed_BME680:: read_temperature(void)
{
	if(read_sensor_data())
	{
		return 0;
	}
	return sensor_result_value.temperature;
}

///@brief This function implements the pressure value of the read sensor
/// @param  NONE.
/// @return sensor_result_value.pressure  The result of pressure value.
///
float Seeed_BME680:: read_pressure(void)
{
	if(read_sensor_data())
	{
		return 0;
	}
	return sensor_result_value.pressure;
}

/**@brief This function implements the humidity value of the read sensor
 * @param  NONE.
 * @return sensor_result_value.humidity  The result of humidity value.
 */
float Seeed_BME680:: read_humidity(void)
{
	if(read_sensor_data())
	{
		return 0;
	}
	return sensor_result_value.humidity;
}

/**@brief This function implements the gas value of the read sensor
 * @param  NONE.
 * @return sensor_result_value.gas  The result of gas value.
 */
float Seeed_BME680:: read_gas(void)
{
	if(read_sensor_data())
	{
		return 0;
	}
	return sensor_result_value.gas;
}

/**@brief Getting four kinds of result value from the sensor.
 * @param NONE.
 * @return Result of function excution. The normal exit is only when it returns BME680_OK(0).
 */
int8_t Seeed_BME680::read_sensor_data(void) 
{
	
	struct bme680_field_data data;
	
	int8_t ret;

	sensor_param.power_mode = BME680_FORCED_MODE;


	uint16_t settings_sel;

	sensor_param.tph_sett.os_hum = BME680_OS_1X;
	sensor_param.tph_sett.os_pres = BME680_OS_16X;
	sensor_param.tph_sett.os_temp = BME680_OS_2X;

	sensor_param.gas_sett.run_gas = BME680_ENABLE_GAS_MEAS;
	// sensor_param.gas_sett.heatr_dur = 150;
	// sensor_param.gas_sett.heatr_temp = 320;
	sensor_param.gas_sett.heatr_dur = 100;
	 sensor_param.gas_sett.heatr_temp = 300;

	settings_sel=BME680_OST_SEL | BME680_OSH_SEL | BME680_OSP_SEL | BME680_FILTER_SEL | BME680_GAS_SENSOR_SEL;

	/*Set sensor's registers*/
	if(ret = bme680_set_sensor_settings(settings_sel, &sensor_param))
	{
		Serial.print("bme680_set_sensor_settings() ==>ret value = ");
		Serial.println(ret);
		return -1;
	}
	/*Set sensor's mode ,activate sensor*/
	if(ret = bme680_set_sensor_mode(&sensor_param))
	{
		Serial.print("bme680_set_sensor_mode() ==>ret value = ");
		Serial.println(ret);
		return -2;
	}
	
	uint16_t meas_period;
	bme680_get_profile_dur(&meas_period, &sensor_param);
	
	delay(meas_period);   /**<It is necessary to delay for a duration time */
	
	/*Get sensor's result value from registers*/
	if(ret = bme680_get_sensor_data(&data, &sensor_param))
	{
		Serial.print("bme680_get_sensor_data() ==>ret value = ");
		Serial.println(ret);
		return -3;
	}

	sensor_result_value.temperature = data.temperature / 100.0;
	sensor_result_value.humidity= data.humidity / 1000.0;
	sensor_result_value.pressure= data.pressure;
	if (data.status & BME680_HEAT_STAB_MSK) 
	{
		sensor_result_value.gas = data.gas_resistance;
	} 
	else 
	{
		sensor_result_value.gas = 0;
	}
 	return BME680_OK;
}





/**@brief  IIC reading interface
 * @param  dev_id   IIC device address
 * @param  reg_addr The register address for operated.
 * @param  reg_data Storing data read from  registers.
 * @param  len      Length of data.
 * @return The normal exit is only when it returns 0.
 */
int8_t iic_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len) 
{
  int32_t i=0;
  Wire.beginTransmission((uint8_t)dev_id);
  Wire.write((uint8_t)reg_addr);
  Wire.endTransmission();
  if (len != Wire.requestFrom((uint8_t)dev_id, (byte)len)) 
  {
    return 1;
  }
  for(i=0;i<len;i++)
  {
  	reg_data[i]=(uint8_t)Wire.read();
  }
  return 0;
}

/**@brief  IIC wrting interface
 * @param  dev_id   IIC device address
 * @param  reg_addr The register address for operated.
 * @param  reg_data The data need to be transmitted.
 * @param  len      Length of data.
 * @return The normal exit is only when it returns 0.
 */
int8_t iic_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len) 
{
	int32_t i=0;
	Wire.beginTransmission((uint8_t)dev_id);
	Wire.write((uint8_t)reg_addr);

	for(i=0;i<len;i++)
	{
		 Wire.write(reg_data[i]);
	}
	Wire.endTransmission();
	return 0;
}


/**@brief  SPI read and write one byte.
 * @param  x   The byte needs to be transmitted.
 * @return The byte received from sensor.
 */
static uint8_t spi_transfer(uint8_t x) 
{
  if (0==user_define_spi_pinmap_flag)
  	{
  		return SPI.transfer(x);
  	}
//  Serial.println(spi_pinmap_cs);
  uint8_t reply = 0;
  for (int i=7; i>=0; i--) 
  {
    reply <<= 1;
    digitalWrite(spi_pinmap_sck, LOW);
    digitalWrite(spi_pinmap_mosi, x & (1<<i));
    digitalWrite(spi_pinmap_sck, HIGH);
    if (digitalRead(spi_pinmap_miso))
    {
    	reply |= 1;
    }
  }
  return reply;
}


/**@brief  SPI reading interface
 * @param  cspin   cspin of SPI hardware interface.
 * @param  reg_addr The register address for operated.
 * @param  reg_data Storing data read from  registers.
 * @param  len      Length of data.
 * @return The normal exit is only when it returns 0.
 */
static int8_t spi_read(uint8_t cspin, uint8_t reg_addr, uint8_t *reg_data, uint16_t len) 
{
  uint32_t i=0;
  digitalWrite(cspin, LOW);

  if (0==user_define_spi_pinmap_flag) 
  {
    SPI.beginTransaction(SPISettings(BME680_DEFAULT_SPIFREQ, MSBFIRST, SPI_MODE0));
  }

  spi_transfer(reg_addr);

  for(i=0;i<len;i++)
  {
  	reg_data[i]=spi_transfer(0x00);
  }
  if (0==user_define_spi_pinmap_flag) 
  {
    SPI.endTransaction();
  }

  digitalWrite(cspin, HIGH);

  return 0;
}

/**@brief  SPI wrting interface
 * @param  cspin   cspin of SPI hardware interface.
 * @param  reg_addr The register address for operated.
 * @param  reg_data The data need to be transmitted.
 * @param  len      Length of data.
 * @return The normal exit is only when it returns 0.
 */
static int8_t spi_write(uint8_t cspin, uint8_t reg_addr, uint8_t *reg_data, uint16_t len) 
{

  uint32_t i=0;
  digitalWrite(cspin, LOW);
  if (0==user_define_spi_pinmap_flag) 
  {
    SPI.beginTransaction(SPISettings(BME680_DEFAULT_SPIFREQ, MSBFIRST, SPI_MODE0));
  }

  spi_transfer(reg_addr);

  for(i=0;i<len;i++)
  {
  	spi_transfer(reg_data[i]);
  }

  if (0==user_define_spi_pinmap_flag) 
  {
    SPI.endTransaction();
  }
  digitalWrite(cspin, HIGH);
  return 0;
}



/**@brief delay milliseconds
 * @param ms   milliseconds
 * @return NONE
 */
static void delay_msec(uint32_t ms)
{
  delay(ms);
}

/**@brief Initialization of sensor
 * @param NONE
 * @return ture or false
 */
bool Seeed_BME680::init() 
{
	/*User specifies IIC protocol in constructor.*/

    if(sensor_param.intf==BME680_I2C_INTF)
	{
	    Wire.begin();
	    sensor_param.read = iic_read;
	    sensor_param.write = iic_write;
  	} 
    /*User specifies SPI protocol in constructor.*/
  	else if(sensor_param.intf==BME680_SPI_INTF)
  	{
	 
   	 digitalWrite(spi_pinmap_cs, HIGH);
    	pinMode(spi_pinmap_cs, OUTPUT);
	

    if (0==user_define_spi_pinmap_flag)
	{
      SPI.begin();
    } 
	else 
    {
      pinMode(spi_pinmap_sck, OUTPUT);
      pinMode(spi_pinmap_mosi, OUTPUT);
      pinMode(spi_pinmap_miso, INPUT);
    }

    sensor_param.dev_id = spi_pinmap_cs;
    sensor_param.intf = BME680_SPI_INTF;
    sensor_param.read = &spi_read;
    sensor_param.write = &spi_write;
	}

  sensor_param.delay_ms = delay_msec;

  int8_t ret = BME680_OK;
  /*Check the wiring,Check whether the protocol stack is normal and Read the calibrated value from sensor */
  if(ret = bme680_init(&sensor_param))
  {
	// Serial.print("bme680_init() ==>ret value = ");
	// Serial.println(ret);
	return false;
  }
/*Print the calibrated value of sensor.*/
#if 0
  Serial.print("T1 = "); Serial.println(sensor_param.calib.par_t1);
  Serial.print("T2 = "); Serial.println(sensor_param.calib.par_t2);
  Serial.print("T3 = "); Serial.println(sensor_param.calib.par_t3);
  Serial.print("P1 = "); Serial.println(sensor_param.calib.par_p1);
  Serial.print("P2 = "); Serial.println(sensor_param.calib.par_p2);
  Serial.print("P3 = "); Serial.println(sensor_param.calib.par_p3);
  Serial.print("P4 = "); Serial.println(sensor_param.calib.par_p4);
  Serial.print("P5 = "); Serial.println(sensor_param.calib.par_p5);
  Serial.print("P6 = "); Serial.println(sensor_param.calib.par_p6);
  Serial.print("P7 = "); Serial.println(sensor_param.calib.par_p7);
  Serial.print("P8 = "); Serial.println(sensor_param.calib.par_p8);
  Serial.print("P9 = "); Serial.println(sensor_param.calib.par_p9);
  Serial.print("P10 = "); Serial.println(sensor_param.calib.par_p10);
  Serial.print("H1 = "); Serial.println(sensor_param.calib.par_h1);
  Serial.print("H2 = "); Serial.println(sensor_param.calib.par_h2);
  Serial.print("H3 = "); Serial.println(sensor_param.calib.par_h3);
  Serial.print("H4 = "); Serial.println(sensor_param.calib.par_h4);
  Serial.print("H5 = "); Serial.println(sensor_param.calib.par_h5);
  Serial.print("H6 = "); Serial.println(sensor_param.calib.par_h6);
  Serial.print("H7 = "); Serial.println(sensor_param.calib.par_h7);
  Serial.print("G1 = "); Serial.println(sensor_param.calib.par_gh1);
  Serial.print("G2 = "); Serial.println(sensor_param.calib.par_gh2);
  Serial.print("G3 = "); Serial.println(sensor_param.calib.par_gh3);
  Serial.print("G1 = "); Serial.println(sensor_param.calib.par_gh1);
  Serial.print("G2 = "); Serial.println(sensor_param.calib.par_gh2);
  Serial.print("G3 = "); Serial.println(sensor_param.calib.par_gh3);
  Serial.print("Heat Range = "); Serial.println(sensor_param.calib.res_heat_range);
  Serial.print("Heat Val = "); Serial.println(sensor_param.calib.res_heat_val);
  Serial.print("SW Error = "); Serial.println(sensor_param.calib.range_sw_err);
#endif

  return true;
}



