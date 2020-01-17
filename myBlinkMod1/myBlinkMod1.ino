/*
 Blink
 Turns on an LED for (blinkDelay), then off for (blinkDelay) repeatedly.
 I modified to accept input from serial mon or pot, and show the delay
 */

// Pin 13 wired to LED on most Arduino boards;
// give it a name:
int led = 13;     // on Mega A1 is 55, A3 57
int blinkDelay = 500;

// the setup routine runs once when you power up, press reset
// or open new Serial Monitor window:

void setup() 
{ 
 //   pinMode(A2, INPUT);   // input for AR
//  pinMode(A4, OUTPUT); // A4 works as ground for pot
//  digitalWrite(A4, LOW);
//  pinMode(A5, OUTPUT); // gives 5v for pot
//  digitalWrite(A5, HIGH);
                 
  // initialize the digital pin for led as an output
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  Serial.print("default delay is ");
  Serial.println(blinkDelay);
}

// the loop runs over and over
void loop() 
{
  if (Serial.available())  // false until some ser input arrives
    {  
      blinkDelay = Serial.parseInt(); // reads chars as numbers, empties ser. buffer
      delay(10);
      Serial.print("delay is now "); // print new value
      Serial.println(blinkDelay);  // when you open ser. mon., delay reverts to
          //  original default because break in serial comm causes Reset
    }
  
  //read pot value on A_ (0-1024), convert to ms for delay (200-2000)
  // blinkDelay = map(analogRead(A2),2,1020,200,2000);
  digitalWrite(led, HIGH);   // turn the LED on
  delay(blinkDelay);         //  for this long
  digitalWrite(led, LOW);    // turn the LED off
  delay(blinkDelay);         // off for this long

}
