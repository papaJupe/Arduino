void setup() {
  Serial.begin(57600);
  delay(1000);
  Serial.println("Labels,A0,A1,A2,A3,A4,A5");
}

void loop() {
  Serial.print("Data,");
  for(int i=0; i < 6; i++)
  {
    Serial.print( analogRead(i) );
    Serial.print(",");
  }
  Serial.print('\n'); // newline character
  delay(100);
}