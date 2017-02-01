const byte mask = B11111000; // mask bits that are not prescale values
int prescale = 0;

void setup()
{
  Serial.begin(9600);
  analogWrite(3,128);
  analogWrite(5,128);
  analogWrite(6,128);
  analogWrite(9,128);
  analogWrite(10,128);
  analogWrite(11,128);
}

void loop()
{
  if ( Serial.available())
  {
    char ch = Serial.read();
    if(ch >= '0' && ch <= '9')              // is ch a number?
    {
      prescale = ch - '0';
    }
    else if(ch == 'a')  // timer 0;
    {
      TCCR0B = (TCCR0B & mask)  | prescale;
    }
    else if(ch == 'b')  // timer 1;
    {
      TCCR1B = (TCCR1B & mask)  | prescale;
    }
    else if(ch == 'c')  // timer 2;
    {
      TCCR2B = (TCCR2B & mask)  | prescale;
    }
  }
}
      
    
