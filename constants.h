#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "credentials.h"
const String BASE_URL = "https://alfa-leetcode-api.onrender.com/";
const String DAILY_ROUTE = "daily";
const String LAST_AC_ROUTE = String(username) + "/acSubmission?limit=1";

// LED Vars
const long INTERVAL = 100; // interval in ms (500 ms = 0.5s)
const int LED_PIN = 23;     // GPIO pin for LED

// SPEAKER Vars
const int SPEAKER_PIN = 22;
const int PWM_CHANNEL = 0;
const int ERROR_FREQ = 1000;
const int BEEP_DURATION_MS = 150;
const int PAUSE_DURATION_MS = 100;

const unsigned long TIME_BETWEEN_RETRY_MS = 1800000;
#endif