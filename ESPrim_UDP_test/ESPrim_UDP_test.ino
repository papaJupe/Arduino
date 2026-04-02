/*  ESPrim_UDP_test --
  -- primary ESP bd like Huzz-Feat makes connect to another ESP bd's AP, like adhoc AP on shield running UDP
  both use UDP to send / receive Packet. e.g. AP on Ardu ESP shield to
  send data, respond to prompt variously, etc. see also ESPrimUDPtxrx_
  UDP data sketch, similar but connected remote Huzz-Feat w/ LED, to Proc
  app on iMac using UDP and comm over built-in iMac wifi; this test is for
  ESPrim bd (USB ser comm w/PC), I/O udp pkts with Ardu/shield running
  US sensor sr04megaUDP sketch, sending US distance et ack

  created 4 Sep 2010 by M Margolis mod 9 Apr 2012 by Tom Igoe (for shield?)
  updated for the (primary) ESP8266 12 Apr 2015 by Ivan Grokhotkov
  mod AM 1711 from NTPquery:
  for primary ESP (on PC's USB)--wifi connect--Ardu w/ shield sends data
  from US device. [not sure all code details correct]
*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

char ssid[] = "espShe";  //  network SSID, (name) of remote ESP AP
char pass[] = "twerp";   // AP's password

unsigned int localPort = 2390; // port (this device) listens for UDP

/*  Lookup the IP address of host by name or use known # */
// IPAddress timeServer(129, 6, 15, 28); // time.nist.gov
// IPAddress timeServerIP; // time.nist.gov NTP server address
// IPAddr is a byte[4]
IPAddress remoteIP(192, 168, 4, 1); // def addr of shield AP or set in code
uint16_t remotePort = 8888;  //  need to know, set by AP code

const int PACKET_SIZE = 32; // arbitrary #, def max set by lib ~8K?
byte packetBuffer[PACKET_SIZE]; //buffer to hold I/O packets

    //  object to send and receive packets by UDP
WiFiUDP udp;  // would pointer be better, ? instanced in setup

void setup()
{
  Serial.begin(115200);  // Ser Mon can display I/O, ESP deflt
  Serial.println();

  // first connect to (ad hoc) AP on remote ESP shield
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    static byte count = 0;
    delay(500);
    Serial.print(".");
    count++;
    if (count % 30 == 0) Serial.println("continue or reset?");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());   // got from AP w/ DHCP

  Serial.println("attempt UDP start");
  udp.begin(localPort);  // start listen on port set above
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

  int pktLen = udp.parsePacket(); // not clear this repeats unless I break
  if (!pktLen)  // len 0 if nothing came, ?should this be while() N if it blox
  {
    //Serial.println("no packet yet");
    delay(500);  // break here ? to restart loop
  }
  else
  {
    Serial.print("packet received, length= ");
    Serial.println(pktLen);
    // got a packet, read the data
    udp.read(packetBuffer, pktLen); // read the packet into the buffer
    // print what came, as number or string, depending
    Serial.print("Contents: ");
    if (pktLen == 2) {  // reconstruct number from 2 byte input
      int val = packetBuffer[0] * 128 + packetBuffer[1];// should be 256?
      Serial.print("distance: "); Serial.println(val);
    }
    else {
      packetBuffer[pktLen] = 0;  // puts a null at end of data; ? now a string
      byte b;   // byte array, .write should print as ch
      for (b = 0; b < pktLen; b++) Serial.write(packetBuffer[b]);
      Serial.println();
      //      // chars from remote; string stops @ null?
      // Serial.println(packetBuffer);  // fails if byte[] vs. string?
         }
    delay(10);  // fast loop in case there's more coming
  }  // end else got pkt

    // send udp premade packet on any key press
    if (Serial.available()) {
      Serial.read();  // trigger sending fx, this just empties stack
      // ? if it would empty stack if more than one char typed
      
// fx to send preformed packet
      sendPacket(remoteIP, remotePort); // send probe pkt to remote device
      Serial.println("sent bytes for 6,9");
    }
    delay(100);
} // end loop

// send this probe pkt to remote device, param 0 is addr of 4 byte array
// also had useless uint32 type; I added port param
void sendPacket(IPAddress& address, uint16_t port) // called in loop by key press
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, PACKET_SIZE);
  Serial.println("sending packet...");

  // preset values of probe pkt
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
