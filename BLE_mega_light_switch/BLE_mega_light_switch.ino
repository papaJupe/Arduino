/* BLE mega light switch uses BLE serial dongle
       on Ardu mega for wireless switch of light
  HC-08 (lite HM10?) fails w/fatBeard app, OK w/ HM10 Serial app
  HM-10 fine with both as long as name HMSoft, pass 000000
  
  Uses: Ser2 + BLE HM 10, hc08, for comm w/ iPhone, TX=16, RX=17 on Mega

  Setup for MEGA:
   level shifter gnd to pin 13+1

  Serial2 to BLE module, pins: TX=16, RX=17; or Ser Soft on Uno
*/


// Emulate Serial1 on pins 6/7 if hardware serial (like Mega) not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

#define LED_PIN 13
int c = 0;
  
//SoftwareSerial Serial2(6, 7); // RX, TX

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  pinMode(LED_PIN, OUTPUT); // w/o this each loop turns lite off again
}

void loop() {

  if (Serial2.available()) {
    c = Serial2.read();
    Serial.println("Got input:");
    if (c == 0)
    {
      Serial.println(" on");
      digitalWrite(LED_PIN, HIGH);

    }
    if (c == 1)
    {
      Serial.println(" off");
      digitalWrite(LED_PIN, LOW);

    }
    if (c == 2)
    {
      Serial.println(" pulse");
      digitalWrite(LED_PIN, HIGH);
      delay(500);
      digitalWrite(LED_PIN, LOW);
    }

  }
  // Serial.println(c);
  delay(500);
}
