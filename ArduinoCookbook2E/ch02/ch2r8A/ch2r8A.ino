/*
 * NumberToString
 * Creates a string from a given number
 */

void setup()
{
  Serial.begin(9600);
}

char buffer[12];  // long data type has 11 characters (including the
                  // minus sign) and a terminating null

void loop()
{
long value = 12345;
  ltoa(value, buffer, 10);
  Serial.print( value);
  Serial.print(" has  ");
  Serial.print(strlen(buffer));
  Serial.println(" digits");
  value = 123456789;
  ltoa(value, buffer, 10);
  Serial.print( value);
  Serial.print(" has  ");
  Serial.print(strlen(buffer));
  Serial.println(" digits");
  delay(1000);
}
      
    
