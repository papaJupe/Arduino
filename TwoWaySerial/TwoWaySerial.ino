/* 2waySerial w/ mega -- to program BT mod w/ AT cmd from Ardu IDE
  soft ser. or Ser2 can be wired to USB-TTL or BT dongle for I/O w/ 
  Proc to test serial protocol: key or var in Proc sent variously, see how
  displayed in Ardu ser. mon. or used in sketch; probably BLE module
  could I/O to phone app ?; later mod: ServoMega TxRx Serial / BT

  Setup for MEGA: gnd for level shift to pin 13+1; Serial2 pins:
  TX=16, RX=17; or Ser Soft on Uno, to Rx/Tx of dongle, BT/ BLE mod ?
   
*/

// Emulate Serial1 on pins 6/7 if no hardware serial (like Mega)
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

//SoftwareSerial SofSer(6, 7); // RX, TX

void setup()
{
  Serial.begin(9600);
  Serial2.begin(9600);
}

void loop() {
  // sketch goes crazy if you enter more chars than array holds:
  char inByt[8] = {};  // empty array for sending <=7 chars
  static byte charcnt = 0;
  while (Serial2.available()) // ch by ch print the incoming bytes
  {
    char ch = Serial2.read();
    if (ch > 47) Serial.print(ch);  // print chars as usual
    else Serial.print((byte)ch); // int or byte cast shows a #
    
    //Serial.print((char)Serial2.read()); // chars OK, # not
    // int or no cast .prints char's ascii val, or int's 0-N or
    //  255-1 if neg
    //Serial.print(Serial2.read());
    // .write prints char for char, nonsense for int
    //Serial.write(Serial2.read());

    charcnt++;  // increment each char read
  }
  if (charcnt > 20)  // keeps lines from overrunning window
  {
    Serial.println();
    charcnt = 0;
  }
  
  if (Serial.available()) // print/write ser.mon. input ch by ch
    //  to outgoing port
  {
    //Serial2.print((char)Serial.read());
    
    // ALL input bytes --> inByt char array
    Serial.readBytes(inByt, Serial.available());
    Serial.print(".write inByt ");
    Serial.write(inByt);  // input bytes print as chars
    Serial.println();
    Serial2.println(inByt); // send to Proc over BT or UART dongle
  }

  delay(20);
}   // end loop
