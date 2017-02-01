/*
    RotaryEncoderMultiPoll
 This sketch has two encoders connected.
 One is connected to pins 2 and 3
 The other is connected to pins 4 and 5
 */

const int  ENCODERS = 2; // the number of encoders

const int encoderPinA[ENCODERS] = {2,4};     // encoderA pins on 2 and 4
const int encoderPinB[ENCODERS] = {3,5};     // encoderB pins on 3 and 5
int encoderPos[ ENCODERS] = { 0,0};          // initialize the positions to 0
boolean encoderALast[ENCODERS] = { LOW,LOW}; // holds last state of encoderA pin


void setup()
{
  for (int i=2; i<6; i++){
    pinMode(i, HIGH);
    digitalWrite(i, HIGH);
  }
  Serial.begin (9600);
}

int updatePosition( int encoderIndex)
{
  boolean encoderA = digitalRead(encoderPinA[encoderIndex]);
  if ((encoderALast[encoderIndex] == HIGH) && (encoderA == LOW))
  {
    if (digitalRead(encoderPinB[encoderIndex]) == LOW)
    {
      encoderPos[encoderIndex]--;
    }
    else
    {
      encoderPos[encoderIndex]++;
    }
    Serial.print("Encoder ");
    Serial.print(encoderIndex,DEC);
    Serial.print("=");
    Serial.print (encoderPos[encoderIndex]);
    Serial.println ("/");
  }
  encoderALast[encoderIndex] = encoderA;
}


void loop()
{
  for(int i=0; i < ENCODERS;i++)
  {
    updatePosition(i);
  }
}
      
    
