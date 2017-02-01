/* KnightRider
 */

const int NbrLEDs = 6;
const int ledPins[] = {2, 3, 4, 5, 6, 7};
const int wait = 30;

void setup(){
  for (int led = 0; led < NbrLEDs; led++)
  {
    pinMode(ledPins[led], OUTPUT);
  }
}

void loop() {
  for (int led = 0; led < NbrLEDs-1; led++)
  {
    digitalWrite(ledPins[led], HIGH);
	delay(wait);
	digitalWrite(ledPins[led + 1], HIGH);
	delay(wait);
	digitalWrite(ledPins[led], LOW);
	delay(wait*2);
  }
	for (int led = NbrLEDs-1; led > 0; led--) {
	digitalWrite(ledPins[led], HIGH);
	delay(wait);
	digitalWrite(ledPins[led - 1], HIGH);
	delay(wait);
	digitalWrite(ledPins[led], LOW);
	delay(wait*2);
  }
}