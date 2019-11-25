/* BLE mega light switch uses BLE serial dongle on Ardu mega's second Ser
  input to receive control of LED over BT (BLE) from phone app
  -- no attempt here to send user input from PC+Ardu back to app
  
  HC-08 (lite HM10?) fails w/fatBeard app, OK w/ HM10 Serial app
  HM-10 fine with both as long as name HMSoft, pass 000000
  HM-12 dual module connects good w/ HM-10SerPro app for lite control
    w/o PIN

  HM-10 Serial Pro app can send char (UTF), raw # (dig) or bin
    -- need to tweak vars and .print/write so I/O formats right

  Uses: Ser2 + BLE HM 10, hc08, H3 for comm w/ iPhone, TX=16, RX=17 on Mega
    also OK Mega + HM-12; peripheral can send to HM10 on serial dongle on PC
    but Mega not recv anything on Ser2 -- ? if central not sending, ? why
    
  Setup for MEGA:
   level shifter gnd to pin 13+1; Serial2 to BLE module,
   pins: TX=16, RX=17; or Ser Soft on Uno
*/


// Emulate Serial on pins 6/7 if hardware serial (like Mega) not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial2(6, 7); // RX, TX
#endif

#define LED_PIN 13
int c = 0;

void setup() {
  Serial.begin(9600); // just prints status to PC Ser Mon, no input coded here
  Serial2.begin(9600);  // output to BT module, sent to app
  pinMode(LED_PIN, OUTPUT); // w/o this each loop turns lite off again
}

void loop() {

  if (Serial2.available()) {
    c = Serial2.read();
    Serial.println("Got input:");
    if (c == '1')  // char value works if input is ascii #
    {
      Serial.println(" on");
      digitalWrite(LED_PIN, HIGH);

    }
    if (c == '0')
    {
      Serial.println(" off");
      digitalWrite(LED_PIN, LOW);

    }
    if (c == '2')
    {
      Serial.println(" pulse");
      digitalWrite(LED_PIN, HIGH);
      delay(500);
      digitalWrite(LED_PIN, LOW);
      delay(500);
      digitalWrite(LED_PIN, HIGH);
      delay(500);
      digitalWrite(LED_PIN, LOW);
      delay(500);
      digitalWrite(LED_PIN, HIGH);
      delay(500);
      digitalWrite(LED_PIN, LOW);
      delay(500);
    }
    Serial2.write("got# "); Serial2.write(c); // write sends char back
    // for HM10SerPro this sends incoming # back as char
  }
   Serial.println(c);
   //Serial2.write("nogot ");
  delay(500);
}
