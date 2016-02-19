#include <Arduino.h>

#define PHOTO_RESISTOR_PIN 0

byte ledMap[72][2] = { { 1, 2 }, { 2, 1 }, { 3, 2 }, { 2, 3 }, { 3, 4 },
		{ 4, 3 }, { 5, 4 }, { 4, 5 }, { 5, 6 }, { 6, 5 }, { 7, 6 }, { 6, 7 }, {
				7, 8 }, { 8, 7 }, { 9, 8 }, { 8, 9 }, { 9, 7 }, { 7, 9 },
		{ 5, 7 }, { 7, 5 }, { 5, 3 }, { 3, 5 }, { 1, 3 }, { 3, 1 }, { 1, 4 }, {
				4, 1 }, { 7, 4 }, { 4, 7 }, { 7, 2 }, { 2, 7 }, { 4, 2 },
		{ 2, 4 }, { 4, 6 }, { 6, 4 }, { 8, 6 }, { 6, 8 }, { 8, 5 }, { 5, 8 }, {
				2, 5 }, { 5, 2 }, { 2, 9 }, { 9, 2 }, { 6, 9 }, { 9, 6 },
		{ 6, 3 }, { 3, 6 }, { 9, 3 }, { 3, 9 }, { 9, 5 }, { 5, 9 }, { 1, 5 }, {
				5, 1 }, { 1, 6 }, { 6, 1 }, { 2, 6 }, { 6, 2 }, { 2, 8 },
		{ 8, 2 }, { 4, 8 }, { 8, 4 }, { 7, 1 }, { 1, 7 }, { 7, 3 }, { 3, 7 }, {
				8, 3 }, { 3, 8 }, { 8, 1 }, { 1, 8 }, { 9, 1 }, { 1, 9 },
		{ 9, 4 }, { 4, 9 } };

byte minLed = 255;
byte hourLed = 255;

bool alternate;
byte ledDelay;

void setup() {
	Serial.begin(9600);
}

void loop() {
	time();
	lightLevel();
	light();
}

void time() {
	unsigned long int ms = millis();
	byte min = ms / 60000 % 60;
	byte hour = ms / 3600000 % 12;
	if (min != minLed) {
		Serial.print("min: ");
		Serial.println(min);
		setMinLed(min);
	}
	if (hour + 60 != hourLed) {
		Serial.print("h: ");
		Serial.println(hour + 60);
		setHourLed(hour + 60);
	}
}

void setMinLed(byte pin) {
	setToInput(minLed);
	minLed = pin;
}

void setHourLed(byte pin) {
	setToInput(hourLed);
	hourLed = pin;
}

void lightLevel() {
	int photoResistorValue = analogRead(PHOTO_RESISTOR_PIN);
	switch (ledDelay) {
	case (0):
		if (photoResistorValue < 200)
			ledDelay = 1;
		break;
	case (1):
		if (photoResistorValue > 300)
			ledDelay = 0;
		else if (photoResistorValue < 100)
			ledDelay = 2;
		break;
	case (2):
		if (photoResistorValue > 200)
			ledDelay = 1;
		break;
	}
}

void light() {
	if (alternate) {
		setToInput(hourLed);
		delay(ledDelay);
		setActive(minLed);
	} else {
		setToInput(minLed);
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
