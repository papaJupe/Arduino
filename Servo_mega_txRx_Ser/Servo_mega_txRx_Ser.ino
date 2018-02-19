/* Servo Mega tx rx Serial -- send / recv bytes over USB-TTL serial
    to prog like Proc on PC, can view Ardu's I/O w/ ser. mon.

  Proc app  --> sends control bytes via dongle on Ser2 of Mega for
  servo; first got a byte for up/down to increment tilt var;
  Proc. w/ GCP lib can use joystick button & axes to send ch or #
  to Ardu sketch mega Servo Tx Rx Ser or BT; GC gui lib can also
  sent  an int from a slider on Proc; Ardu converts chars and #'s to
  angle change for servos. code started for tilt & pan, vals read OK

  Uses: USB-TTL dongle on ser2, read/write bytes, servo.write,
  elapsedMillis for timed print, convert incoming data to change angle
  for servo

  for comm w/ remote PC, mega's TX=16 (voltage divider-->3.3v), RX=17,
  to RX/TX on dongle, send to Proc2 app using dongle's serial port on PC

  local Serial Mon. sees data I/O prn
*/

#include <Servo.h>
#include <elapsedMillis.h>  // to .print at interval

// Emulate Serial1 on pins 6/7 if no hardware serial (like Mega)
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

// int ack = 30; //now sending angle instead

elapsedMillis timeElapsed; // declare global so not reset every loop
unsigned int interval = 2000;  // ms for local ser mon print prn

Servo tiltServ;
Servo panServ;

int positT = 90; // tilt's default neutral position, or motor stop
int positP = 90;

void setup()
{
  // on mega, servo pwr must come from VR not D pins, unclear why
  tiltServ.attach(2);  // control servo from pin D2

  Serial.begin(9600);    // speed for Ser. Mon. if on
  Serial2.begin(9600); // speed = mega to UART rate, same as Proc's
  tiltServ.write(positT);  // init tilt to neutral angle

}  // end setup

void loop()
{
  // loop runs 10 hz, gets control bytes from Proc via Serial2
  // ? need faster than incoming data or need to clear buffer
  // if there's data available, read & ack
  while (Serial2.available())
  {
    // read the chars, if >1 sent all get used in while repeats
    char ch = Serial2.read(); // cmd for servo & clears buffer
    Serial.print(ch);  // chars from remote app
    int deltaT, deltaP;
    if (ch < 13) // low values read as ints for tilt
    {
      deltaT = (int)ch - 5;
      positT += deltaT;
    }
    else if (ch < 33) // mid values read as ints for pan
    {
      deltaP = (int)ch - 25;
      positP += deltaP;
    }

    // parse pkt and change servo setting within tilt limits
    // math looks reversed because 35 = full up, 150 full down
    // seems like 'key' bytes still work as second byte of string
    // but only if draw loop is not sending # continuously
    if (ch == 'u') positT -= 5;
    if (ch == 'd') positT += 5;
    if (ch == 'l') positP -= 5;
    if (ch == 'r') positP += 5;

    positT = constrain(positT, 35, 150);
    positP = constrain(positP, 30, 150);

    // send ack / angles to Proc
    // Serial2.print(++ack); counts # of acks; this sends angle
    Serial2.print("angle: "); Serial2.print(positT);
    Serial2.print(" / " ); Serial2.println(positP);

  }  // end if Ser2's respond to data sent

  tiltServ.write(positT); // write current # to servo

  // print to ser. mon. at sane interval prn
  if (timeElapsed > interval)
  {
    Serial.print("angle: ");
    Serial.println(positT);
    timeElapsed = 0;
  }

  delay(100);
}  // end loop

