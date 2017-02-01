/*
 * RespondingToChanges sketch
 * flash an LED to indicate low voltage levels
 */

long batteryFull       = 1500;  // millivolts for a full battery
long warningThreshold  = 1200;  // Warning level in millivolts - LED flashes
long criticalThreshold = 1000;  // Critical voltage level - LED stays on

const int batteryPin = 0;
const int ledPin = 13;

void setup()
{
   pinMode(ledPin, OUTPUT);
}

void loop()
{
  int val = analogRead(batteryPin);    // read the value from the sensor
  int mv = map(val, 0,1023, 0,5000); 
  if(mv < criticalThreshold)
     flash(0);
  else if (mv < warningThreshold) {
    // in the line above, L following a number makes it a 32 bit value
    int percent = map(mv, criticalThreshold, batteryFull, 0, 100);
    constrain(percent, 0,100); 
    flash(percent) ;
  }
  delay(1);
}

// function to flash an led with on/off time determined by value
// passed as percent
void flash(int percent)
{
  digitalWrite(ledPin, HIGH);
  delay(100 - percent +1);  // ensure delay is > 0
  digitalWrite(ledPin, LOW);
  delay(percent + 1);
}
      
    
