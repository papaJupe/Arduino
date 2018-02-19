/* BLE mega light switch uses BLE serial dongle
       on Ardu mega for wireless control of light
  HC-08 (lite HM10?) fails w/fatBeard app, OK w/ HM10 Serial app
  HM-10 mod fine with both as long as name HMSoft, pass 000000
  HM-12 dual module connects good w/ HM-10SerPro for lite control
    w/o needing PIN
  
  HM-10 Serial Pro app can send char (UTF), raw # (dig) or bin
    -- need to tweak vars and .print/write so I/O formats right
  
  Uses: Ser2 + BLE HM 10, hc08, for comm w/ iPhone, TX=16, RX=17 on Mega
    also Mega + HM-12
  Setup for MEGA:
   level shifter gnd to pin 13+1; Serial2 to BLE module, 
   pins: TX=16, RX=17; or Ser Soft on Uno
*/


// Emulate Serialn on pins 6/7 if hardware serial (like Mega) not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial2(6, 7); // RX, TX
#endif

#define LED_PIN 13
int c = 0;

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  pinMode(LED_PIN, OUTPUT); // w/o this each loop turns lite off again
}

void loop() {

  if (Serial2.available()) {
    c = Serial2.read();
    Serial.println("Got input:");
    if (c == '0')  // char value works if input is ascii #
    {
      Serial.println(" on");
      digitalWrite(LED_PIN, HIGH);

    }
    if (c == '1')
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
    }
  Serial2.write("gotsum ");Serial2.write(c); // write sends char back
     // for HM10SerPro this sends incoming # back as char 
  }
  // Serial.println(c);
  delay(500);
}
