/*
ReadAnalogCalMap --  (moving) averages analog pin input, 0-5 v. (as from batt
 discharger device with an op-amp reading V drop across load resistor); 
maps that to the actual v. 11.5-13 being sampled; prints raw and calcul. voltage
 if it's changed; use to get params for map function in other sketches
 */

#define PinIn A5

float newAvg;
float prevAvg;
//int sensorV;local to loop
//int mapAvg;

  // setup runs once when you power on or Reset
void setup() 
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // prevAvg = analogRead(PinIn);  // better below
}

//  loop runs over and over
void loop() 
{
       // read the input on analog pin 
  float sensorV = analogRead(PinIn);
  static float prevAvg = sensorV;  // just some starting place

  float newAvg = (sensorV*0.2 + prevAvg*0.8);  //update expon moving average of AR; using float and mult improves accuracy of this calc.
        // convert analog reading (0 - 1023) to a voltage (11.5-13V) using map
  int mapAvg = round(newAvg);   // nearest int good for mapping, could use float?
  // int voltage = map(mapAvg,479,540,11500,13000);  // in mV, w/ volt divid, resolves ~25mV
   int voltage = map(mapAvg,123,966,11500,13000);  // in mV, resolves ~2 mV
  
       // print out the new values if changed
  if (mapAvg != round(prevAvg))
    {
      Serial.print("pin out= "); Serial.print(mapAvg); 
      Serial.print("  volt calc= "); Serial.println(voltage);
      prevAvg = newAvg;
    }
  delay (200);   // five loops / second
}
