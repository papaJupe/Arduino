/*
  SFM Ser Volt read ==>  Ardu reads voltage on some pin, sends string
  message about voltage over serial (softwareSerial 2nd port for Uno)
  every 2 seconds to the Simb bd rx pin, this SFM code sends text to app;
  can use Ardu test sketch: AnalogReadSerialPrintBT, or just connect
  USB-TTL dongle to Simb bd's rx/tx and run Ser Mon to that port
  
  Uses: updateText, drawText, memset to clear array, readBytes to
  fill array from serial input,

*/

#include <SimbleeForMobile.h>

char data[20] = {"I'm here !"};  // char array holds data from Ardu

uint8_t textShow;  // field on app to show V data as text

void setup()
{
  // this is data for advertisement;
  // if the deviceName + advertisementData are too long to fit 15 char
  // ble advertisement packet, first the advertisementData is truncated to
  // (whatever fits, or) a single byte prn, then the deviceName is truncated)
  SimbleeForMobile.deviceName = "RF-Simblee";
  SimbleeForMobile.advertisementData = "showV";

  //  // use a subdomain to create an application specific cache
  //  SimbleeForMobile.domain = "temp.Simblee.com";
  //
  //  // establish a baseline to use the cache during development to bypass
  //  //  uploading the image each time
  //  SimbleeForMobile.baseline = "180225";

  Serial.begin(9600);

  // start SimbleeForMobile
  SimbleeForMobile.begin();
}  // end setup

unsigned long lastUpdate = 0;
int updateRate = 500; // send update to app Q 1/2 sec

// include newlib printf float support (%f used in sprintf below)
// asm(".global _printf_float");

void loop()
{
  unsigned long loopTime = millis();
  
  //  static float prev_temp = Simblee_temperature(CELSIUS);
  //  // sample once per second on low power
  //  // todo: Simblee_ULPDelay( SECONDS(1) );

  if (SimbleeForMobile.updatable && updateRate < (loopTime - lastUpdate))
  { 
    lastUpdate = loopTime;
    // SimbleeForMobile.updateText(textShow, ""); // clear old

    //    // requires newlib printf float support
    //    char buf[8];  // ? need to convert float to chars?
    //    sprintf(buf, "%.02f", temp);

    // update the text field with current data
    SimbleeForMobile.updateText(textShow, data);

  }  // end updating app text


  // capture incoming serial data from Ardu
  if (Serial.available())
  { 
    memset(data, 0, 20);  // wipe it

    Serial.readBytes(data, 20);

    while (Serial.read() != -1);  // clear buffer
  } // end if serial avail

  // .process must be called in the loop
  SimbleeForMobile.process();

}  // end loop

//void SimbleeForMobile_onConnect()
//{
//  first_sample = true;
//}

void ui()
{
  SimbleeForMobile.beginScreen(WHITE);

  textShow = SimbleeForMobile.drawText(60, 240, "", BLUE, 24);

  SimbleeForMobile.endScreen();
}  // end ui

void ui_event(event_t &event)
{ // no app events captured
}
