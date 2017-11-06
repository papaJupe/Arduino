/*
 * References: notes came with TardisTime -- Ardu IDE code for primary ESP board to
 get time from GPS dongle, send time by UDP to local PC over its wifi connection -- e.g. Proc2-3 running on PC, also display on screen on the ESP
 * https://github.com/esp8266/Arduino/blob/esp8266/hardware/esp8266com/esp8266/libraries/DNSServer/examples/CaptivePortal/CaptivePortal.ino
 * Similar: http://www.esp8266.com/viewtopic.php?f=32&t=3618&p=23900#p23900
 */

 /*
  * Notes:
  * This Sketch is for Arduino 1.6.6 daily build
  * The DNSServer.cpp and DNSServer.h tabs are bound to the sketch
  * As the Arduino ESP8266 repository as of this time has not been
  * Patched for the DNS bug
  * 
  * sketch + Utility libs must be in a single Sketch folder
  */

  /**************UTILITY TABS*****************************
  This is a library of utility functions
  Designed specifically to work with ESP8266 in the
  Arduino IDE platform.
  Written by Dave St. Aubin.
  Creative Commons license.
 *******************************************************/

