/*
  AnalogReadFltoString --  Reads an analog input on pin A_, prints the result to the 
  serial monitor; mod to show use of String object for better printing and take a float, 
  which normally it won't accept in constr.  String can use printf like Java / C (not used here)
  Attach the center pin of a potentiometer to pin A_, and the outside pins to +5V and ground.
  Uses: String object, cast from int to float, dtostrf to convert float to string then to String.
  alternate method using Streaming.h in  SampSynt and USsr04-avg-Flt
 */

//  setup runs once when you load, press reset or open Ser. Mon.:
void setup() 
{
  // initialize serial communication at 9600 bits per second
  Serial.begin(9600);
}

// the loop runs over and over 

void loop() 
{
  // read the input on analog pin as an int:
  //int sensorValue = analogRead(A0); // done for clarity, could put aR(4) into String constructor  
  // convert the int to a String object, not same as char string (a plain C array)
  //String myValue = String(sensorValue,DEC);// optional DEC formatter for an int; float not allowed
  
  // conversion needed to output a float
  float vFlt = 4.77 *((float)analogRead(4)/1023);  // need to cast from an int var to divide
  char vStr[5];// make target buffer array for dtostrf, one byte larger than width you want to print
          // dtostrf(sourceFLOAT,WIDTH,PRECISION,targetBUFFER);
  dtostrf (vFlt,4,3,vStr);  // convert the float to a C string
  String myStr = String(vStr);  // make a String object from it
  // print the value
  Serial.println(("pin AR is " + myStr + " in volts")); // concat works if all are Strings/literals
  // also works but unnecessary:
  // Serial.println (String ("vFlt is " + myStr + " volts")); // concat only works if all 
  // items are String objects, or string constants; won't convert int or flt

  delay(2000);        // delay between prints for sanity
}  // end loop

