const int  batteryPin = 0;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  long val = analogRead(batteryPin);   // read the value from the sensor -
                                       // note val is a long int
  Serial.println( (val * (500000/1023)) / 100);  // print the value 
                                                 // in millivolts
}
      
    
