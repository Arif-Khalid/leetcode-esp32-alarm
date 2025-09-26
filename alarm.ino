#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "credentials.h"
#include "constants.h"


unsigned long previousMillis = 0; // stores last toggle time
bool isLedOn = false;
int ledState = 0;
int errorCode = 0;
bool isPendingRetry = false;

const size_t JSON_CAP = JSON_OBJECT_SIZE(3) + 1024;
String getLatestAcceptedName(){
  String title = "";
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.setTimeout(10000);  // 10 seconds

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
    http.setTimeout(10000);  // 10 seconds

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

void completedDailyCheck(){
  ledState = 0;
  String dailyName = getDailyName();
  if(dailyName.length() == 0){
    return;
  }
  String latestAcceptedName = getLatestAcceptedName();
  if(latestAcceptedName.length() == 0){
    return;
  }
  if(dailyName == latestAcceptedName){
    ledState = 1;
  }
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

  Serial.println("\nWiFi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Setup PWM on the speaker pin
  ledcAttachChannel(SPEAKER_PIN, 2000, 8, PWM_CHANNEL);
  testing();
}

void loadingLed(){
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(LED_PIN, LOW);
}

void successLed(){
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(LED_PIN, HIGH);
}

void failureLedSound(){
  digitalWrite(LED_PIN, LOW);
  for (int i = 0; i < 2; i++){
    ledcWrite(SPEAKER_PIN, ERROR_FREQ);
    delay(BEEP_DURATION_MS);
    ledcWrite(SPEAKER_PIN, 0);
    delay(PAUSE_DURATION_MS);
  }
}

void retryLed(){
  digitalWrite(LED_BUILTIN, LOW);
  if(errorCode > 0){
    unsigned long currentMillis = millis();
    if(previousMillis > currentMillis){
      // Rollover occured
      previousMillis = 0;
    }

    // check if interval has passed
    if (currentMillis - previousMillis >= INTERVAL) {
      previousMillis = currentMillis;   // save last toggle time
      isLedOn = !isLedOn;               // flip state
      digitalWrite(LED_BUILTIN, isLedOn);    // apply to LED
    }
  }
}

unsigned long retryStart = 0;
void loop() {
  if(isPendingRetry){
    retryLed();
    unsigned long currentMillis = millis();
    if(retryStart > currentMillis){
      retryStart = 0;
    }
    if(currentMillis - retryStart > TIME_BETWEEN_RETRY_MS){
      isPendingRetry = false;
    }
    delay(100);
  }else{
    loadingLed();
    completedDailyCheck();
    if(ledState == 0){
      failureLedSound();
    }else if(ledState == 1){
      successLed();
    }
    Serial.println("Now waiting 30 minutes till next loop, reset to try again earlier");
    retryStart = millis();
    isPendingRetry = true;
  }
}