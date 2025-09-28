#include "time.h"
#include "constants.h"
int currentDay = 0;

void timeSetup(){
  configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER);
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    errorCode = 4;
  }
  currentDay = timeinfo.tm_mday;
  Serial.print("Current Day: ");
  Serial.println(currentDay);
}

Config getConfig(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    errorCode = 4;
    return configs[0];
  }
  if(timeinfo.tm_mday != currentDay){
    // Is the next day, just use last defined config
    return configs[NUM_CONFIGS - 1];
  }
  int configIndex = 1;
  while(configIndex < NUM_CONFIGS){
    if(configs[configIndex].hourStart > timeinfo.tm_hour){
      break;
    }
    configIndex++;
  }
  return configs[configIndex - 1];
}

bool hasReset(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    errorCode = 4;
    return false;
  }
  bool isPastReset = (timeinfo.tm_hour == RESET_HOUR && timeinfo.tm_min >= RESET_MIN) || timeinfo.tm_hour > RESET_HOUR;
  if(timeinfo.tm_mday != currentDay && isPastReset){
    currentDay = timeinfo.tm_mday;
    return true;
  }
  return false;
}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.print("Day of week: ");
  Serial.println(&timeinfo, "%A");
  Serial.print("Month: ");
  Serial.println(&timeinfo, "%B");
  Serial.print("Day of Month: ");
  Serial.println(&timeinfo, "%d");
  Serial.print("Year: ");
  Serial.println(&timeinfo, "%Y");
  Serial.print("Hour: ");
  Serial.println(&timeinfo, "%H");
  Serial.print("Hour (12 hour format): ");
  Serial.println(&timeinfo, "%I");
  Serial.print("Minute: ");
  Serial.println(&timeinfo, "%M");
  Serial.print("Second: ");
  Serial.println(&timeinfo, "%S");

  Serial.println("Time variables");
  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  Serial.println(timeHour);
  char timeWeekDay[10];
  strftime(timeWeekDay,10, "%A", &timeinfo);
  Serial.println(timeWeekDay);
  Serial.println();
}