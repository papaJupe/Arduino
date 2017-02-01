/*
  XBeeEcho
 Reply with whatever you receive over the serial port
 */

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  while (Serial.available() ) {
    Serial.write(Serial.read()); // reply with whatever you receive
  }
}
      
    
