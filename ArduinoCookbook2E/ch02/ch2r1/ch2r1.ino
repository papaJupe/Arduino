const int ledPin =  13;    // LED connected to digital pin 13

// The setup() method runs once, when the sketch starts
void setup()
{
   pinMode(ledPin, OUTPUT);      // initialize the digital pin as an output
}

// the loop() method runs over and over again,
void loop()
{
   digitalWrite(ledPin, HIGH);   // turn the LED on
   delay(1000);                  // wait a second
   digitalWrite(ledPin, LOW);    // turn the LED off
   delay(1000);                  // wait a second
}
      
    
