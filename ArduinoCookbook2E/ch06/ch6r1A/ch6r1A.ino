/*
  shaken sketch
  tilt sensor connected to pin 2
  led connected to pin 13
*/

const int tiltSensorPin = 2;
const int ledPin = 13;
int tiltSensorPreviousValue = 0;
int tiltSensorCurrentValue = 0;
long lastTimeMoved = 0;
int shakeTime=50;

void setup()
{
  pinMode (tiltSensorPin, INPUT);
  digitalWrite (tiltSensorPin, HIGH);
  pinMode (ledPin, OUTPUT);
}

void loop()
{
  tiltSensorCurrentValue=digitalRead(tiltSensorPin);
  if (tiltSensorPreviousValue != tiltSensorCurrentValue){
    lastTimeMoved = millis();
    tiltSensorPreviousValue = tiltSensorCurrentValue;
  }

  if (millis() - lastTimeMoved < shakeTime){
    digitalWrite(ledPin, HIGH);
  }
  else{
    digitalWrite(ledPin, LOW);
  }
}
      
    
