/*
 * Morse sketch
 *
 * Direct port I/O used to send AM radio carrier at 1MHz
 */

const int sendPin = 2;

const byte WPM = 12;                    // sending speed in words per minute
const long repeatCount = 1200000 / WPM; // count determines dot/dash duration
const byte dot = 1;
const byte dash = 3;
const byte gap = 3;
const byte wordGap = 7;

byte letter = 0; // the letter to send

char *arduino = ".- .-. -.. ..- .. -. ---";


void setup()
{
  pinMode(sendPin, OUTPUT);
  Serial.begin(9600);
}


void loop()
{
  sendMorse(arduino);
  delay(2000);
}

void sendMorse(char * string)
{
  letter = 0 ;
  while(string[letter]!= 0)
  {
    if(string[letter] == '.')
    {
      sendDot();
    }
    else if(string[letter] == '-')
    {
      sendDash();
    }
    else if(string[letter] == ' ')
    {
      sendGap();
    }
    else if(string[letter] == 0)
    {
      sendWordGap();
    }
    letter = letter+1;
  }
}

void  sendDot()
{
  transmitCarrier( dot *  repeatCount);
  sendGap();
}

void  sendDash()
{
  transmitCarrier( dash *  repeatCount);
  sendGap();
}

void  sendGap()
{
  transmitNoCarrier( gap *  repeatCount);
}

void  sendWordGap()
{
  transmitNoCarrier( wordGap *  repeatCount);
}

void transmitCarrier(long count)
{
  while(count--)
  {
    bitSet(PORTD, sendPin);
    bitSet(PORTD, sendPin);
    bitSet(PORTD, sendPin);
    bitSet(PORTD, sendPin);
    bitClear(PORTD, sendPin);
  }
}


void transmitNoCarrier(long count)
{
  while(count--)
  {
    bitClear(PORTD, sendPin);
    bitClear(PORTD, sendPin);
    bitClear(PORTD, sendPin);
    bitClear(PORTD, sendPin);
    bitClear(PORTD, sendPin);
  }
}
      
    
