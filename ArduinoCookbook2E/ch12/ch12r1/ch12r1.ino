/*
 * delay sketch
 */

const long oneSecond = 1000;  // a second is a thousand milliseconds
const long oneMinute = oneSecond * 60;
const long oneHour   = oneMinute * 60;
const long oneDay    = oneHour * 24;


void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Serial.println("delay for 1 millisecond");
  delay(1);
  Serial.println("delay for 1 second");
  delay(oneSecond);
  Serial.println("delay for 1 minute");
  delay(oneMinute);
  Serial.println("delay for 1 hour");
  delay(oneHour);
  Serial.println("delay for 1 day");
  delay(oneDay);
  Serial.println("Ready to start over");
}
      
    
