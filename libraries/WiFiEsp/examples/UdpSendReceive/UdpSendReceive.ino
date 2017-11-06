/*
 WiFiEsp example: WiFi UDP Send and Receive String -- for Ardu with ESP? Shield
  -- probably orig. made for Ardu WiFi shield (? ESP based)
 This sketch for basic Ardu board w/ shield, waits for UDP packet on localPort 
 coming over WiFi via shield.   When a packet is received an 'ACK' packet is sent to the client on port remotePort.

 For 'more details' see [little there]: http://yaab-arduino.blogspot.com/p/wifiesp.html
*/


#include <WiFiEsp.h>
#include <WiFiEspUdp.h>

// Emulate Serial1 on pins 6/7 if hardware (as Mega has) not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

char ssid[] = "hny_Trp";            // your network SSID (name) 2.4ghz only
char pass[] = "hunniBunch69";     // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

unsigned int localPort = 8888;  // local port to listen on, any unused

char packetBuffer[255];          // buffer to hold incoming packet
char ReplyBuffer[] = "ACK";      // acknowl string to send back

WiFiEspUDP Udp;

void setup() {
/* initialize serial (USB to local PC port) for debugging -- comm w/ ser.mon. but
  both Ardu and other app like Proc. on same PC can't use same port @ once
*/
  Serial.begin(115200);
  // initialize Mega's second serial to comm w/ ESP shield module
  Serial2.begin(115200);  // soft ser. would need much slower
  // initialize ESP module
  WiFi.init(&Serial2);

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }
  
  Serial.println("Connected to wifi");
  printWifiStatus();

  Serial.println("\nTrying connection to server...");
  // if you get a connection, report back via serial mon.:
  Udp.begin(localPort);
  delay(500);
  Serial.print("Listening on port ");
  Serial.println(localPort);
}    // end setup

void loop() {

  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.print("Received packet, size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort()); // only shows 3 digits?, not full #

    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }
    Serial.println("Contents:");
    Serial.println(packetBuffer);

    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort()); // port may be partial digits
    Udp.write(ReplyBuffer);
    Udp.endPacket();
  }  // end if pktsiz
}  // end loop


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
