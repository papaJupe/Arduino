/* ESPrimUDP GPS time (stub) aka
   Tardis Time Server by: M. Ray Burnette 20150910 -- code for programming primary
    ESP board in Ardu IDE; orig got time from GPS, sends over wifi, this seems like test
    stub, no GPS input, etc; may work to test various I/O. More code in TardisTimeServ.ino
    
   Create a private 10. network and capture all DNS requests to the Tardis Time server
   Respond to both UDP and HTML requests:
    Sketch uses 345,724 bytes (79%) of program storage space. Maximum is 434,160 bytes.
    Global variables use 53,448 bytes (65%) of dynamic memory, leaving 28,472 bytes 
    for local variables. Maximum is 81,920 bytes.
*/

#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include "./Utility.h"        // See Notes tab for credits

const byte   DNS_PORT  =   53;     // Capture DNS requests on port 53
int          ack_Count =    0;
unsigned int localPort = 8888;    // any unused port on LAN
IPAddress    apIP(10, 10, 10, 1);     // Private network address: local & gateway

char         packetBuffer[UDP_TX_PACKET_MAX_SIZE];  // buffer to hold incoming packet,
char         ReplyBuffer[] = "acknowledged";    // a 12 character string to send back

String       responseHTML = ""
                      "<!DOCTYPE html><html><head><title>CaptivePortal</title></head><body>"
                      "<h1>Tardis Time</h1><p>Coordinated Universal Time (UTC) "
                      "HH:MM:SS  YYYYMMDD </p></body></html>";

DNSServer         dnsServer;             // Create the DNS object
WiFiUDP           UDP;        // UDP protocol on STA interface, localPort
ESP8266WebServer  webServer(80);     // HTTP web server on common port 80


void setup()
{
  Serial.begin(115200);     // Initialise Serial connection

  WiFi.mode(WIFI_AP_STA);                            // AP + STA
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));   // subnet FF FF FF 00
  WiFi.softAP("TardisTime");

  dnsServer.start(DNS_PORT, "*", apIP);            // "*" creates a captive portal

  // replay to all requests with same HTML
  webServer.onNotFound([]() {
    webServer.send(200, "text/html", responseHTML);
  });

  webServer.begin();
  
  while (! connectUDP() ) {                                     // protocol connected ?
    Serial.print(".");
    delay( 100 );
  }

  Serial.println("Setting pin#2 to Output mode");
  pinMode(2, OUTPUT);                                      // initialise pin mode
}


void loop()
{
  dnsServer.processNextRequest();
  delay(0);
  webServer.handleClient();
  delay(0);
  int packetSize = UDP.parsePacket();    // if there’s data available, read a packet
  
  if (packetSize)
  {
    Serial.println("");
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = UDP.remoteIP();

    for (int i = 0; i < 4; i++)
    {
      Serial.print(remote[i], DEC);
      if (i < 3)
      {
        Serial.print(".");
      }
    }

    Serial.print(", port ");
    Serial.println(UDP.remotePort());

    UDP.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);  // read packet into packetBufffer
    Serial.print("Contents: ");
    int value = packetBuffer[0] * 256 + packetBuffer[1];
    Serial.println(value);
    ++ack_Count;               // added mrb to give an ack serial-number
    Serial.print("Ack counter: ");
    Serial.println(ack_Count);      // ever increasing serialnumber
    intToStr(ack_Count, ReplyBuffer, 12);            // 12 characters wide

        // send a reply, to the IP address and port that sent us the packet
   UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
    UDP.write(ReplyBuffer);
    UDP.endPacket();

    digitalWrite(2, value);      // turn LED on or off depending on value received
  }
  delay(10);
}


boolean connectUDP()   // connect to UDP – returns true if successful / false if not
{
  boolean state = false;

  Serial.println("");
  Serial.println("Connecting to UDP");

  if (UDP.begin(localPort) == 1) {
    Serial.println("Connection successful");
    state = true;
  }
  else {
    Serial.println("Connection failed");
  }
  return state;
}


