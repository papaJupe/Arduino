const int ledPin =  13;    // LED connected to digital pin 13

void setup()
{
  pinMode(ledPin, OUTPUT);
}

void loop()
{
  digitalWrite(ledPin, HIGH);   // set the LED on
  delay(2000);                  // wait for two seconds
  digitalWrite(ledPin, LOW);    // set the LED off
  delay(2000);                  // wait for two seconds
}
      
    
