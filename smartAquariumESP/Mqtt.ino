char* mqtt_server = "192.168.0.101";
char* mqtt_user = "smarthome";
char* mqtt_password = "Summercomeback!23";

const char* filterTempOffTopic = "/room/aquarium/filtertempoff";
const char* filterTempOffTopicResp = "/room/aquarium/filtertempoffstatus";

const char* funOnTopic = "/room/aquarium/funon";
const char* funOnTopicResp = "/room/aquarium/funonstatus";

const char* forceUpdateTimeTopic = "/room/aquarium/forceupdatetime";

const char* getTimeReqTopic = "/room/aquarium/gettimereq";
const char* getTimeRespTopic = "/room/aquarium/gettimeresp";

const char* autoModeTopic = "/room/aquarium/setAutoMode";
const char* autoModeStatusTopic = "/room/aquarium/setAutoModeStatus";

const char* feedTopic = "/room/aquarium/feed";

void mqtt_setup() 
{
  if (!mqtt_client.connected()) {
    mqtt_client.setServer(mqtt_server, 14037);
    mqtt_client.setCallback(mqtt_callback);
    mqtt_client.setClient(wifiClient);

    mqtt_client.connect(clientName, mqtt_user, mqtt_password);
    mqtt_client.subscribe(filterTempOffTopic);

    mqtt_client.subscribe(funOnTopic);
    
    mqtt_client.subscribe(getTimeReqTopic);
    mqtt_client.subscribe(forceUpdateTimeTopic);
    mqtt_client.subscribe(autoModeTopic);
    mqtt_client.subscribe(feedTopic);
  }
}

void mqtt_callback(char* topic, byte* payload, unsigned int len) 
{
  if(strcmp(topic, filterTempOffTopic) == 0)
  {
    filterTempOff((char)payload[0]);
    mqtt_client.publish(getTimeReqTopic, "0", false);
    return;
  }

  if(strcmp(topic, funOnTopic) == 0)
  {
    funOn((char)payload[0]);
    return;
  }
  
  if(strcmp(topic, getTimeReqTopic) == 0)
  {
    String s1 = " ctime: " + String(getTimeT());
    String s2 = " stime: " + String(filterTimeStart);
    String s3 = getTimeStr();
    String s = s3.substring(7) + s1 + s2;
    mqtt_client.publish(getTimeRespTopic, s.c_str());
    return;
  }
  
  if(strcmp(topic, forceUpdateTimeTopic) == 0)
  {
    syncTime(true);
    return;
  }
  
  if(strcmp(topic, autoModeTopic) == 0)
  {
    setAutoMode((char)payload[0]);
    return;
  }

  if(strcmp(topic, feedTopic) == 0)
  {
    feed();
    return;
  }
}
