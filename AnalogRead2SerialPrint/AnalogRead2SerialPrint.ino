/*
  AnalogRead2Serial -- for nano with extra A pins
  Reads an analog input on pins, prints the result to the serial monitor.
  Attach the center pin of 2 pots to pins, and the outside pins to +5V and ground
  sent by adjacent A pins here
*/

// the setup routine runs once when you press reset

void setup()
{ pinMode(A2, OUTPUT);
  digitalWrite(A2, HIGH);  // ground for pot1
  pinMode(A4, OUTPUT);
  digitalWrite(A4, LOW);  // 5v for pot1
  pinMode(A3, INPUT);  // read v on this pin

//  pinMode(A4, OUTPUT);
//  digitalWrite(A4, LOW);  // ground for pot2
//  pinMode(A5, OUTPUT);
//  digitalWrite(A5, HIGH);  // 5v for pot2
//  pinMode(A6, INPUT);  // read v on this pin

  // initialize serial communication at 9600 bits per second
  Serial.begin(9600);
}

// the loop runs over and over

void loop()
{
  // read the input on analog pin 2 and 6: raw AR values
  int volt1 = analogRead(A3);
 // int volt2 = analogRead(A6);
  // print out
  Serial.print("pot1 is "); Serial.println(volt1);
  //Serial.print("pot2 is "); Serial.println(volt2);
  delay(2000);        // delay between reads for sanity
}
