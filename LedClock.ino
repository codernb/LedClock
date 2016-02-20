#include <Arduino.h>

#define PHOTO_RESISTOR_PIN 0
#define MAX_DELAY 13
#define MIN_DELAY 0
#define DOWN_BARRIER (downBarrierValue - ledDelay) * 50
#define UP_BARRIER (upBarrierValue - ledDelay) * 50
#define MINUTE_BUTTON_PIN 12
#define HOUR_BUTTON_PIN 13

const byte ledMap[72][2] = { { 1, 2 }, { 2, 1 }, { 3, 2 }, { 2, 3 }, { 3, 4 }, {
		4, 3 }, { 5, 4 }, { 4, 5 }, { 5, 6 }, { 6, 5 }, { 7, 6 }, { 6, 7 }, { 7,
		8 }, { 8, 7 }, { 9, 8 }, { 8, 9 }, { 9, 7 }, { 7, 9 }, { 5, 7 },
		{ 7, 5 }, { 5, 3 }, { 3, 5 }, { 1, 3 }, { 3, 1 }, { 1, 4 }, { 4, 1 }, {
				7, 4 }, { 4, 7 }, { 7, 2 }, { 2, 7 }, { 4, 2 }, { 2, 4 },
		{ 4, 6 }, { 6, 4 }, { 8, 6 }, { 6, 8 }, { 8, 5 }, { 5, 8 }, { 2, 5 }, {
				5, 2 }, { 2, 9 }, { 9, 2 }, { 6, 9 }, { 9, 6 }, { 6, 3 },
		{ 3, 6 }, { 9, 3 }, { 3, 9 }, { 9, 5 }, { 5, 9 }, { 1, 5 }, { 5, 1 }, {
				1, 6 }, { 6, 1 }, { 2, 6 }, { 6, 2 }, { 2, 8 }, { 8, 2 },
		{ 4, 8 }, { 8, 4 }, { 7, 1 }, { 1, 7 }, { 7, 3 }, { 3, 7 }, { 8, 3 }, {
				3, 8 }, { 8, 1 }, { 1, 8 }, { 9, 1 }, { 1, 9 }, { 9, 4 },
		{ 4, 9 } };

const byte downBarrierValue = MAX_DELAY + 2;
const byte upBarrierValue = MAX_DELAY + 3;

byte minuteLed;
byte hourLed;
byte minuteOffset;
byte hourOffset;

bool alternate;
byte ledDelay = 15;

bool minuteButtonPressed;
bool hourButtonPressed;

void setup() {
	pinMode(MINUTE_BUTTON_PIN, INPUT);
}

void loop() {
	time();
//	lightLevel();
	light();
//	buttons();
}

void time() {
	unsigned long int ms = millis();
//	byte min = (ms / 60000 + minuteOffset) % 60;
	byte min = (ms / 500 + minuteOffset) % 60;
//	byte hour = (ms / 3600000 + hourOffset) % 12;
	byte hour = (ms / 30000 + hourOffset) % 12;
	setMinuteLed(min);
	setHourLed(hour);
}

void setMinuteLed(byte pin) {
	if (pin == minuteLed)
		return;
	setToInput(minuteLed);
	minuteLed = pin;
}

void setHourLed(byte pin) {
	pin += 60;
	if (pin == hourLed)
		return setToInput(hourLed);
	hourLed = pin;
}

void buttons() {
	byte minuteButtonState = digitalRead(MINUTE_BUTTON_PIN);
	byte houreButtonState = digitalRead(HOUR_BUTTON_PIN);
	if (!minuteButtonPressed && minuteButtonState == HIGH) {
		addMinOffset();
		minuteButtonPressed = true;
	} else if (minuteButtonState == LOW) {
		minuteButtonPressed = false;
	}
}

void addMinOffset() {
	minuteOffset = (minuteOffset + 1) % 60;
}

void addHourOffset() {
	hourOffset = (hourOffset + 1) % 12;
}

void lightLevel() {
	int photoResistorValue = analogRead(PHOTO_RESISTOR_PIN);
	if (ledDelay < MAX_DELAY && photoResistorValue < DOWN_BARRIER)
		ledDelay++;
	else if (ledDelay > MIN_DELAY && photoResistorValue > UP_BARRIER)
		ledDelay--;
}

void light() {
	if (alternate) {
		setToInput(hourLed);
		delay(ledDelay);
		setActive(minuteLed);
	} else {
		setToInput(minuteLed);
		delay(ledDelay);
		setActive(hourLed);
	}
	alternate = !alternate;
}

void setActive(byte led) {
	setToOutput(led);
	digitalWrite(ledMap[led][0], HIGH);
	digitalWrite(ledMap[led][1], LOW);
}

void setToInput(byte led) {
	pinMode(ledMap[led][0], INPUT);
	pinMode(ledMap[led][1], INPUT);
}

void setToOutput(byte led) {
	pinMode(ledMap[led][0], OUTPUT);
	pinMode(ledMap[led][1], OUTPUT);
}
