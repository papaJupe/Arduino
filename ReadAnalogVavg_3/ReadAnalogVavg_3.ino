/*
ReadAnalogVaverage_3 -- averages (same) analog input to 3 pins X Y Z, and prints the result to 
		the serial monitor if it has changed.
		Used to calibrate and check op amp I/O params, compare accuracy of different AR pins
based on: ReadAnalogVaverage - but using (same) voltage to 3 pins; 
		and here we're finding the multiplier for the op-amp part of batt discharger
		
*/

int prevAvgX;  // global, used in setup and loop
int prevAvgY;
int prevAvgZ;

#define PinX 1   //sample 3 pins at a time
#define PinY 3  // this = A3
#define PinZ 5

void setup() 
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  int prevAvgX = analogRead(PinX);  //just some starting place
  int prevAvgY = analogRead(PinY);
  int prevAvgZ = analogRead(PinZ);
}

void loop() 
{
         // read the input on analog pins 0-1023
  int sensorVx = analogRead(PinX);
  int sensorVy = analogRead(PinY);
  int sensorVz = analogRead(PinZ);
  
  int newAvgX = round((sensorVx + 4*prevAvgX)/5);  //updates expon moving average of raw AR's
  int newAvgY = round((sensorVy + 4*prevAvgY)/5);
  int newAvgZ = round((sensorVz + 4*prevAvgZ)/5);
  
        // Convert the analog reading (which goes from 0 - 1023) to a calc voltage (11.xx - 13.x v.):
  int voltsX = (int)(newAvgX * 1.79 + 11280);  // in mV, resolves about 5 mV
  int voltsY = (int)(newAvgY * 1.79 + 11280);  // was measuring offset from 11.28 v ref from regulator
  int voltsZ = (int)(newAvgZ * 1.79 + 11280);  // 1.79 was test of op amp multiplier
  
    // print out the new MA if different on pin X
  if (newAvgX != prevAvgX)
    {
      Serial.print("pin out X= "); Serial.print(newAvgX); Serial.print("  volt calc= "); Serial.println(voltsX);
      Serial.print("pin out Y= "); Serial.print(newAvgY); Serial.print("  volt calc= "); Serial.println(voltsY);
      Serial.print("pin out Z= "); Serial.print(newAvgZ); Serial.print("  volt calc= "); Serial.println(voltsZ);

      prevAvgX = newAvgX;
      prevAvgY = newAvgY;
      prevAvgZ = newAvgZ;
    }
    delay (200);   // two loops / second
}

