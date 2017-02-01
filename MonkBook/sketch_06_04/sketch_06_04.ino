//sketch 06-04 -- mead mod

byte inputPin = 2;   // was int but why waste memory?
byte ledPin = 9;

void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(inputPin, INPUT);
  digitalWrite(inputPin, HIGH);  //enable internal pullup on input
}

void loop()
{
  bool switchOpen = digitalRead(inputPin);  // was int but why waste memory?
  digitalWrite(ledPin, ! switchOpen);
  
  // even simpler,just say  digitalWrite(ledPin, ! digitalRead(inputPin));
}
