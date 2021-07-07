void filterTempOffShort()
{
  filterTempOff('1', 600);
}

void filterTempOffLong()
{
  filterTempOff('1');
}

void filterTempOff(char tempOff)
{
  filterTempOff(tempOff, 3600);
}

void filterTempOff(char tempOff, int timeOff)
{
  if (tempOff != '0' && tempOff != '1')
  {
    return;
  }

  if (tempOff == '1')
  {
    isFilterEnabled = false;
    filterTimeStart = getTimeT() + timeOff;  
  }
  else
  {
    isFilterEnabled = true;
    filterTimeStart = -1;    
  }

  mqtt_client.publish(filterTempOffTopicResp, String(tempOff).c_str(), true);
}

void funOn(char fon)
{
  if (fon != '0' && fon != '1')
  {
    return;
  }

  if (fon == '1')
  {
    isFunEnabled = true;
    funTimeStart = getTimeT() + 3600;  
  }
  else
  {
    isFunEnabled = false;
    funTimeStart = -1;    
  }

  mqtt_client.publish(funOnTopicResp, String(fon).c_str(), true);
}

void feed()
{
  digitalWrite(feederPin, LOW);
  delay(200);
  digitalWrite(feederPin, HIGH);
}


void modeAuto()
{ 
  if (clk.hour >= 8 && clk.hour < 22)
  {
    digitalWrite(lightPin, LOW);
  }
  else
  {
    digitalWrite(lightPin, HIGH);
  }

  if (dayOfLastFeed != clk.dayOfMonth && clk.hour >= 8)
  {
    dayOfLastFeed = clk.dayOfMonth;
    EEPROM.begin(1);
    EEPROM.write(0, dayOfLastFeed);
    EEPROM.commit();
    filterTempOff('1', 600);
    feed();
  }
}

void setAutoMode(char automod)
{
  if (automod != '0' && automod != '1')
  {
    return;
  }

  mqtt_client.publish(autoModeStatusTopic, String(automod).c_str(), true);

  char c = modeAutob ? '1' : '0';

  if (automod == c)
  {
    return;
  }

  if (automod == '1')
  {
    modeAutob = true;
  }
  else
  {
    modeAutob = false;   
  }

  EEPROM.begin(1);
  EEPROM.put(1, automod);
  EEPROM.commit();
}
