byte ledMap[72][2] = { { 7, 8 }, { 8, 7 }, { 7, 9 }, { 9, 7 } };

int photoResistorValue;

byte hourLed;
byte minLed;

byte photoResistorPin = 0;
byte transistor1ControlPin = 7;
byte transistor2ControlPin = 8;
byte thirdPin = 9;
bool alternate;
byte ledDelay = 1;

void setup() {
	for (byte i = 0; i < 10; i++) {
		pinMode(i, OUTPUT);
		digitalWrite(i, HIGH);
	}
	setHourLed(0);
	setMinLed(1);
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
	photoResistorValue = analogRead(photoResistorPin);
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
		setToInput(minLed);
		delay(ledDelay);
		setToOutput(hourLed);
		digitalWrite(ledMap[hourLed][0], HIGH);
		digitalWrite(ledMap[hourLed][1], LOW);
	} else {
		setToInput(hourLed);
		delay(ledDelay);
		setToOutput(minLed);
		digitalWrite(ledMap[minLed][0], HIGH);
		digitalWrite(ledMap[minLed][1], LOW);
	}
	alternate = !alternate;
}

void setToInput(byte led) {
	pinMode(ledMap[led][0], INPUT);
	pinMode(ledMap[led][1], INPUT);
}

void setToOutput(byte led) {
	pinMode(ledMap[led][0], OUTPUT);
	pinMode(ledMap[led][1], OUTPUT);
}
