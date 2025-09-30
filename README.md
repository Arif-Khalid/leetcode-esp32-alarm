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
  * My Buzzer is connected to pin 22(GPIO PWM Enabled) with a resistor to ground
  * (Optional)10 segment [LED BAR](https://www.digikey.sg/en/products/detail/kingbright/DC10GWA/1747576) that I have configured to show the progress till the next api check and the error code if there is one
2. Software Setup
  * Clone the project into a folder */alarm (necessary for arduino ide)
  * Create credentials.h and copy in the example in credentials_example.h, filling in your credentials. `unique_base_url` can be set to "https://alfa-leetcode-api.onrender.com/" to use public api. See [alfa-leetcode-api repo](https://github.com/alfaarghya/alfa-leetcode-api/)
  * (Optional) Adjust the values in constants.h to your preference
  * Using Arduino IDE, verify and upload the code to your DOIT ESP32 DEVKIT V1 or other board using. [Setup](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/)
3. (Optional) Host your own LeetCode API
  * This is to solve the problem of unpredictable rate limits from the public api as specified in step 2. Your rate limit can increase or decrease depending on traffic to the api, resulting in regular http code 429 errors.
  * Fortunately, the legendary OSS contributors to the [alfa-leetcode-api repo](https://github.com/alfaarghya/alfa-leetcode-api/) made and published a docker image.
  * Go to your favourite cloud platform, I used Google Cloud Platform, specifically [Google Cloud Run](https://cloud.google.com/run).
  * With google cloud run, you can input the public docker image found in [alfa-leetcode-api repo](https://github.com/alfaarghya/alfa-leetcode-api/), `alfaarghya/alfa-leetcode-api:2.0.1` at the time of writing, go through a simple setup of the request based instance and they give you a unique url.
  * Input this url as `unique_base_url` (remember to end with '/') in `credentials.h` and thats it, no more rate limit problems!
  * Cost of the default setup I have here is estimated at $0.00 - $0.01 per month
  * Be wary of cost buildup as you vary the query frequencies, read up on the [cloud run pricing](https://cloud.google.com/run#pricing)
# Functionality
## Overview
The alarm consists of an LED signalling completion of daily and a buzzer that serves as an alarm when haven't completed.
## Timeline
This timeline assumes daily is NOT yet completed during any of the retries  
When detected as daily completed, will stop retrying until 8.30am the following day
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
  * RetryFrequency: 30 minutes
  * This is very annoying and will force me to wake up and do it

*Timeline can be configured through `constants.h` file*  
*RetryFrequency refers to the frequency that the leetcode alfa-api is called to check daily completion status, each status check sounds the alarm if checked as incomplete*
# Things to note
* The method of checking if your leetcode daily is completed is by checking your last accepted submission title is the same as the leetcode daily problem title
* Therefore if you complete the daily and then proceed to submit an accepted solution to another problem before the retry frequency, it will detect as uncompleted
* To fix this issue, just submit the daily again so your last AC is the daily once more

