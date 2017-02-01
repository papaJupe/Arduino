/*
  AnalogReadSerial
  Reads an analog input on pin A4, prints the result to the serial monitor.
  Attach the center pin of a potentiometer to pin A4, and the outside pins to +5V and ground.
*/

// the setup routine runs once when you press reset

void setup()
{ pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  // initialize serial communication at 9600 bits per second
  Serial.begin(9600);
}

// the loop routine runs over and over

void loop()
{
  // read the input on analog pin 4:
  int sensorValue = analogRead(A7);
  // print out the value
  Serial.print("sensor V is "); Serial.println(sensorValue);
  delay(2000);        // delay between reads for sanity
}
