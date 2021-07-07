#include <time.h>
int dayOfLastUpdate = -1;
void syncTime(bool forceUpdate)
{
  if (WiFi.status() != WL_CONNECTED)
  {
    return;
  }

  configTime(4*3600, 2 * 3600, "0.ua.pool.ntp.org", "pool.ntp.org");
  int i = 0;
  Serial.println("\nWaiting for time");
  while (time(nullptr) < 1500000000 && i < 10)
  {
    Serial.print(".");
    i++;
    delay(1000);
  }

  if (time(nullptr) < 1500000000)
  {
    return;
  }

  time_t t = time(nullptr);

  
  struct tm *tmp = gmtime(&t);

  if ((dayOfLastUpdate != tmp->tm_mday && tmp->tm_hour >= 6) || forceUpdate)
  {
    dayOfLastUpdate = tmp->tm_mday;
    clk.fillByYMD(tmp->tm_year + 1900, tmp->tm_mon, tmp->tm_mday);
    clk.fillByHMS(tmp->tm_hour, tmp->tm_min, tmp->tm_sec);
  //clock.fillDayOfWeek(SAT);
    clk.setTime();//write time to the RTC chip
    Serial.println("\nTime sync was successful");
  }
}

time_t getTimeT()
{
  if(clk.year < 19)
  {
    syncTime(true);
  }
  
  struct tm tmp2;
  tmp2.tm_sec = clk.second;
  tmp2.tm_min = clk.minute;
  tmp2.tm_hour = clk.hour;
  tmp2.tm_mday = clk.dayOfMonth;
  tmp2.tm_mon = clk.month;
  tmp2.tm_year = clk.year + 100;

  return mktime(&tmp2);
}

void rtcGetTime()
{
  do
  {
    clk.getTime();
  } while (clk.year < 19);
}

char* getTimeStr()
{
  time_t t = getTimeT();
  
  return ctime(&t);
}
