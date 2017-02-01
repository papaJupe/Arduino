// this sketch triggers pin 13 if temp is greater than a threshold

const int inPin = 0;  // sensor connected to this analog pin
const int outPin = 13;  // digital output pin

const int threshold = 25; // the degrees celsius that will trigger the output pin


void setup()
{
  Serial.begin(9600);
  pinMode(outPin, OUTPUT);
}


void loop()
{
  int value = analogRead(inPin);
  long celsius =  (value * 500L) /1024;     // 10 mV per degree c, see text
  Serial.print(celsius);
  Serial.print(" degrees Celsius: ");
  if(celsius > threshold)
  {
     digitalWrite(outPin, HIGH);
     Serial.println("pin is on");
  }
  else
  {
     digitalWrite(outPin, LOW);
     Serial.println("pin is off");
  }
  delay(1000); // wait for one second
}
      
    
