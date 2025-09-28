#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "credentials.h"
const String BASE_URL = unique_base_url;  // Can be set to https://alfa-leetcode-api.onrender.com/ to use the public API. Problem is it has quite a strict rate limit,
const String DAILY_ROUTE = "daily";
const String LAST_AC_ROUTE = String(username) + "/acSubmission?limit=1";
const int NETWORK_REQUEST_TIMEOUT_MS = 60000;

// LED Vars
const long INTERVAL = 100; // interval in ms (500 ms = 0.5s)
const int LED_PIN = 23;     // GPIO pin for GREEN LED
const int LED_BAR_COUNT = 10;
const int LED_BAR_PINS[LED_BAR_COUNT] = { 17, 16, 4, 32, 33, 25, 26, 27, 14, 13 };

// SPEAKER Vars
const int SPEAKER_PIN = 21;
const int PWM_CHANNEL = 0;
const int ERROR_FREQ = 1000;
const int ERROR_BEEP_DURATION_MS = 150;
const int ERROR_PAUSE_DURATION_MS = 100;
const int SUCCESS_MELODY_COUNT = 7;
const int SUCCESS_MELODY[SUCCESS_MELODY_COUNT] = { 784, 880, 988, 1047, 1175, 1319, 1568 };  // G5 → A5 → B5 → C6 → D6 → E6 → G6
const int SUCCESS_TONE_DURATIONS[SUCCESS_MELODY_COUNT] = {500, 500, 500, 500, 500, 500, 500}; 
const int SUCCESS_TONE_DURATION = 150;
const int SUCCESS_PAUSE_DURATION = 50;

// TIME Vars
const long GMT_OFFSET_SEC = 28800;    // GMT+8 in SG
const int DAYLIGHT_OFFSET_SEC = 0;    // Offset for daylight savings time, not observed in SG
const char* NTP_SERVER = "pool.ntp.org";
struct Config {
  int hourStart;
  bool shouldPlayAlarm;
  unsigned long timeBetweenRetryMs;
};
struct Config config1 = { 0, false, 3600000 };
struct Config config2 = { 20, false, 1800000 };
struct Config config3 = { 23, true, 1800000 };
const int NUM_CONFIGS = 3;
const Config configs[NUM_CONFIGS] = {config1, config2, config3};

// Even though the new daily comes out at 8.00am, we reset at 8.30 to account for
// minor differences in time at LeetCode HQ and the NTP, ensuring new daily has come out by this time
const int RESET_HOUR = 8;
const int RESET_MIN = 30;

const unsigned long TIME_BETWEEN_RETRY_MS = 1800000;
const unsigned long TIME_BETWEEN_TIME_CHECK_MS = 900000;
#endif