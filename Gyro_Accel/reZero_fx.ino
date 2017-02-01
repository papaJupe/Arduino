
void reZero()  // resets global vars: currentAngle to 0, gyroZeroVolt to its resting val 
{
  gyroV = analogRead(gyroPin);
  newGavgV = round((gyroV + 4*prevGavgV)/5);  //updates expon moving average 
  gyroZeroVolt = ((float)newGavgV / 1020) * gyroVolt;  // resting gyro voltage, zero angle

  Serial.println('/n' + "reZeroed angle and gyro");
  Serial.print("rest volts = "); 
  Serial.println(gyroZeroVolt,3); // 3 digit precis.
  Serial.println();

  lcd.clear();  // clear screen and set cursor to (0,0)       
  lcd.print("zeroing");
  lcd.setCursor(0,1);   // second line
  lcd.print("GV "); 
  lcd.print(gyroZeroVolt,3);  // 3 digit precis.
  currentAngle = 0;
  delay (2000);   
}
void reBase()  // resets gyroZeroVolt to its resting val, keeps currentAngle
{
  gyroV = analogRead(gyroPin);
  newGavgV = round((gyroV + 4*prevGavgV)/5);  //updates expon moving average 
  gyroZeroVolt = ((float)newGavgV / 1020) * gyroVolt;  // resting gyro voltage, zero angle

  Serial.println('/n' + "reZeroed gyro base");
  Serial.print("rest volts = "); 
  Serial.println(gyroZeroVolt,3); // 3 digit precis.
  Serial.println();

  lcd.clear();  // clear screen and set cursor to (0,0)       
  lcd.print("zeroingV");
  lcd.setCursor(0,1);   // second line
  lcd.print("GV "); 
  lcd.print(gyroZeroVolt,3);  // 3 digit precis.
  delay (2000);   
}
