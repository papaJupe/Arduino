/*
ReadPoToPWM_B -- reads analog input on pin 0, converts it to voltage, proportional PWM output,
		and prints the result to the serial monitor when it changes. Very basic, could be improved with
		MA of voltage, rounding, faster loop.  Attach the center pin of a potentiometer to pin A0, and 
		the outside pins to +5V and ground.
		Uses: float, cast to byte, PWM output with analogWrite
 */

const int pwmPin=6;
float voltage;
byte pwmPower;
int wasSensor;      //put here so it doesn't reinitialize inside loop, or could use static loc var

void setup() 
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

void loop() 
{
  // read the input on analog pin 0:
  int  sensorValue = analogRead(A0);
  if (sensorValue != wasSensor)  //if new reading, otherwise leave as is 
  {
    // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
    float voltage = sensorValue * (5.0 / 1023.0);

    wasSensor=sensorValue; //store for next loop's if

            //convert sensor reading to a byte, 0 - 255, for PWM
    pwmPower=byte(sensorValue/4);       // variable declared above, forcing conversion here
            //  byte pwmPower=sensorValue/4;  //this works too, or could use map() as in A version

    analogWrite(pwmPin,pwmPower);  //send PWM power 0-255 to digital pin

    // print out the values you read: (in if block, so only print if changed)
    Serial.print("PWM signal is: ");
    Serial.println(pwmPower);
    Serial.print("voltage from pot is:");
    Serial.println(voltage);
    Serial.println();
  }
  delay(1000);
}

