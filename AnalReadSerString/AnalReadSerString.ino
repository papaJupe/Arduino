/*
 AnalogReadSerialString -- first mod shows use of String var. for better printing, has no 
 float capacity;  Reads an analog input on pin A_, prints the result to the serial monitor.
 Attach the center pin of a potentiometer to pin A_, and the outside pins to +5V and ground.
 Uses: exit, String constructors, analogRead, String concatenation
 */

//  setup runs once when you load code, start Ser Mon, or press reset:
void setup() 
{
  // initialize serial communication at 9600 bits per second
  Serial.begin(9600);
}

// loop normally repeats, but not this one

void loop() 
{
  // read the input on some analog pin:
  int sensorValue = analogRead(A3); // done for clarity, could put aR(_) into String(int)

  // convert the int to a String object var, not same as char string (a plain C array)
  String myValue = String(sensorValue,DEC);  // optional DEC formatter for an int
  // float not acceptable arg for String constructor 
  // Serial.println("raw pin AR is " + sensorValue + " ticks"); // fails to compile w/ int
  // Serial.println("raw pin AR is " + myValue + " ticks"); // prints OK since myVal is String var
  // Serial.println("raw pin AR is " + String(sensorValue) + " ticks"); // works OK w/ conversion
  //  this works but is unnecessary
  Serial.println (String ("raw pin AR is " + myValue + " ticks"));  // concat of Str's OK
  //Serial.println (String ("raw pin AR is " + sensorValue + " ticks")); // w/ int fails to compile

  delay(500);        // delay between prints for sanity
  exit(1); // exit(int) undocumented C fx, stops loop, sends int param to OS
}


