/*
 * EZ1Rangefinder Distance Sensor
 * prints distance and changes LED flash rate
 * depending on distance from the sensor
 */

const int sensorPin = 5;
const int ledPin  = 13; // pin connected to LED

long value = 0;
int cm = 0;
int inches = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop()
{
  value = pulseIn(sensorPin, HIGH) ;
  cm = value / 58;        // pulse width is 58 microseconds per cm
  inches = value / 147;   // which is 147 microseconds per inch
  Serial.print(cm);
  Serial.print(',');
  Serial.println(inches);

  digitalWrite(ledPin, HIGH);
  delay(cm * 10 ); // each centimeter adds 10 milliseconds delay
  digitalWrite(ledPin, LOW);
  delay( cm * 10);

  delay(20);
}
      
    
