BME680:  [![Build Status](https://travis-ci.com/Seeed-Studio/Seeed_BME680.svg?branch=master)](https://travis-ci.com/Seeed-Studio/Seeed_BME680)
===========
![BME680](https://github.com/linux-downey/picture_repository/blob/master/BME680.png)  

BME680 is an integrated environmental sensor developed specifically for mobile applications and wearables where size and low power consumption are key requirements.It also supports four kinds of numerical measurements of temperature, humidity, gas and air pressure.  

Usage:
==========
***Bme680 supports IIC and SPI communication protocols.***
If the user uses IIC protocol,The main functions are as follows，And the results of mesurements stored in struct of ***bme680.sensor_result_value***.

	Seeed_BME680 bme680(IIC_ADDR); //IIC_ADDR is address of IIC device
	bme680.init();
	bme680.read_sensor_data();

If user uses SPI protocol,The main functions are as follows，And the results of mesurements stored in struct of ***bme680.sensor_result_value***.

	Seeed_BME680 bme680();//if user uses default pins of UNO development board.(cs-10,mosi-11,miso-12,sck-13)
	//  Seeed_BME680 bme680(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);//if user customizes pin of SPI hardware interface.
	bme680.init();
	bme680.read_sensor_data();

***user also can downloads the source code and running it on arduino IDE,More details in /examples***

Software reference:
----------
**1.The official drive of Bosch**  
**2.The BME680 drive of Adafruit**

***

NOTICE!!!
======  
This release version doesn't support gas measurement well,The result value of gas measurement is biased.If you want get a accurate measurement of gas,please refer to [Seeed_BME680_V1](https://github.com/Seeed-Studio/Seeed_BME680_V1),***but this release version only supports several large memory arduino platform like ATMEGA2560,Manufacturers are developing...***  


***
statement:
==========
This software is written by downey for seeed studio and is licensed under The MIT License. Check License.txt for more information.

Contributing to this software is warmly welcomed. You can do this basically by
forking, committing modifications and then pulling requests (follow the links above
for operating guide). Adding change log and your contact into file header is encouraged.
Thanks for your contribution.

Seeed is a hardware innovation platform for makers to grow inspirations into differentiating products. By working closely with technology providers of all scale, Seeed provides accessible technologies with quality, speed and supply chain knowledge. When prototypes are ready to iterate, Seeed helps productize 1 to 1,000 pcs using in-house engineering, supply chain management and agile manufacture forces. Seeed also team up with incubators, Chinese tech ecosystem, investors and distribution channels to portal Maker startups beyond.

