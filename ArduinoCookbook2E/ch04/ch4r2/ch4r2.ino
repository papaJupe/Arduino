/*
 * SerialFormatting, extensive mods from original ch4r2 in cookbook which had many errors
 * Print values in various formats to the serial monitor
 */
char chrValue = 65;    // these are the starting values to print, all the same number
byte byteValue = 65;   // but will print very differently
int intValue  = 65;
float floatValue = 65.0;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  while(Serial.available() != 1)  // stop loop with any serial input
  {
  Serial.print("chrValue: ");Serial.println(chrValue);
  Serial.print("chrValue with write: ");Serial.write(chrValue); Serial.write('\n');
  Serial.print("chrValue with DEC option ");Serial.println(chrValue,DEC);
  Serial.print("chrValue with BIN option ");Serial.println(chrValue,DEC);
  
  Serial.print("byteValue: ");Serial.println(byteValue);
  Serial.write("byteValue with write: ");Serial.write(byteValue);Serial.write('\n');
  Serial.print("byteValue with DEC option: "); Serial.println(byteValue,DEC);
  Serial.print("byteValue with BIN option: "); Serial.println(byteValue,BIN);
  
  Serial.print("intValue plain: ");Serial.println(intValue);
  Serial.print("intValue with DEC: ");Serial.println(intValue,DEC);
  Serial.print("intValue as HEX: ");Serial.println(intValue,HEX);
  Serial.print("intValue as OCT: ");Serial.println(intValue,OCT);
  Serial.print("intValue as BIN: ");Serial.println(intValue,BIN);

  Serial.print("floatValue: "); Serial.println(floatValue);
  Serial.write('\n');  // write blank line between sets
  
  delay(2000); // delay 2 second between numbers
  chrValue++;  // advance to the next value
  byteValue++;
  intValue++;
  floatValue +=1;
  }  // end while
}
