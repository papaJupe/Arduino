//sketch 6-01 -- added rebuke for straying from binary

int outPin = 13;

void setup()
{
  pinMode(outPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Enter 1 or 0");
}

void loop()
{
  if (Serial.available() > 0)
  {
    char ch = Serial.read();
    if (ch == '1')
    {
      digitalWrite(outPin, HIGH);
    }
    else if (ch == '0')
    {
      digitalWrite(outPin, LOW);
    }
    else Serial.println("I said ENTER A ONE OR A ZERO!");
  }
}
