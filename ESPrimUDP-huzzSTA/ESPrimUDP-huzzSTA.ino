/* for ESP primary bd as STA, send/recv UDP data from https://thearduinoandme.wordpress.com/tutorials/esp8266-send-receive-binary-data

load to primary ESP board -- connect to wifi router, then from other machine  use some app to make serial UDP com to this device's Ardu ser. mon.
*/

// libs from ~/Library/Arduino15/packages/esp8266/hardware/
// esp8266/2.3.0/libraries/ESP8266WiFi/src/ESP8266WiFi.h <-- exampl here too
#include <ESP8266WiFi.h>  // all such libs are for coding primary
#include <WiFiUDP.h>   //                  ESP boards w/ Ardu IDE

// wifi connection vars
const char* ssid = "slugs24";  // wifi AP to internet
const char* password = "bl0-Kesmadder2";
boolean wifiConnected = false;
boolean value = false;

// UDP variables
unsigned int localPort = 4210;  // incoming port of this device
WiFiUDP udp;
boolean udpConnected = false;  // does this pkt size come from lib ?
// was char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer incoming packt
char packetBuffer[69];
char replyBuffer[] = "ack\r\n"; // a string to send back

void setup()
{
  // Initialise Serial connection ( host PC <--> ESP board over USB)
  Serial.begin(115200);

  // Initialise wifi connection  v.i. these fx, timeout if failing
 // wifiConnected = connectWifi();

  // only proceed if wifi connection good
//   if (wifiConnected) {
//     udpConnected = connectUDP();
//     if (udpConnected) {
//       // initialise pin for LED display of input signal
//       pinMode(14, OUTPUT);
//     }
//   }
   WiFi.mode(WIFI_STA);
   
  Serial.println();
  Serial.printf("Connecting to %s ", ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  Udp.begin(localPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", \
         WiFi.localIP().toString().c_str(), localPort);

}  // end setup

void loop() {
  // check if the WiFi and UDP connections are good
//   if (wifiConnected && udpConnected {

      // if there’s data available, read a packet
      int packetSize = udp.parsePacket();
      if (packetSize)
     {
    // receive incoming UDP packet
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, \
          udp.remoteIP().toString().c_str(), udp.remotePort());
    int len = udp.read(packetBuffer, 68);
    if (len >2) value = true; // if > 2 char led on 
     else if (len > 0)  // if 1 or 2 set false and clear buff
    {
      packetBuffer[len] = 0; // append to make string
      value = false;
    }
    Serial.printf("UDP packet: %s\n", packetBuffer);

    // send ack reply to the IP address & port the packet came from
    udp.beginPacket(udp.remoteIP(), udp.remotePort());
    udp.write(replyBuffer);
    udp.endPacket();
  }  // end if packetSize

        // turn LED on or off depending on len rcvd
        digitalWrite(14, value);
      
      delay(100);

}  // end loop

// connect to wifi – returns true if successful, false if not
boolean connectWifi()
{
  boolean state = true;
  int i = 0;
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Attempt WiFi Connection");

  // Wait for connection, 10 tries
  Serial.print("...trying...");
  while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
        if (i > 10) 
        {
          state = false;
          break;
        }  // end if
      i++;
    }  // end while
  if (state) 
    {
      Serial.println("");
      Serial.print("Connected to ");
      Serial.println(ssid);
      Serial.print("IP address: ");  // of this device from DHCP
      Serial.println(WiFi.localIP());
    }  // end if
  
  else Serial.println("Connection failed.");
 
  return state;
}  // end connectwifi fx

// connect to UDP (service?) – return true if successful, else false
boolean connectUDP()
{
  boolean state = false;

  Serial.println();  // ? just connect to local svc, not remote dev
  Serial.println("Connecting to UDP");

  if (UDP.begin(localPort) == 1) 
    {
      Serial.println("Connection successful");
      state = true;
    }
  else  Serial.println("Connection failed");
 
  return state;
}  // end connectUDP

// alleged way to test from Proc. app
/*
  test (shell/netcat):
  --------------------
    nc -u 192.168.esp.address 8888
*/
