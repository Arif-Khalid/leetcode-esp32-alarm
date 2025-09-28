#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "credentials.h"
#include "constants.h"
/*  Error code meanings:
    0: No error
    1: Too many requests (rate limit of server hit)
    2: HTTP query failed (likely timeout, server didn't respond, possibly offline)
    3: WiFi not connected (WiFi network down/disconnected)
    4: Time not retrieved (NTP error)
*/
int errorCode = 0;
bool isPendingRetry = false;
bool hasSucceeded = false;
Config config = configs[0];

const size_t JSON_CAP = JSON_OBJECT_SIZE(3) + 1024;
String getLatestAcceptedName(){
  String title = "";
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.setTimeout(NETWORK_REQUEST_TIMEOUT_MS);

    http.begin(BASE_URL + LAST_AC_ROUTE);
    int httpCode = http.GET();

    if (httpCode > 0) {
      Serial.printf("HTTP Response code: %d\n", httpCode);
      if(httpCode == 429){
        errorCode = 1;
        return "";
      }
      String payload = http.getString();


      DynamicJsonDocument doc(JSON_CAP);
      DeserializationError error = deserializeJson(doc, payload);

      if (error) {
        Serial.print("JSON deserialization failed: ");
        Serial.println(error.f_str());
        return "";
      }

      title = doc["submission"][0]["titleSlug"].as<String>();
      Serial.print("Last AC Title: ");
      Serial.println(title);
    } else {
      errorCode = 2;
      Serial.printf("GET failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  } else {
    errorCode = 3;
    Serial.println("WiFi not connected");
  }
  return title;
}

String getDailyName(){
  String title = "";
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.setTimeout(NETWORK_REQUEST_TIMEOUT_MS);

    http.begin(BASE_URL + DAILY_ROUTE);
    int httpCode = http.GET();

    if (httpCode > 0) {
      Serial.printf("HTTP Response code: %d\n", httpCode);
      if(httpCode == 429){
        errorCode = 1;
        return "";
      }
      String payload = http.getString();


      DynamicJsonDocument doc(JSON_CAP);
      DeserializationError error = deserializeJson(doc, payload);

      if (error) {
        Serial.print("JSON deserialization failed: ");
        Serial.println(error.f_str());
        return "";
      }

      title = doc["titleSlug"].as<String>();
      Serial.print("Title: ");
      Serial.println(title);
    } else {
      errorCode = 2;
      Serial.printf("GET failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  } else {
    errorCode = 3;
    Serial.println("WiFi not connected");
  }
  return title;
}

bool hasCompletedDaily(){
  errorCode = 0;
  String dailyName = getDailyName();
  if(dailyName.length() == 0){
    return false;
  }
  String latestAcceptedName = getLatestAcceptedName();
  if(latestAcceptedName.length() == 0){
    return false;
  }
  return dailyName == latestAcceptedName;
}

void setup() {
  Serial.begin(9600);
  delay(1000);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  feedbackSetup();
  timeSetup();
  Serial.println("\nWiFi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}



unsigned long retryStart = 0;
void loop() {
  if(hasSucceeded){
    if(hasReset()){
      isPendingRetry = false;
      hasSucceeded = false;
      errorCode = 0;
    }else{
      displayErrorCode(errorCode);
      delay(TIME_BETWEEN_TIME_CHECK_MS);
    }
    return;
  }

  config = getConfig();
  if(isPendingRetry){
    // Non blocking wait for TIME_BETWEEN_RETRY MS
    unsigned long currentMillis = millis();
    if(retryStart > currentMillis){
      retryStart = 0;
    }
    int currentPast = currentMillis - retryStart;
    retryLed(currentPast * 1.0 / config.timeBetweenRetryMs);
    if(currentPast > config.timeBetweenRetryMs){
      isPendingRetry = false;
    }
    delay(100);
  }else{
    loadingLed();
    if(hasCompletedDaily()){
      successLed();
      hasSucceeded = true;
      Serial.println("Congratulations! Now I will continue checking after 8.30a.m.");
    }else{
      failureLedSound();
      Serial.println("Now waiting 30 minutes till next loop, reset to try again earlier");
      retryStart = millis();
      isPendingRetry = true;
    }
  }
}