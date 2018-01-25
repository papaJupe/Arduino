/* ESP primary UDP, send/recv UDP data
  from https://thearduinoandme.wordpress.com/tutorials/esp8266-send-receive-binary-data/
  load to primary ESP board -- uses Serial() to comm with PC hosting the ESP board, view
   tx/rx messages that it sends and gets from Processing app over wifi, on same or other
   PC; works as modif with Feather-Huzz, turns LED on/off in response to incoming message
*/

// libs from ~/Library/Arduino15/packages/esp8266/hardware/
// esp8266/2.3.0/libraries/ESP8266WiFi/src/ESP8266WiFi.h <-- exampl here too
#include <ESP8266WiFi.h>  // all such libs are for coding primary
#include <WiFiUDP.h>   //         ESP boards w/ Ardu IDE

// wifi connection variables
const char* ssid = "hny_Trp";  // wifi AP on iMac
const char* password = "hunniBunch69";
boolean wifiConnected = false;

// UDP variables
unsigned int localPort = 8888;  // incoming port of this device
WiFiUDP UDP;
boolean udpConnected = false;  // does this pkt size come from lib ?
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet
char ReplyBuffer[] = "ack"; // a string to send back

void setup()
{
  // Initialise Serial connection (to host PC <--> ESP board over USB)
  Serial.begin(115200);

  // Initialise wifi connection
  wifiConnected = connectWifi();

  // only proceed if wifi connection successful
  if (wifiConnected) {
    udpConnected = connectUDP();
    if (udpConnected) {
      // initialise pin for LED display of input signal
      pinMode(14, OUTPUT);
    }
  }
}  // end setup

void loop() {
  // check if the WiFi and UDP connections were successful
  if (wifiConnected) {
    if (udpConnected) {

      // if there’s data available, read a packet
      int packetSize = UDP.parsePacket();
      if (packetSize)
      {
        Serial.println("");
        Serial.print("Rcvd packet of size ");
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

        // read the packet into packetBufffer, assuming incoming 2 byte #
        UDP.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
        Serial.println("Contents: ");
        int value = packetBuffer[0] * 256 + packetBuffer[1];
        Serial.println(value);

        // send ack reply to the IP address & port that sent us the packet
        UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
        UDP.write(ReplyBuffer);
        UDP.endPacket();

        // turn LED on or off depending on value rcvd
        digitalWrite(14, value);
      }
      delay(100);

    }  // end if UDP connected

  }  // end if wifi Conn

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


