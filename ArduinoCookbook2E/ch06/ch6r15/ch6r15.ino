/*
  gyro sketch
  displays the rotation rate on the Serial Monitor
 */


const int inputPin = 0;  // analog input 0
int rotationRate = 0;

void setup()
{
  Serial.begin(9600);   // sets the serial port to 9600
}

void loop()
{
  rotationRate = analogRead(inputPin); // read the gyro output
  Serial.print("rotation rate is ");
  Serial.println(rotationRate);
  delay(100);                          // wait 100ms for next reading
}
