/* piezo sketch
 * lights and LED when the Piezo is tapped
 */

const int sensorPin = 0;  // the analog pin connected to the sensor
const int ledPin  = 13;     // pin connected to LED
const int THRESHOLD = 100;


void setup()
{
   pinMode(ledPin, OUTPUT);
}

void loop()
{
  int val = analogRead(sensorPin);
  if (val >= THRESHOLD)
  {
    digitalWrite(ledPin, HIGH);
    delay(100);  // to make the LED visible
  }
  else
    digitalWrite(ledPin, LOW);
}
      
    
