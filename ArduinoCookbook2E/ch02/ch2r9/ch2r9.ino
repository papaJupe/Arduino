/*
 * StringToNumber
 * Creates a number from a string
 */

const int ledPin = 13; // pin the LED is connected to

int   blinkDelay;     // blink rate determined by this variable
char strValue[6];     // must be big enough to hold all the digits and the
                      // 0 that terminates the string
int index = 0;        // the index into the array storing the received digits

void setup()
{
 Serial.begin(9600); 
 pinMode(ledPin,OUTPUT); // enable LED pin as output
}

void loop()
{
  if( Serial.available())
  {
    char ch = Serial.read();
    if(index < 5 && isDigit(ch) ){
      strValue[index++] = ch; // add the ASCII character to the string;
    }
    else
    {
      // here when buffer full or on the first non digit
      strValue[index] = 0;        // terminate the string with a 0
      blinkDelay = atoi(strValue);  // use atoi to convert the string to an int
      index = 0;
    }
  }
  blink();
}  

void blink()
{
   digitalWrite(ledPin, HIGH);
   delay(blinkDelay/2);  // wait for half the blink period 
   digitalWrite(ledPin, LOW);
   delay(blinkDelay/2);  // wait for the other half 
}