/*
  WiFiEsp example: UdpNTPClient
  mod by AM to use 2 serial channels of MEGA and display to LCD
  
  Get the time from a Network Time Protocol (NTP) server.
  Demonstrates use of UDP to send and receive data packets
  For more on NTP time servers and the messages needed to communicate,
  see http://en.wikipedia.org/wiki/Network_Time_Protocol

  NOTE: The serial buffer size must be larger than 36 + packet size
  In this example we use an UDP packet of 48 bytes so the buffer must be
  at least 36+48=84 bytes & that exceeds Ardu default buffer size (64).
  You must modify the serial buffer size to 128 -- worked on Mega without
  doing any of this -- maybe its serial code adapts?
  For HardwareSerial modify (no such setting) _SS_MAX_RX_BUFF in (no such file)
   Arduino\hardware\arduino\avr\cores\arduino\SoftwareSerial.h
  there is:~/Library/Arduino15/packages/arduino/hardware/avr/1.6.19/cores
  /arduino/.. stuff, in hardwareSerial.h ? edit rxserialbuffersize prn
  
  For SoftwareSerial modify _SS_MAX_RX_BUFF orig filename didn't exist 
  but this does
   ~/Library/Arduino15/packages/arduino/hardware/avr/1.6.19/libraries/
   SoftwareSerial/src/SoftwareSerial.h which has above setting = 64
*/

#include "WiFiEsp.h"
#include "WiFiEspUdp.h"

// Emulate Serial1 on pins 6/7 if not present (as on MEGA)
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX, would need much slower baud too
#endif

//  include the library and init for 8x2 LCD (display time only)
#include <LiquidCrystal.h>
// initialize new object with the Arduino pins it uses
LiquidCrystal lcd(12, 11, 4, 5, 6, 7);

char ssid[] = "hny_Trp";            // your network SSID (name)
char pass[] = "hunniBunch69";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

char timeServer[] = "time.nist.gov";  // NTP server
unsigned int localPort = 2390;        // local port to listen for UDP packets

const int NTP_PACKET_SIZE = 48;  // NTP timestamp is in the first 48 bytes of the message
const int UDP_TIMEOUT = 2000;    // timeout in miliseconds to wait for an UDP packet to arrive

byte packetBuffer[NTP_PACKET_SIZE]; // buffer to hold incoming and outgoing packets

// A UDP instance to send and receive packets over wireless / UDP
WiFiEspUDP Udp;

void setup()
{ pinMode(40, OUTPUT);  // need this for serial2 level shift 5 --> 3.3
  digitalWrite(40, LOW);  // ground for level shift resistor
  
  lcd.begin(8, 2);
  // initialize serial for debugging and time display to Ser Mon
  Serial.begin(115200);
  // initialize serial for ESP module
  Serial2.begin(115200);
  // initialize ESP module
  WiFi.init(&Serial2);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // you're connected, so inform the user
  Serial.println("You're connected to the network");

  Udp.begin(localPort);
}  // end setup

void loop()
{
  sendNTPpacket(timeServer); // send an NTP packet to a time server

  // wait for a reply for UDP_TIMEOUT miliseconds
  unsigned long startMs = millis();
  while (!Udp.available() && (millis() - startMs) < UDP_TIMEOUT) {}

  Serial.println(Udp.parsePacket());
  if (Udp.parsePacket()) {
    Serial.println("packet received");
    // We've received a packet, read the data from it into the buffer
    Udp.read(packetBuffer, NTP_PACKET_SIZE);

    // the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, extract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    Serial.print("Seconds since 1900 Jan 1: ");
    Serial.println(secsSince1900);

    // now convert NTP time into normal time:
    Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    // print Unix time:
    Serial.println(epoch);

    // print the hour, minute, second:
    Serial.print("The UTC time is ");   // UTC is at Greenwich UK (GMT)
    Serial.print((epoch % 86400L) / 3600); // print the hour (86400 equals secs per day)
    Serial.print(':');
    if (((epoch % 3600) / 60) < 10) {
      // In the first 10 minutes of each hour, add a leading '0'
      Serial.print('0');
       }
    Serial.print((epoch % 3600) / 60); // print the minute (3600 equals secs per minute)
    Serial.print(':');
    if ((epoch % 60) < 10) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      Serial.print('0');
        }
    Serial.println(epoch % 60); // print the second

    //print local hour/min to Ser Mon if on
    Serial.print("Mnt Std Time is ");   // UTC - 7
    // print the hour (86400 equals secs per day)
    Serial.print( ((epoch % 86400L) / 3600) - 7 ); // print the hour (86400 equals secs per day)
    Serial.print(':');
    if (((epoch % 3600) / 60) < 10) {
      // In the first 10 minutes of each hour, put a leading '0'
      Serial.print('0');
    }  //end if 1st 10
    Serial.print((epoch % 3600) / 60); // print the minute

    // print to 2 lines of LCD, if it's present
    lcd.clear();  // first clear screen and set the cursor to (0,0)
    lcd.print("MST:");
    lcd.setCursor(0, 1);  // second line
    lcd.print( ((epoch % 86400L) / 3600) - 7 );
    lcd.print(':');
    if (((epoch % 3600) / 60) < 10) {
      // In the first 10 minutes of each hour, put a leading '0'
      lcd.print('0');
    }  //end if 1st 10
    lcd.print((epoch % 3600) / 60); // print the minute
    
  }   // end udp parse packt

  // wait ten seconds before getting time again
  delay(10000);
  
}  // end loop

// send an NTP request to the time server at the given address
void sendNTPpacket(char *ntpSrv)
{
  // set all bytes in the buffer (byte array) to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)

  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send the packet with them, requesting a timestamp:
  Udp.beginPacket(ntpSrv, 123); //NTP requests are to port 123

  Udp.write(packetBuffer, NTP_PACKET_SIZE);

  Udp.endPacket();
}  // end sendNTPacket

