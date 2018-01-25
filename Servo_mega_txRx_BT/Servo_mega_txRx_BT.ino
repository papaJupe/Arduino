/* Servo Mega tx rx BT -- Mega w/ BT/BTE? module, send / recv bytes
   over BT to prog like Proc on PC, can view Ardu's input w/ ser. mon.

  Proc app  --> sends control bytes via BT to Ardu for servo; first got
  a byte for up/down to increment tilt var; Proc. can use joystick button
  simul. key stroke, like sketch mega Servo Tx Rx BT; could try getting
  an int from a slider on Proc

  Uses: BT2 mod on ser2 read/write bytes, servo.write, elapsedMillis for
  timed print, later: convert incoming data to int angle for servo

  for comm w/ remote PC, mega's TX=16 (voltage divider-->3.3v), RX=17,
  to RX/TX on BT mod, send to Proc2 app using BT serial port on PC; must
  pair to device from PC (native BT ok for iMac)

  local Serial Mon. sees data I/O prn
*/

#include <Servo.h>
#include <elapsedMillis.h>  // to .print at interval

// Emulate Serial1 on pins 6/7 if hardware serial (like Mega) not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

// char packetBuffer[10];     // buffer to hold incoming packet, can't be small
// byte replyBuffer[] = "ACK";   // acknowl string to send back, char/byte?
// char replyBuffer[] = "30";  // must be byte array ?<- string, final null sent too
int ack = 30;

elapsedMillis timeElapsed; // declare global so not reset every loop
unsigned int interval = 2000;  // ms for local ser mon print prn

Servo tiltServ;
int posit = 90; // default neutral position, or motor stop

void setup()
{
  // on mega, servo pwr must come from VR not D pins, unclear why
  tiltServ.attach(2);  // control for servo pin D2

//  pinMode(40, OUTPUT);
//  digitalWrite(40, LOW);  // ground for level shift resistor

  Serial.begin(9600);    // speed for Ser. Mon. if on
  Serial2.begin(9600); // speed = BT module UART rate
  tiltServ.write(posit);  // init to neutral angle

}  // end setup

void loop()
{
  // loop runs 10 hz, gets control bytes from BT mod
// ? need faster than incoming data or need to clear buffer
  // if there's BT data available, read & ack
  if (Serial2.available())
  { // print for debug prn
    // read the chars, if >1 sent all get used in loop repeats
    char ch = Serial2.read(); // cmd for servo & clears buffer
    Serial.print(ch);  // chars from remote app

    // parse pkt and change servo setting within tilt limits
    // math looks reversed because 35 = full up, 150 full down
    // seems like 'key' bytes still work as second byte of string
    // but only if draw loop is not sending # continuously
    if (ch == 'u' && posit >= 40) posit -= 5;
    else if (ch == 'd' && posit <= 145) posit += 5;
//    else   // parse string input to int, to send to servo
//    { // read serial w/ readBytes then ? use parseInt()
//      int posiPoss = atoi(packetBuffer);  // conv chars to int
//      if ((posiPoss - 40) >= 1 && (posiPoss - 40) <= 109)
//        posit = posiPoss;  // in 41-149 range, use the new value
//    }
      // send ack to Proc
      // Serial2.print(++ack); counts # of acks; this sends angle
      Serial2.print("angle: "); Serial2.println(posit);
      
  }  // end if Ser2's respond to data sent

  tiltServ.write(posit); // write current # to servo
  
  // print to ser. mon. at sane interval prn
  if (timeElapsed > interval)
    {
      Serial.print("angle: ");
      Serial.println(posit);
      timeElapsed = 0;
    }

  delay(100);
}  // end loop

