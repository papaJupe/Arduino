/*  ESPrim_UDP_test -- connect to adhoc AP on shield running UDP
  -- primary ESP bd like Huzz-Feat tries to connect to another ESP bd's AP,
  both use UDP to send / receivePacket. e.g. AP on Ardu ESP shield to 
  send data, respond to prompt variously, etc. see also ESPrimUDPtxrx_
  UDP data sketch, similar but connected remote Huzz-Feat w/ LED, to Proc
  app on iMac using UDP and comm over built-in iMac wifi; this test is for
  ESPrim bd (USB ser comm w/PC), I/O udp pkts with Ardu/shield running
  USsr04megaUDP sketch, sending US distance et ack

  created 4 Sep 2010 by M Margolis mod 9 Apr 2012 by Tom Igoe (for shield?)
  updated for the (primary) ESP8266 12 Apr 2015 by Ivan Grokhotkov
  mod AM 1711 from NTPquery: 
  for primary ESP (on PC's USB)--wifi connect--Ardu w/ shield sends data
  from US device

*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

char ssid[] = "espShe";  //  network SSID, (name) of ESP shield AP
char pass[] = "twerpton";       // shield AP's password

unsigned int localPort = 2390; // port (this device) listens for UDP

/*  Lookup the IP address of host by name or use known # */
// IPAddress timeServer(129, 6, 15, 28); // time.nist.gov
// IPAddress timeServerIP; // time.nist.gov NTP server address

IPAddress remoteIP(192, 168, 4, 1); // def addr of shield AP, set manually
uint16_t remotePort = 8888;  // ? how to get, prob must set manually

const int PACKET_SIZE = 32; // arbitrary #, max set in lib ?
byte packetBuffer[PACKET_SIZE]; //buffer to hold I/O packets

// make UDP instance to send and receive packets over UDP
WiFiUDP udp;  // would pointer be better, ? instance in setup

void setup()
{
  Serial.begin(115200);  // Ser Mon can display I/O
  Serial.println();

  // first connect to (ad hoc network) AP on remote ESP shield
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  // would be nice to get AP's remoteIP addr to use later
  // could reconstruct from localIP got by DHCP

  while (WiFi.status() != WL_CONNECTED)
  {
    static byte count = 0;
    delay(500);
    Serial.print(".");
    count++;
    if (count % 30 == 0) Serial.println();
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());   // got from AP w/ DHCP

  Serial.println("Starting UDP");
  udp.begin(localPort);  // starts listen on port set above
  Serial.print("Listening to local port: ");
  Serial.println(udp.localPort());

}  // end setup

void loop()
{   // could send prompt regularly, or manually as here
  // fx uses manually entered numbers above, empties buffer &
//  sendPacket(remoteIP, remotePort); // sends probe packet to remote device
//  Serial.println("sent bytes for 6,9");
//  // wait to see if a reply comes
//  delay(1000);

  int pktLen = udp.parsePacket();
  if (!pktLen)   // len 0 if nothing came
  {
    //Serial.println("no packet yet");
    delay(100);
  }
  else
  {
    Serial.print("packet received, length= ");
    Serial.println(pktLen);
    // got a packet, read the data
    udp.read(packetBuffer, pktLen); // read the packet into the buffer
    // print what came, as number or string, depending
    Serial.print("Contents: ");
    if (pktLen == 2) {  // reconstruct number
      int val = packetBuffer[0] * 128 + packetBuffer[1];
      Serial.print("distance: "); Serial.println(val);
    }
    else {
      packetBuffer[pktLen] = 0;  // puts a null at end of data; ? now a string
      byte b;   // byte array, .write should print ch
      for (b = 0; b < pktLen; b++) Serial.write(packetBuffer[b]);
      Serial.println();
      //      // chars from remote; string stops @ null?
      // Serial.println(packetBuffer);  // fails if byte[]
         }
    delay(10);  // fast loop in case there's more coming
  }  // end else got pkt
  
    // send udp only if some keybd entry, I don't care what
    if (Serial.available()) {
      Serial.read();  // just type one ch
      sendPacket(remoteIP, remotePort); // send probe pkt to remote device
      Serial.println("sent bytes for 6,9");
      delay(100);
    }
} // end loop

// sends probe pkt to remote device, why use addr ref not value?
// also had useless uint32 type; I added port param
void sendPacket(IPAddress& address, uint16_t port) // called in loop
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, PACKET_SIZE);
  Serial.println("sending packet...");
  
  // set values of probe pkt
  packetBuffer[0] = 54;  // ascii 6
  packetBuffer[1] = 44;  // ,
  packetBuffer[2] = 57;  // 9
  packetBuffer[3] = 0;
  //  packetBuffer[12] = 0;
  //  packetBuffer[13] = 0;
  //  packetBuffer[14] = 0;
  //  packetBuffer[15] = 0;
  udp.beginPacket(address, port);
  udp.write(packetBuffer, PACKET_SIZE);
  udp.endPacket();
}  // end sendPack

