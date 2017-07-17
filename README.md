# arduinohydrata
Yet another Arduino-based plant watering system. This project aims to develop a kit for a flexible and modular watering system which requires minimal configuation or customization.

## Getting Started
*Minimum Installation* Arduino Uno and a 3-5 volt water pump. Pinouts and config settings are included in arduinohydrata.ino. Upload the Sketch in the usual way.

*RTC Installation* Arduino Uno, a 3-5 volt water pump, and a DS1307 RTC. Pinouts and config settings are included in arduinohydrata.ino. Upload the Sketch in the usual way. The code has two commented-out lines which can be restored to stop the program if there is an error communicating with the RTC.
```Serial.println("ERROR: Couldn't find RTC");
//while (1);
```

```Serial.println("ERROR: RTC is NOT running. Check wiring.");
//while (1);
```

*Moisture Sensor Installation* FC-28 module not yet implemented.

*Bluetooth Installation* HC-05 module not yet implemented.

*Full Build* See the Parts subdirectory for parts and schematics.

## Parts
*BTE13-010 - Arduino Pro Mini clone
*DS1307 - Tiny RTC I2C module
*HC-05 - Bluetooth module master/slave
*FC-28 - soil moisture sensor module
*Misc: LED & resistor, pin headers

## WIRING
Note: Wiring instructions may vary slightly between manufacturers. Check your pinouts.
|BTE13-010|Pump IC|
| --- | ---:|
|GND|GND|
|2|VCC|
|10|LED|
 
|BTE13-010|Tiny RTC|
| --- | ---:|
|A5         SCL|
|A4         SDA|
|VCC        VCC|
|GND        GND|

|BTE13-010|Moisture Sensor|
| --- | ---:|
|VCC|VCC|
|GND|GND|
|A0|A0|
|13|D0|

|BTE13-010|Bluetooth|
| --- | ---:|
|VCC|VCC|
|GND|GND|
|TX0|RX|
|RX1|TX|

## Tips & Warnings
*BTE13-010 won't upload sketch*
Many inexpensive Pro Mini clones ship without bootloaders. Use Nick Gammon's (Atmega bootloader programmer) [http://www.gammon.com.au/bootloader] to install an "Arduino/Genuino Uno" bootloader.

*Bluetooth module won't echo back serial communications*
HC-06 is a recieve-only 'slave' module; HC-05 has two modes. Obtain an HC-05 and set it to 'master' mode for bilateral communication.

## Authors

* **Jay McKinnon** - *Initial work* - [Github](https://github.com/mckinnon/) [Twitter](https://twitter.com/opendna)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments
* All praise to Adafruit and Nick Gammon!