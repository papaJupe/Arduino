/* 2waySerial w/ mega -- to program BT mod w/ AT cmd from Ardu IDE, or
  soft ser. or Ser2 can be wired to USB-TTL or BT dongle for I/O w/
  Proc to test serial protocol: key or var in Proc sent variously, see
  how displayed in Ardu ser. mon. or used in sketch; BLE module
  on Mega can I/O to phone app; later mod: ServoMega TxRx Serial / BT

  Setup for MEGA: gnd for level shift to pin 13+1; Serial2 pins:
  TX=16, RX=17; or Ser Soft on Uno, to Rx/Tx of dongle, BT/ BLE mod ?
  HM10 (BLE) on USB dongle on PC recv only from remote H3 on Mega -- 1 way
  Works with H3 on Mega Ser2 comm with iPhone app 'HM10 BT Serial'

  Uses: serial.write, read, print, TimedAction lib for repeats w/o
  blocking, Serial.flush, readBytes
*/

#include <TimedAction.h>
// must define fx() before making object w/ it
//void seri2Write() // this sends a string periodically to Serial2
//{
//  Serial2.println("hurry up please!");
//}
//TimedAction timeActi = TimedAction(4000, seri2Write);


// Emulate Serial1 on pins 6/7 if no hardware serial (like Mega)
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

//SoftwareSerial SofSer(6, 7); // RX, TX

void setup()
{
  Serial.begin(57600);
  Serial2.begin(57600);
}

void loop() {

  char keyByt[8] = {};  // empty array for sending <=8 chars
  static byte charcnt = 0;

 // timeActi.check(); // sends string @interval, w/o blocking

  while (Serial2.available()) // ch by ch print the incoming bytes
  {
//    char ch = Serial2.read();
//    if (ch > 47) Serial.print(ch);  // print chars as usual
//    // if (ch == 13 or ch == 10) Serial.print(32);
//    else Serial.print(ch); // int or byte cast shows ascii #

    //Serial.print((char)Serial2.read()); // chars OK, # not
    // int or no cast .prints char's ascii val, or int's 0-N or
    //  255-1 if neg
    Serial.print(Serial2.read());
    Serial.print(" ");
    // .write prints char for char, nonsense for int
    //Serial.write(Serial2.read());

    charcnt++;  // increment each char read
  }

  if (charcnt > 16)  // keeps lines from overrunning window
  {
    Serial.println();
    charcnt = 0;
  }

  if (Serial.available()) // print/write ser.mon. input (ch by ch
    //   or full buffer) (v.i.) to outgoing port
  {
    //Serial2.print((char)Serial.read());

    // reads 1st 8 input bytes from Ser buffer: array holds 8 ch
    Serial.readBytes(keyByt, 8);
    Serial.print(".write keyByt ");
    Serial.write(keyByt);  // input bytes print as chars
    // flush remaining bytes in Ser buffer
    while (Serial.read() > 0)
      Serial.flush();  // may not need
    Serial.println();
    Serial2.write(keyByt); // need for HC06, no \n or \cr in SerMon
    // H3 needs line ends sent to it, maybe not
    // send to Proc or phone app over BT or UART dongle
    //Serial2.println(keyByt);
    
  }

  delay(100);
}   // end loop

