# LeetCode Alarm
This project is a little alarm for that checks if you did your daily leetcode for you!
# Requirements
* [DOIT ESP32 DEVKIT V1](https://www.espboards.dev/esp32/esp32doit-devkit-v1/) (or another ESP32 board)
* Single 2 pin [Speaker/Buzzer](https://sg.element14.com/multicomp-pro/mckpt-g1720-3922/piezo-buzzer/dp/1756525)
* Single 2 pin [LED](https://sg.element14.com/cree-led/c513a-wsn-cw0z0151/led-5mm-12cd-cool-white/dp/2579515)
* 12 * [330Ω Resistors](https://www.allelcoelec.com/blog/330-ohm-resistor-and-color-codes.html)
* A [LeetCode](https://leetcode.com/) account
* [Arduino](https://www.arduino.cc/) IDE
* (Optional) [LED BAR](https://www.digikey.sg/en/products/detail/kingbright/DC10GWA/1747576)
# Setup
1. Hardware Setup (Change respective pin number in constants.h as required for your own pin numbers)
  * Circuit diagram made in [circuit-diagram.org](https://www.circuit-diagram.org/), found as `circuit.cddx` in repo root, also shown as an image below
    * *Please follow your specific ESP32 datasheet for accurate pin mappings*
  * My LED is connected to pin 23(GPIO) with a resistor to ground
  * My Buzzer is connected to pin 22(GPIO PWM Enabled) with a resistor to ground
  * (Optional)10 segment [LED BAR](https://www.digikey.sg/en/products/detail/kingbright/DC10GWA/1747576) that I have configured to show the progress till the next api check and the error code if there is one
    * This is represented by the 10 adjacent LEDs in circuit diagram  
![Circuit Diagram](https://github.com/user-attachments/assets/7a66c9e6-1abd-4bc1-9647-8623bcc501d7)
2. Software Setup
  * Clone the project into a folder */alarm (necessary for arduino ide)
  * Create credentials.h and copy in the fields in credentials_example.h, filling in your credentials. `unique_base_url` can be set to "https://alfa-leetcode-api.onrender.com/" to use public api. See [alfa-leetcode-api repo](https://github.com/alfaarghya/alfa-leetcode-api/)
  * (Optional) Adjust the values in constants.h to your preference
  * Using Arduino IDE, verify and upload the code to your DOIT ESP32 DEVKIT V1 or other board with this [tutorial](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/)
3. (Optional) Host your own LeetCode API
  * This is to solve the problem of unpredictable rate limits from the public api as specified in step 2  
    Your rate limit can increase or decrease depending on traffic to the api, resulting in regular http code 429 errors
  * Fortunately, the legendary OSS contributors to the [alfa-leetcode-api repo](https://github.com/alfaarghya/alfa-leetcode-api/) made and published a docker image
  * Go to your favourite cloud platform, I used Google Cloud Platform, specifically [Google Cloud Run](https://cloud.google.com/run)
  * With google cloud run, you can input the public docker image found in [alfa-leetcode-api repo](https://github.com/alfaarghya/alfa-leetcode-api/), `alfaarghya/alfa-leetcode-api:2.0.1` at the time of writing, go through a simple setup of the request based instance and they give you a unique url
  * Input this url as `unique_base_url` (remember to end with '/') in `credentials.h` and thats it, no more rate limit problems!
  * Cost of the default setup I have here is estimated at $0.00 - $0.01 per month
  * Be wary of cost buildup as you vary the query frequencies, read up on the [cloud run pricing](https://cloud.google.com/run#pricing)
# Functionality
## Overview
The alarm consists of an LED signalling completion of daily LeetCode problem and a buzzer that serves as an alarm when haven't completed
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

## Error Codes
### Feedback
* When an API error occurs, you will see the built-in LED of the ESP32 blinking  
* If LED BAR is added, error code can be seen via which LED is lit up on bar (0 based indexing)
  * Since `errorCode = 0` is no error, the first (0 index) of the LED BAR will never be lit up solely as an error code
  * This prevents confusion with regular retry progression display
  * The indexing of the LEDs of the bar is from top to bottom relative to the circuit diagram above. (GPIO 17 is index 0, GPIO 16 is index 1, etc.)
### Error code meanings
1. Too many requests (rate limit of server hit, response is http code 429)
2. HTTP query failed (likely timeout, server didn't respond, possibly offline)
3. WiFi not connected (WiFi network down/disconnected)
4. Time not retrieved (NTP error, used to sync with time server for accurate timings)
### Recommended actions
*All errors should be first confirmed by resetting the board, either with reset button on the ESP32, or turning off and on the power*  
*After every fix, reset the board again and check that error no longer occurs*

1. Too many requests
  * If using the public API, only option is to reduce query frequency, increase the time between checks in the config
  * If using the private API, either increase time between checks or configure your hosting service for more frequent requests
2. HTTP Query failed
  * Check on your own browser if you can visit the endpoints in your url like "https://alfa-leetcode-api.onrender.com/daily" (Replace appropriately with unique URL)
  * Increase `NETWORK_REQUEST_TIMEOUT_MS` field in `constants.h` in case api call is timing out
3. WiFi not connected
  * Check the WiFi specified is working on other devices
  * Ensure SSID and PASSWORD in credentials.h are correct
  * Shift ESP32 closer to router if necessary
4. Time not retrieved (NTP error)
  * This is not able to be handled by us since its the public NTP server error
  * The code falls back to first config behaviour (No alarm, 1 hour retry), does not reset daily check automatically
  * Can just reset the board at 8+a.m. manually to trigger daily reset of completion status in the meantime

# Things to note
* LeetCode daily completion logic checks your last accepted submission title against the leetcode daily problem title
* Therefore if you complete the daily and then proceed to submit an accepted solution to another problem before the retry frequency, it will detect as uncompleted
* To fix this issue, submit the daily again so your last accepted submission is the daily once more

