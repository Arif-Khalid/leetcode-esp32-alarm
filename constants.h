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
const int BEEP_DURATION_MS = 150;
const int PAUSE_DURATION_MS = 100;

const unsigned long TIME_BETWEEN_RETRY_MS = 1800000;
#endif