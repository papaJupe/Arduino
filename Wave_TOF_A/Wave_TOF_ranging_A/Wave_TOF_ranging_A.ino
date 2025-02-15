
/*
  TOF ranging test adapted from Wave's demo sketch

  Receives from serial port 1, print to the main serial (Serial 0).

  This example works with boards with more than one serial like Arduino Mega, 
  Due, Zero etc.  Also with softwareSerial for Uno et al, as below

  The circuit:
  - TOF Laser Range Sensor attached to Serial port 1
  - Serial Monitor open using Serial port 0

  created 27 July 2020
  by kaloha [Waveshare] https://www.waveshare.com/

  This example code is in the public domain.
*/
#include <SoftwareSerial.h>
#include <elapsedMillis.h>  // to .print at interval
#include <LiquidCrystal.h>

unsigned char TOF_data[32] = { 0 };  //store 2 TOF frames
unsigned char TOF_length = 16;
unsigned char TOF_header[3]{ 0x57, 0x00, 0xFF };
unsigned long TOF_system_time = 0;
unsigned long TOF_distance = 0;
unsigned char TOF_status = 0;
unsigned int TOF_signal = 0;
unsigned char TOF_check = 0;

SoftwareSerial Serial1(2, 3);  //ardu RX=2,TX=3, Serial1 connect to TOF's Serial：3<-->RX, 2<-->TX

elapsedMillis timeElapsed;     // declare global so not reset every loop
unsigned int interval = 1000;  // ms for local ser mon print

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);  // pin # for 16x2 shield


void setup() {
  // initialize both serial ports:
  Serial.begin(115200);
  Serial1.begin(115200);
  lcd.begin(16, 2);  // 16 col, 2 row LCD

}  // end setup

bool verifyCheckSum(unsigned char data[], unsigned char len) {
  TOF_check = 0;
  // this function will print data OK
  for (int k = 0; k < len - 1; k++) {
    TOF_check += data[k];  // sum of array values
  }

  if (TOF_check == data[len - 1])  // final frame value is checksum
  {
    Serial.println("TOF data OK!");
    Serial.println("");
    lcd.clear();
    lcd.setCursor(0, 1);  // start on second line
    lcd.print("TOF data OK!     ");
    return true;
  } else {
    Serial.println("data chksum error!");
    Serial.println("");

    lcd.setCursor(0, 1);  // start on second line
    lcd.print("checkSum error");
    return false;
  }  // end else
}  // end verifyCheckSum

void loop() {
  lcd.clear();  // lcd rewrite every loop (20 hz)
  // led.setCursor(thisCol, thisRow)
  lcd.setCursor(0, 0);
  lcd.print("range mm: ");

  // read from serial1 port, fill array w/ 32 char from 2 frame:
  if (Serial1.available() >= 32) {
    for (int i = 0; i < 32; i++) {
      TOF_data[i] = Serial1.read();
    }  // end array fill

    for (int j = 0; j < 16; j++) {  // first 3 bytes match known header and checkSum verifies 16 byte frame
      if ((TOF_data[j] == TOF_header[0] && TOF_data[j + 1] == TOF_header[1] && TOF_data[j + 2] == TOF_header[2])
          && (verifyCheckSum(&TOF_data[j], TOF_length))) {
        // status byte of first frame should != 0
        if (((TOF_data[j + 12]) | (TOF_data[j + 13] << 8)) == 0) {
          Serial.println("Out of range!");
          Serial.println("");
          lcd.setCursor(2, 0);  //col 10, row 0
          lcd.print("Out of Range!   ");
          break;

        } else {
          // Serial.print("TOF id is: ");
          // Serial.println(TOF_data[j + 3], DEC);

          // TOF_system_time = TOF_data[j + 4] | TOF_data[j + 5] << 8 | TOF_data[j + 6] << 16 | \
          // TOF_data[j + 7] << 24;
          // Serial.print("TOF system time is: ");
          // Serial.print(TOF_system_time, DEC);
          // Serial.println("ms");

          // not clear how this assignment works
          TOF_distance = (TOF_data[j + 8]) | (TOF_data[j + 9] << 8) | (TOF_data[j + 10] << 16);

          TOF_signal = TOF_data[j + 12] | TOF_data[j + 13] << 8;
          // print to ser. mon. at sane interval
          if (timeElapsed > interval) {
            Serial.print("distance mm: ");
            Serial.print(TOF_distance, DEC);
            Serial.println("");
            Serial.print("signal power: ");
            Serial.println(TOF_signal, DEC);
            Serial.println("");
            timeElapsed = 0;
          }  // end time elapsed print

          //lcd.setCursor(10,0);
          lcd.clear();
          lcd.print("range mm = ");
          lcd.print(TOF_distance, DEC);
          // lcd.print("   ");
          lcd.setCursor(0, 1);  // start on second line
          lcd.print("signal power: ");
          lcd.print(TOF_signal, DEC);

          // TOF_status = TOF_data[j + 11];
          // Serial.print("TOF status is: ");
          // Serial.println(TOF_status, DEC);

        }  // end else -- print good data

        // needed ? to break loop
      }  // end if header and frame sum OK

    }  // end for loop reading 2 frame content

  }  //  end Ser1 avail
  delay(200);
}  // end loop
