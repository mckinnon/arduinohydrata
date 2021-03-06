/*
 * WIRING ---------------------+
 *  Arduino    Pump IC
 *    GND        GND
 *    2          VCC
 *    10         LED
 * 
 *  Arduino    Tiny RTC
 *    A5         SCL
 *    A4         SDA
 *    VCC        VCC
 *    GND        GND
 *    
 *  Arduino    Moisture Sensor
 *    VCC        VCC
 *    GND        GND
 *    A0         A0
 *    13         D0
 *    
 *  Arduino    Bluetooth
 *    VCC        VCC
 *    GND        GND
 *    TX0        RX
 *    RX1        TX
 *    
 * +-------------------------------+
 */

#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include "HC05.h" // https://github.com/jdunmire/HC05

RTC_DS1307 rtc;

int pumpPin = 2;       // pin that turns on the motor
int blinkPin = 10;     // pin that turns on the LED

/* Configuration */
char mode = 'P';       // "T" for 'Time' using clock, "P" for 'Period' using counter
int watertime = 10;    // how long to water in seconds
int waittime = 1;      // how long to wait between waterings, in minutes
int alarmHour_1 = 1;   // hour to start watering
int alarmMin_1 = 25;   // minute to start watering

int counterwait = waittime * 60;
int counter = 0; 
bool watering = false;

/* btSerial echo.ino from HC05 
#ifdef HC05_SOFTWARE_SERIAL
#include <SoftwareSerial.h>
HC05 btSerial = HC05(A2, A5, A3, A4);  // cmd, state, rx, tx
#else
HC05 btSerial = HC05(3, 2);  // cmd, state
#endif
*/

void setup()
{
  rtc.begin();
  Wire.begin();
  Serial.begin(9600);
  
  pinMode(pumpPin, OUTPUT); // set A0 to an output so we can use it to turn on the transistor
  pinMode(blinkPin, OUTPUT); // set pin 13 to an output so we can use it to turn on the LED
  
  if (! rtc.begin()) {
    Serial.println("ERROR: Couldn't find RTC");
    //while (1);
  } else if (rtc.begin()) {
    Serial.println("SUCCESS: Found RTC");
  }

  if (! rtc.isrunning()) {
    Serial.println("ERROR: RTC is NOT running. Check wiring.");
    //while (1);
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  } else if ( rtc.isrunning() ) {
    Serial.println("SUCCESS: RTC is running");
    //following line sets the rtc to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  DateTime now = rtc.now();


  Serial.print("MODE: ");
  if (mode == 'P') {
    Serial.println("Periodic");
  } else if (mode == 'T') {
    Serial.print("Timed (");
    Serial.print(alarmHour_1);
    Serial.print(":");
    Serial.print(alarmMin_1);
    Serial.println(")");
  } else {
    Serial.println("Undefined");
  }
  Serial.print("BOOT: ");

  // DateTime variables: unixtime, year, month, day, hour, minute, second
  Serial.print(now.year());
  Serial.print("/");
  Serial.print(now.month());
  Serial.print("/");
  Serial.print(now.day());
  Serial.print(" ");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  Serial.println(now.second());
  Serial.println("----------------------------");

  /* btSerial echo.ino from HC05
  DEBUG_BEGIN(57600);
  btSerial.findBaud();
  */
}

void loop()
{
  DateTime now = rtc.now(); 
  /* btSerial echo.ino from HC05
  btSerial.println("Echo Server- type something");
    while (btSerial.connected())
  {
    if (btSerial.available())
    {
      btSerial.write(btSerial.read());
    }
  }
  */
  
  // RTC DateTime variables: unixtime, year, month, day, hour, minute, second
  if (mode == 'T' ) {
    if (now.second() == 0 || now.second() == 15 || now.second() == 30 || now.second() == 45) {
      Serial.print(now.hour(), DEC);
      Serial.print(":");
      Serial.print(now.minute(), DEC);
      Serial.print(":");
      Serial.print(now.second(), DEC);
      Serial.print(": ");
        
      if (now.hour() == alarmHour_1 && now.minute() == alarmMin_1 && now.second() == 0) {
        watering = true;
        Serial.print("watering. ");  // Serial report
        digitalWrite(pumpPin, HIGH); // turn on the motor
        Serial.print("Pump ON");  // Serial report
        ++counter;
        delay(1000);
      } else if (watering == false){
        Serial.println("waiting for time");
        delay(1000);
      } else {
        Serial.println("ERROR 1: neither True nor False");
        while (1);
      }
    }
    if (watering == true) {
      digitalWrite(blinkPin, HIGH); // turn on the LED
      if (counter < watertime) {
        Serial.print(" . ");
        ++counter;
        delay(1000);
      } else if (counter == watertime) {
        digitalWrite(pumpPin, LOW);  // turn off the motor
        Serial.println("Pump OFF");  // Serial report
        digitalWrite(blinkPin, LOW);  // turn off the LED
        watering = false;
        counter = 0;
        delay(1000);
      } else {
        Serial.println("ERROR 2: impossible counter value");
        while (1);
      }
    }
  }
    
  if (mode == 'P' ) {
    if (counter == 0) {
      Serial.print(now.hour(), DEC);
      Serial.print(":");
      Serial.print(now.minute(), DEC);
      Serial.print(":");
      Serial.print(now.second(), DEC);
      Serial.print(": ");
      digitalWrite(pumpPin, HIGH); // turn on the motor
      Serial.print("Pump ON ");  // Serial report
      digitalWrite(blinkPin, HIGH); // turn on the LED
      ++counter;
      delay(1000);
    } else if (counter < watertime) {
      Serial.print(" . ");
      ++counter;
      delay(1000);
    } else if (counter == watertime) {
      digitalWrite(pumpPin, LOW);  // turn off the motor
      Serial.println("Pump OFF");  // Serial report
      digitalWrite(blinkPin, LOW);  // turn off the LED
      ++counter;
      delay(1000);
      Serial.print(now.hour(), DEC);
      Serial.print(":");
      Serial.print(now.minute(), DEC);
      Serial.print(":");
      Serial.print(now.second(), DEC);
      Serial.print(": Waiting");
    } else if (counter > watertime && counter < counterwait) {
      //Serial.print(" . ");
      ++counter;
      delay(1000);
    } else if (counter == counterwait) {
      Serial.println(" OK");
      counter = 0;
      delay(1000);
    } else {
      Serial.println("ERROR 3: impossible counter value");
      while (1);
      //++counter;
      //delay(1000);
    }
  }
}


