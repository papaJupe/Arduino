/*
 * Stepper_Easystepper sketch
 *
 * stepper is controlled from the serial port.
 * a numeric value followed by '+' or '-' steps the motor
 *
 */

const int dirPin = 2;
const int stepPin = 3;

int speed = 100;    // desired speed in steps per second
int steps = 0;      // the number of steps to make

void setup()
{
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  if ( Serial.available()) {
    char ch = Serial.read();

    if(isDigit(ch)){             // is ch a number?
      steps = steps * 10 + ch - '0';        // yes, accumulate the value
    }
    else if(ch == '+'){
      step(steps);
      steps = 0;
    }
    else if(ch == '-'){
      step(-steps);
      steps = 0;
    }
    else if(ch == 's'){
      speed = steps;
      Serial.print("Setting speed to ");
      Serial.println(steps);
      steps = 0;
    }
  }
}

void step(int steps)
{
  int stepDelay = 1000 / speed;  //delay in ms for speed given as steps per sec
  int stepsLeft;

  // determine direction based on whether steps_to_mode is + or -
  if (steps > 0)
  {
    digitalWrite(dirPin, HIGH);
    stepsLeft = steps;
  }
  if (steps < 0)
  {
    digitalWrite(dirPin, LOW);
    stepsLeft = -steps;
  }
  // decrement the number of steps, moving one step each time
  while(stepsLeft > 0)
  {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(1);
    digitalWrite(stepPin,LOW);
    delay(stepDelay);
    stepsLeft--;      // decrement the steps left
  }
}
      
    
