/*
 SiblDigitalReadLED -- non BLE, board test of button controlling LED
 Reads digital input on pin 3 (of Spark BOB), prints to the serial monitor 

 */

// digital pin 3 wired to D3 of Spark BOB. Give it a name:
int pushButton = 3;

// setup routine runs once when you press reset, power up or open Ser. Mon.
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // make the pushbutton's pin an input:
  pinMode(pushButton, INPUT_PULLUP);
  pinMode(2, OUTPUT);  // LED on D2
}

void loop() {
  // read the input pin:
  int buttonState = digitalRead(pushButton); // low when pressed
  // print out the state of the button:
  Serial.println(buttonState);
  digitalWrite(2,!buttonState);  // LED on when button pressed
  delay(200);        // delay in between reads for print sanity
}



