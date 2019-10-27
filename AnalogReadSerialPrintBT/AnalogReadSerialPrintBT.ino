/*
  AnalogReadSerialPrintBT
  Reads an analog input on pin A_, prints the result to the serial monitor and
  second serial for transmit over BT (Simblee et al) or other.
  Attach the center pin of a pot to pin A5, and pwr pins to +5V & gnd.

  Uses: Software Serial, cast of int to String 

  Can test serial#2 I/O by connecting rx and tx of USB-TTL dongle to
  second serial pins and connecting Ser Mon to that port
*/
#include "SoftwareSerial.h"
SoftwareSerial sofser(2, 3); // RX, TX

//  setup runs once on pwr up, press reset, or open Ser Mon

void setup()
{ pinMode(A3, OUTPUT);
  digitalWrite(A3, LOW);
  pinMode(A4, OUTPUT);
  digitalWrite(A4, HIGH);
  pinMode(A5, INPUT);  // read pot voltage

  // initialize serial communication at 9600 bps
  Serial.begin(9600);
  sofser.begin(9600);  // 3.3 v output to (most) BT module, sent to app
}

void loop()
{
  // read the V on analog pin:
  int sensorValue = analogRead(A5);
  // print value to Ser Mon
  Serial.println("pot V = " + String(sensorValue));
  sofser.print("pot V = " + String(sensorValue));
  delay(2000);
}


