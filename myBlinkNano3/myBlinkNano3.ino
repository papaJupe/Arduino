/*
 Blink
 Turns on an LED on for (blinkDelay), then off for (blinkDelay) repeatedly.
 I modified to accept input from serial monitor and show what's running;
 nano mod for AR input from pot to adj rate, NB A6-7 only good for input
 */

// Pin 13 has an LED installed on most Arduino boards;
// give it a name:
int led = 13;
int blinkDelay = 1500;

// the setup routine runs once when you power up or press reset:

void setup() 
{ 
  pinMode(A6, INPUT);   // input for AR
  pinMode(A4, OUTPUT); // A4 works as ground for pot
  digitalWrite(A4, LOW);
  pinMode(A5, OUTPUT); // gives 5v for pot
  digitalWrite(A5, HIGH);
                 
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  Serial.print("default delay is ");
  Serial.println(blinkDelay);
}

// the loop runs over and over:
void loop() 
{
  if (Serial.available())  // false until some ser input arrives
  {   
    // blinkDelay = Serial.parseInt(); // reads characters as numbers, empties serial buffer
    delay(10);
    Serial.print("delay is now "); // print new value
    Serial.println(blinkDelay);  // when you open ser. mon., delay reverts to original default
        // because break in serial comm causes Reset
  }
  //read pot value on A6 (0-1024), convert to ms for delay (200-2000)
  blinkDelay = map(analogRead(A6),2,1024,200,2000);
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(blinkDelay);               // wait a bit
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(blinkDelay);               // wait a bit

}

