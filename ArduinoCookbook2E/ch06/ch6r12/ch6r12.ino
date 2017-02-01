/*
  RotaryEncoderInterrupt sketch
 */

const int encoderPinA = 2;
const int encoderPinB = 4;
int Pos, oldPos;
volatile int encoderPos = 0; // variables changed within interrupts are volatile


void setup()
{
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  digitalWrite(encoderPinA, HIGH);
  digitalWrite(encoderPinB, HIGH);
  Serial.begin(9600);

  attachInterrupt(0, doEncoder, FALLING); // encoder pin on interrupt 0 (pin 2)
}

void loop()
{
  uint8_t oldSREG = SREG;

  cli();
  Pos = encoderPos;
  SREG = oldSREG;
  if(Pos != oldPos)
  {
    Serial.println(Pos,DEC);
    oldPos = Pos;
  }
   delay(1000);
}


void doEncoder()
{
  if (digitalRead(encoderPinA) == digitalRead(encoderPinB))
    encoderPos++;    // count up if both encoder pins are the same
  else
    encoderPos--;    //count down if pins are different
}
      
    
