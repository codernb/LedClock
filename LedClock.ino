#define PHOTO_RESISTOR_PIN 0

byte ledMap[72][2] = { { 7, 8 }, { 8, 7 }, { 7, 9 }, { 9, 7 } };

byte hourLed;
byte minLed;

bool alternate;
byte ledDelay;

void setup() {
	;
}

void loop() {
	lightLevel();
	light();

	if (millis() / 2000 % 2 == 0) {
		setMinLed(2);
		setHourLed(1);
	} else {
		setMinLed(0);
		setHourLed(3);
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
