#define PIN_PHOTO A4

#define PIN_BUZZER 6
#define PIN_LED 3

#define PIN_CAR_LIGTHS_ON 7
#define PIN_DISABLE_ALARM 8

#define SENSOR_TOO_DARK 200
#define SENSOR_DAY_LIGHT 300

/* 
 * The "arrow" is moving slowly between 
 * ARROW_NIGHT_VALUE and ARROW_DAY_VALUE.
 */
#define ARROW_NIGHT_VALUE 0
#define ARROW_DAY_VALUE 40

bool day = true;
int arrow = ARROW_DAY_VALUE;
int alarmCounter = 0;

void alarmOff() {
  digitalWrite(PIN_LED, LOW);
  noTone(PIN_BUZZER);
  alarmCounter = 0;
}

void alarmWithLED() {
  digitalWrite(PIN_LED, LOW);
  if (
    alarmCounter == 0 || alarmCounter == 1
    || alarmCounter == 5 || alarmCounter == 7
  ) {
    digitalWrite(PIN_LED, HIGH);
  }
}

void alarmWithTone() {
  if (alarmCounter == 0)
    tone(PIN_BUZZER, 1976);
  else if (alarmCounter == 1)
    tone(PIN_BUZZER, 1760);
  else
    noTone(PIN_BUZZER);
}

void alarmOn() {
  alarmWithLED();
  alarmWithTone();

  if (++alarmCounter >= 40)
    alarmCounter = 0;
}

bool isCarLightsOn() {
  return digitalRead(PIN_CAR_LIGTHS_ON) == HIGH;
}

bool isDisableAlarm() {
  return digitalRead(PIN_DISABLE_ALARM) == LOW;
}

bool isTooDarkSensor(int photoValue) {
  return photoValue <= SENSOR_TOO_DARK;
}

bool isDayLightSensor(int photoValue) {
  return photoValue >= SENSOR_DAY_LIGHT;
}

void analyzeDayOrNight(int photoValue) {
  if (isTooDarkSensor(photoValue))
    arrow -= 1;

  if (isDayLightSensor(photoValue))
    arrow += 2; // Move the arrow faster from night to day: += 2

  if (isDisableAlarm() || isCarLightsOn())
    arrow = ARROW_DAY_VALUE;

  if (arrow >= ARROW_DAY_VALUE) {
    arrow = ARROW_DAY_VALUE;
    day = true;
  }

  if (arrow <= ARROW_NIGHT_VALUE) {
    arrow = ARROW_NIGHT_VALUE;
    day = false;
  }
}

int readLightSensor() {
  return analogRead(PIN_PHOTO);
}

void checkAutolight() {
  analyzeDayOrNight(readLightSensor());

  if (day)
    alarmOff();
  else
    alarmOn();
}

void setupAutolight() {
  pinMode(PIN_PHOTO, INPUT);
  pinMode(PIN_CAR_LIGTHS_ON, INPUT);
  pinMode(PIN_DISABLE_ALARM, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
}

void setup() {
  setupAutolight();
}

void loop() {
  checkAutolight();
  delay(250);
}

