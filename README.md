# LeetCode Alarm
This project is a little alarm for that checks if you did your daily leetcode for you!
# Requirements
* DOIT ESP32 DEVKIT V1 (or another ESP32 board)
* Speaker
* LED
* Resistors
* A leetcode account
* Arduino IDE
# Setup
1. Hardware Setup (Change respective pin number in constants.h as required for your own pin numbers)
  * My LED is connected to pin 23(GPIO) with a resistor to ground
  * My Buzzer is connected to pin 21(GPIO PWM Enabled) with a resistor to ground
2. Software Setup
  * Clone the project into a folder */alarm (necessary for arduino ide)
  * Create credentials.h and copy in the example in credentials_example.h, filling in your credentials
  * (Optional) Adjust the values in constants.h to your preference
  * Using Arduino IDE, verify and upload the code to your DOIT ESP32 DEVKIT V1 or other board using. [Setup](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/)
# Functionality
## Overview
The alarm consists of an LED signalling completion of daily and a buzzer that serves as an alarm when haven't completed.
## Timeline
This timeline assumes daily is NOT yet completed during any of the retries  
When detected as daily completed, will stop retrying until 8.00am the following day
* 8.30am: Run Daily Completion Check to reset for new daily problem
  * Alarm: No
  * RetryFrequency: 1 hour
  * Not at home most days, unlikely to complete before next interval
* 8.00pm: Estimated time to be home and resting (Ideally completing daily here)
  * Alarm: No
  * RetryFrequency: 30 minutes
  * Don't want an alarm if I am trying to rest for a bit and might annoy others unnecessarily while there is still time
* 11.00pm: Estimated time to be on bed about to sleep
  * Alarm: Yes
  * RetryFrequency: 15 minutes (Should still not hit rate limit of leetcode alfa-api calls)
  * This is very annoying and will force me to wake up and do it
 

*RetryFrequency refers to the frequency that the leetcode alfa-api is called to check daily completion status, also the frequency the alarm is sounded when status is incompleted*
