/*
 Read a rotary encoder
 This simple version polls the encoder pins
 The position is displayed on the Serial Monitor
 */


const int encoderPinA = 4;
const int encoderPinB = 2;
const int encoderStepsPerRevolution=16;
int angle = 0;

int val;

int encoderPos = 0;
boolean encoderALast = LOW;  // remembers the previous pin state


void setup()
{
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  digitalWrite(encoderPinA, HIGH);
 digitalWrite(encoderPinB, HIGH);
  Serial.begin (9600);
}

void loop()
{
  boolean encoderA = digitalRead(encoderPinA);

if ((encoderALast == HIGH) && (encoderA == LOW))
  {
    if (digitalRead(encoderPinB) == LOW)
    {
      encoderPos--;
    }
    else
    {
      encoderPos++;
    }
    angle=(encoderPos % encoderStepsPerRevolution)*360/encoderStepsPerRevolution;
    Serial.print (encoderPos);
    Serial.print (" ");
    Serial.println (angle);
  }

  encoderALast = encoderA;
}
      
    
