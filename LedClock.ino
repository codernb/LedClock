#include <Arduino.h>

#define PHOTO_RESISTOR_PIN 0
#define MAX_DELAY 13
#define MIN_DELAY 0
#define DOWN_BARRIER (downBarrierValue - ledDelay) * 50
#define UP_BARRIER (upBarrierValue - ledDelay) * 50
#define MINUTE_BUTTON_PIN 12
#define HOUR_BUTTON_PIN 11
#define PIN_OFFSET 1

const byte minutesMap[60][2] = {
		{ 8, 4 },
		{ 1, 2 },
		{ 2, 1 },
		{ 3, 2 },
		{ 2, 3 },
		{ 3, 4 },
		{ 4, 3 },
		{ 5, 4 },
		{ 4, 5 },
		{ 5, 6 },
		{ 6, 5 },
		{ 7, 6 },
		{ 6, 7 },
		{ 7, 8 },
		{ 8, 7 },
		{ 9, 8 },
		{ 8, 9 },
		{ 9, 7 },
		{ 7, 9 },
		{ 5, 7 },
		{ 7, 5 },
		{ 5, 3 },
		{ 3, 5 },
		{ 1, 3 },
		{ 3, 1 },
		{ 1, 4 },
		{ 4, 1 },
		{ 7, 4 },
		{ 4, 7 },
		{ 7, 2 },
		{ 2, 7 },
		{ 4, 2 },
		{ 2, 4 },
		{ 4, 6 },
		{ 6, 4 },
		{ 8, 6 },
		{ 6, 8 },
		{ 8, 5 },
		{ 5, 8 },
		{ 2, 5 },
		{ 5, 2 },
		{ 2, 9 },
		{ 9, 2 },
		{ 6, 9 },
		{ 9, 6 },
		{ 6, 3 },
		{ 3, 6 },
		{ 9, 3 },
		{ 3, 9 },
		{ 9, 5 },
		{ 5, 9 },
		{ 1, 5 },
		{ 5, 1 },
		{ 1, 6 },
		{ 6, 1 },
		{ 2, 6 },
		{ 6, 2 },
		{ 2, 8 },
		{ 8, 2 },
		{ 4, 8 }
};

const byte hoursMap[12][2] = {
		{ 4, 9 },
		{ 7, 1 },
		{ 1, 7 },
		{ 7, 3 },
		{ 3, 7 },
		{ 8, 3 },
		{ 3, 8 },
		{ 8, 1 },
		{ 1, 8 },
		{ 9, 1 },
		{ 1, 9 },
		{ 9, 4 }
};

const byte downBarrierValue = MAX_DELAY + 2;
const byte upBarrierValue = MAX_DELAY + 3;

byte minute;
byte hour;
byte oldMinute;

bool alternate;
byte ledDelay;

bool minuteButtonPressed;
bool hourButtonPressed;

void setup() {
	pinMode(MINUTE_BUTTON_PIN, INPUT);
	pinMode(HOUR_BUTTON_PIN, INPUT);
}

void loop() {
	time();
	lightLevel();
	lightInterval();
	buttons();
}

void time() {
	unsigned long int ms = millis();
	byte newMinute = (ms / 60000) % 60;
	if (newMinute != oldMinute) {
		incrementMinute();
		oldMinute = newMinute;
	}
}

void buttons() {
	byte minuteButtonState = digitalRead(MINUTE_BUTTON_PIN);
	byte hourButtonState = digitalRead(HOUR_BUTTON_PIN);
	if (!minuteButtonPressed && minuteButtonState == HIGH) {
		incrementMinute();
		minuteButtonPressed = true;
	} else if (minuteButtonState == LOW) {
		minuteButtonPressed = false;
	}
	if (!hourButtonPressed && hourButtonState == HIGH) {
		incrementHour();
		hourButtonPressed = true;
	} else if (hourButtonState == LOW) {
		hourButtonPressed = false;
	}
}

void incrementMinute() {
	setMinuteInactive();
	minute = (minute + 1) % 60;
	if (minute == 0)
		incrementHour();
}

void incrementHour() {
	setHourInactive();
	hour = (hour + 1) % 12;
}

void lightLevel() {
	int photoResistorValue = analogRead(PHOTO_RESISTOR_PIN);
	if (ledDelay < MAX_DELAY && photoResistorValue < DOWN_BARRIER)
		ledDelay++;
	else if (ledDelay > MIN_DELAY && photoResistorValue > UP_BARRIER)
		ledDelay--;
}

void lightInterval() {
	if (alternate) {
		setHourInactive();
		delay(ledDelay);
		setMinuteActive();
	} else {
		setMinuteInactive();
		delay(ledDelay);
		setHourActive();
	}
	alternate = !alternate;
}

void setMinuteActive() {
	setActive(getMinuteHighPin(), getMinuteLowPin());
}

void setHourActive() {
	setActive(getHourHighPin(), getHourLowPin());
}

void setMinuteInactive() {
	setToInput(getMinuteLowPin());
	setToInput(getMinuteHighPin());
}

void setHourInactive() {
	setToInput(getHourLowPin());
	setToInput(getHourHighPin());
}

byte getMinuteHighPin() {
	return minutesMap[minute][0];
}

byte getMinuteLowPin() {
	return minutesMap[minute][1];
}

byte getHourHighPin() {
	return hoursMap[hour][0];
}

byte getHourLowPin() {
	return hoursMap[hour][1];
}

void setActive(byte highPin, byte lowPin) {
	setToOutput(highPin);
	setToOutput(lowPin);
	setToHigh(highPin);
	setToLow(lowPin);
}

void setToHigh(byte pin) {
	digitalWrite(pin + PIN_OFFSET, HIGH);
}

void setToLow(byte pin) {
	digitalWrite(pin + PIN_OFFSET, LOW);
}

void setToInput(byte pin) {
	pinMode(pin + PIN_OFFSET, INPUT);
}

void setToOutput(byte pin) {
	pinMode(pin + PIN_OFFSET, OUTPUT);
}
