 /*
ReadAnalCurrMap also copy in voltSampB2 folder, will it open with it? 
		reads analog input on pin X, and prints the result
		to the serial monitor if it has changed; uses mapping values to map pin AR to
		actual current (voltage across 2 ohm) going to op-ampA.
		Uses: analRead, expon moving averag, map, round
 */
int newAvg;
int prevAvg;          // globals, appear in setup and loop
int indxAvg;

#define PinIn 5

  // setup runs once when you power on or press reset button
void setup() 
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  prevAvg = analogRead(PinIn);  // just some starting place
  indxAvg = prevAvg;          // init this value for MA calc in loop
}

void loop() 
{
       // read the input on analog pin, make moving average to stabilize 
  int pinSees = analogRead(PinIn);
  newAvg = round((pinSees + 4*prevAvg)/5);  //update expon moving average of AR
        // convert analog reading (0 - 1023) to current (via amplified voltage (0-1.9 across Rc) 
  int current = map(newAvg,114,975,230,900);  // map AR ticks to current (mA) for 2 ohm Rc
  prevAvg = newAvg;
  
       // print out the new MA if changed from index value, set new index
  if (newAvg != indxAvg)
    {
      Serial.print("pin out= "); Serial.print(newAvg); Serial.print("  curr calc= "); Serial.println(current);
      indxAvg = newAvg;
    }
  delay (200);   // five loops / second
}