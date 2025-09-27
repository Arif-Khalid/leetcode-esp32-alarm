unsigned long previousMillis = 0; // stores last toggle time for blinking LED
bool isLedOn = false;
void setAllLedBar(uint8_t val){
  for(int i = 0; i < LED_BAR_COUNT; ++i){
    digitalWrite(LED_BAR_PINS[i], val);
  }
}
void feedbackSetup(){
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  for (int i = 0; i < LED_BAR_COUNT; ++i){
    pinMode(LED_BAR_PINS[i], OUTPUT);
    digitalWrite(LED_BAR_PINS[i], LOW);
  }

  // Setup PWM on the speaker pin
  ledcAttachChannel(SPEAKER_PIN, 2000, 8, PWM_CHANNEL);
}

void loadingLed(){
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(LED_PIN, LOW);
  setAllLedBar(LOW);
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

void retryLed(float progress){
  digitalWrite(LED_BUILTIN, LOW);

  // Display progress on LED BAR
  // First bar signals 0 -> 1 / LED_BAR_COUNT progress
  // Full bar signals LED_BAR_COUNT - 1 / LED_BAR_COUNT progress
  progress *= LED_BAR_COUNT;
  // Prevent array overflow if so happen to have full progress which would light LED_BAR_COUNT bars
  if(progress >= LED_BAR_COUNT){
    progress -= 1;
  }
  for(int i = 0; i <= progress; ++i){
    digitalWrite(LED_BAR_PINS[i], HIGH);
  }

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

    // Overwrite LED BAR with error code
    if(errorCode >= LED_BAR_COUNT){
      // Exceed error code display capabilities fallback (Maybe can be improved with binary rep.)
      setAllLedBar(HIGH);
    }else{
      digitalWrite(LED_BAR_PINS[errorCode], HIGH);
    }
  }
}