#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <EEPROM.h>
#include "OneButton.h"

#include <Wire.h>
#include "DS1307.h"
#include "GyverTimer.h"

DS1307 clk;//define a object of DS1307 class

WiFiClient wifiClient;
PubSubClient mqtt_client;

const char* wifi_ssid = "Znet_IOT";
const char* wifi_password = "Summercomeback!23";

const int lightPin = 14;
const int filterPin = 12;
const int feederPin = 13;
const int funPin = 2;

const int buttonPin = 16;
OneButton oneButton(buttonPin, true, true);

GTimer_ms myTimer;

const char* clientName = "esp8266-aquarium";
bool isFilterEnabled = true;
int filterTimeStart = -1;

bool isFunEnabled = false;
int funTimeStart = -1;

bool modeAutob = false;

byte dayOfLastFeed = -1;

bool needInit = true;




void setup()
{
  Serial.begin(115200);
  
  pinMode(feederPin, OUTPUT);
  pinMode(lightPin, OUTPUT);
  pinMode(filterPin, OUTPUT);
  pinMode(funPin, OUTPUT);

  digitalWrite(feederPin, HIGH);
  digitalWrite(lightPin, HIGH);
  digitalWrite(filterPin, HIGH);
  digitalWrite(funPin, LOW);
  
  clk.begin();

  EEPROM.begin(1);
  dayOfLastFeed = EEPROM.read(0);

  WiFi.begin(wifi_ssid, wifi_password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  ArduinoOTA.setHostname(clientName);
  ArduinoOTA.begin();

  myTimer.setInterval(1000);

  oneButton.attachClick(filterTempOffShort);
  oneButton.attachLongPressStart(filterTempOffLong);
}

void loop()
{
  oneButton.tick();




  if (clk.hour == 5 && !isFunEnabled)
  {
    funOn('1');
  }


  

  if (myTimer.isReady())
  {
  mqtt_setup();

  rtcGetTime();

  if (needInit)
  {
    needInit = false;
    char am;
    EEPROM.get(1, am);
    setAutoMode(am);
  }

  if (modeAutob)
  {
    modeAuto();
  }
  else
  {
    digitalWrite(lightPin, LOW);
  }

  if (isFilterEnabled)
  {
    digitalWrite(filterPin, HIGH);
  }
  else
  {
    digitalWrite(filterPin, LOW);
  }

  if (isFunEnabled)
  {
    digitalWrite(funPin, LOW);
  }
  else
  {
    digitalWrite(funPin, HIGH);
  }


  if (getTimeT() >= filterTimeStart && filterTimeStart != -1)
  {
    filterTempOff('0');
  }

  
  if (getTimeT() >= funTimeStart && funTimeStart != -1)
  {
    funOn('0');
  }

  syncTime(false);

  ArduinoOTA.handle();

  mqtt_client.loop();
  }
}
