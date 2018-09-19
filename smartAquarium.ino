#include <Wire.h>
#include "DS1307.h"

DS1307 clock;//define a object of DS1307 class

const int lightPin = 6;
const int filterPin = 7;
const int compressorPin = 8;
const int feederPin = 9;

void setup()
{
  pinMode(feederPin, OUTPUT);
  pinMode(lightPin, OUTPUT);
  pinMode(filterPin, OUTPUT);
  pinMode(compressorPin, OUTPUT);

  digitalWrite(feederPin, HIGH);
  digitalWrite(lightPin, HIGH);
  digitalWrite(filterPin, HIGH);
  digitalWrite(compressorPin, HIGH);
  
  clock.begin();
  //clock.fillByYMD(2018,9,8);
  //clock.fillByHMS(18,45 ,00);
  //clock.fillDayOfWeek(SAT);
  //clock.setTime();//write time to the RTC chip
}
void loop()
{
  clock.getTime();

  if (clock.hour >= 7 && clock.hour < 21)
  {
    digitalWrite(lightPin, LOW);
  }
  else
  {
    digitalWrite(lightPin, HIGH);
  }

  if (clock.hour == 7 && clock.minute == 5)
  {
    digitalWrite(feederPin, LOW);
    delay(1000);
    digitalWrite(feederPin, HIGH);
    delay(60000);
  }

  if (clock.hour == 7 && clock.minute >= 30)
  {
    digitalWrite(filterPin, LOW);
  }
  else
  {
    digitalWrite(filterPin, HIGH);
  }

  if (clock.hour >= 7 && clock.hour < 21 && clock.minute >= 10 && clock.minute <= 30)
  {
    digitalWrite(compressorPin, LOW);
  }
  else
  {
    digitalWrite(compressorPin, HIGH);
  }

  delay(1000);
}
