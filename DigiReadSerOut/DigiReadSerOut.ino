/*
DigitalReadSerialOut
 Reads a digital input on pin 2, prints the result to the serial monitor 
*/

// digital pin 2 --> pushbutton to ground; give it a name:
int pushButton = 2;

// the setup routine runs on load and when you press reset:
void setup() 
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // make the pushbutton's pin an input:
  pinMode(pushButton, INPUT_PULLUP);
       // old method pinMode(...INPUT); digitalWrite(pushButton, HIGH);
}

// loop runs over and over:
void loop() 
{
  // read the input pin:
  int buttonState = digitalRead(pushButton);
  // print out the state of the button:
  Serial.println(buttonState);  // could just put dR as argument
  delay(500);        // delay between reads & prints
}