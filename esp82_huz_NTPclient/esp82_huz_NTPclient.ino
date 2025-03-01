/*
 Udp NTP Client mod from example in ~/Library/Arduino15/
 packages/esp8266/hardware/esp8266/2.3.0/libraries/ESP8266WiFi/
  -- this is Ardu 'core' lib for primary ESP8266 boards like Feather-Huzzah
  not shields riding on Ardu which use WiFiEsp libs
  
 Gets time from a Network Time Protocol (NTP) time server
 Demonstrates use of UDP sendPacket and ReceivePacket
 For more on NTP time servers and the messages needed to communicate with them,
 see http://en.wikipedia.org/wiki/Network_Time_Protocol

 updated for the ESP8266 12 Apr 2015 
 by Ivan Grokhotkov
  -- works OK w/ Huzzah on my iMac internet share AP, also other router AP
  
 2 libs from ~/Library/Arduino15/packages/esp8266/hardware/esp8266/2.3.0/
 libraries/ESP8266WiFi/src/
 
 */

#include <ESP8266WiFi.h>  // <-- auto instantiates WiFi object w/o
#include <WiFiUdp.h>     // me doing anything

char ssid[] = "roseBudd";  //  your network SSID (name)
char pass[] = "tabuLay4710";       // your network password

unsigned int localPort = 2390;  // local port to listen for UDP packets

/* could hardwire the IP address but won't get benefits of the pool
 *  Lookup the IP address for the host name instead in fx below */
//IPAddress timeServer(129, 6, 15, 28); // time.nist.gov NTP server
IPAddress timeServerIP; // time.nist.gov NTP server address
const char* ntpServerName = "time.nist.gov";

// NTP time stamp is in the first 48 bytes of the message
const int NTP_PACKET_SIZE = 48;

//buffer to hold incoming and outgoing packets
byte packetBuffer[NTP_PACKET_SIZE]; 

// UDP instance lets us send and receive packets over UDP
WiFiUDP udp;

void setup()
{
  Serial.begin(115200);  // IDE ser.mon.'s speed
  Serial.println();

  // IPAddress localIp(192,168,1,69);  // static IP, settable ?
  // as in espWiFi, WiFi.config(localIp); no such fx here, so gets w/ DHCP
   
  // We start by connecting to a WiFi network using params entered above
  Serial.print("Attempt connect to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  Serial.println();
  
  Serial.println("WiFi connected");
  Serial.println("my IP address: ");
// IP from DHCP, possible, otherwise gets from prev config ?
  Serial.println(WiFi.localIP()); 
  Serial.println("Starting UDP");
  udp.begin(localPort);  // uses manually entered # above
  Serial.print("Local port: ");
  Serial.println(udp.localPort());
  
}  // end setup

void loop()
{
  // get a random server from the pool-- server name declared above
  // tSIP is &ref, assigned when this fx runs 
  WiFi.hostByName(ntpServerName, timeServerIP); 
   // shield code just uses this local fx w/ name instead of IP
  sendNTPpacket(timeServerIP); // send NTP packet to time server IP
  // wait 1 sec. for reply if available
  delay(1000); // instead of timeout, just loops 1hz sending new probe
  
  int pktLen = udp.parsePacket();
  if (!pktLen)   // ? 0 if nothing came
    Serial.println("no packet yet");
  
  else {
    Serial.print("packet received, length=");
    Serial.println(pktLen);
    // received a packet, read the data in it
    udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes
    // long = two words. First, extract the two words:
   // each word is just 2 bytes, so I don't see need for uint32_t here
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    Serial.print("Seconds since Jan 1 1900 = " );
    Serial.println(secsSince1900);

    // now convert NTP time into everyday time:
    Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    // print Unix time:
    Serial.println(epoch);

    // print the hour, minute and second:
    Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
    Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
    Serial.print(':');
    if ( ((epoch % 3600) / 60) < 10 ) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      Serial.print('0');
                                      }
    Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
    Serial.print(':');
    if ( (epoch % 60) < 10 ) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      Serial.print('0');
                              }
    Serial.println(epoch % 60); // print the second
  }  // end else
  // wait ten seconds before getting time again
  delay(10000);
}  // end loop

// send NTP request to time server at IP param; had silly 32 bit ret. type --
 // pkt is big, but fx doesn't return anything, so why uint32 type ???
 // also unclear why param is &ref not just value
void sendNTPpacket(IPAddress& address)
  {
        // reset all bytes in the buffer to 0
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    Serial.println("sending NTP packet...");
    // Initialize values needed for NTP request
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
    // you can send a packet requesting a timestamp:
    udp.beginPacket(address, 123); // NTP requests are to port 123
    udp.write(packetBuffer, NTP_PACKET_SIZE);
    udp.endPacket();
  }  // end send NTPkt
