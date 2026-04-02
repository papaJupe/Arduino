/* ESP primary UDP, send/recv UDP data w/ some wifi AP, possibly another ESPxxx?
  from https://thearduinoandme.wordpress.com/tutorials/esp8266-send-receive-binary-data/
  load to primary ESP board -- uses USB Serial() Mon. to comm with PC hosting the ESP board;
  the wifi AP that this board connects to could recv/send from its own Ser. Mon. or to/from
   Processing or other UART + UDP capable app connected to the AP device's port; 
   works as modif with Feather-Huzz, turns LED on/off in response to incoming msg value

   libs from ~/Library/Arduino15/packages/esp8266/hardware/
   esp8266/2.3.0/libraries/ESP8266WiFi/src/ESP8266WiFi.h <-- exampl here too
*/


#include <ESP8266WiFi.h>  // all such libs are for coding primary
#include <WiFiUDP.h>      //         ESP boards w/ Ardu IDE

// wifi connection variables
const char* ssid = "hny_Trp";  // wifi AP w/ UDP server
const char* pass = "hunniBunch69";
boolean wifiConnected = false;
int doomct = 0;  // need restart if connect failure

// UDP variables
unsigned int localPort = 8888;  // incoming port of this device
WiFiUDP UDP;
boolean udpConnected = false;
char packetBuffer[16];  // was 8192?
// does MAX pkt size come from lib ?
// char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  //buffer to hold incoming packet
char ReplyBuffer[] = "ack";  // a string to send back, better to make pointer?

void setup() {
  // Initialise Serial connection (to PC hosting this ESP board over USB)
  Serial.begin(115200);
  // set this device to Station mode
  Serial.println();
  Serial.println("setting station...");
  WiFi.mode(WIFI_STA);

  // Initialise wifi connect
  wifiConnected = connectWifi();  // v.i. fx details

  // only proceed if wifi connection successful
  if (wifiConnected) {
    udpConnected = connectUDP();  // v.i. fx prints success / fail
    if (udpConnected) {
      // initialise pin for LED display of input signal & could print ____
      pinMode(14, OUTPUT);
      Serial.println("");
      Serial.print("WiFi connect, IP addr: ");  // this (assigned by AP)
      Serial.println(WiFi.localIP());
      // Serial.printf("UDP server on port %d ", localPort);
    }  // end if UPD
  }    // end if WiFi
  else
    Serial.println("connect failed"); 
}  // end setup

void loop() {
  // check the WiFi and UDP connection
  if (wifiConnected) &&(udpConnected) {

      // if there’s data available, read a packet
      int packetSize = UDP.parsePacket();
      if (packetSize) {
        Serial.println("");
        Serial.printf("Rcvd packet of size  %d\n", packetSize);
        Serial.println(packetSize);
        Serial.print("From ");
        IPAddress remote = UDP.remoteIP();  // 4 bytes
        for (int i = 0; i < 4; i++) {
          Serial.print(remote[i], DEC);  // print as int x4
          if (i < 3) {
            Serial.print(".");
          }
        }
        Serial.print(", port ");
        Serial.println(UDP.remotePort());

        // handle longer packet as string
        // read the packet into packetBufffer, assuming incoming is 2 byte #
        UDP.read(packetBuffer, 2);
        Serial.println("Contents: ");
        int value = packetBuffer[0] * 256 + packetBuffer[1];
        Serial.println(value);  // 0 vs. 1

        // send ack reply to the IP address & port that sent the packet
        UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
        UDP.write(ReplyBuffer);
        UDP.endPacket();

        // turn LED on or off depending on value rcvd,  ? if 0 off
        digitalWrite(14, value);  // if 0 then off, else ON ?
      }
    }  // end if wifi + udp Connect
  else {
    Serial.print("connect Failure");
    doomct++;
    if (doomct > 150) exit(0);
  }
  delay(100);
}  // end loop

// connect to wifi – returns true if successful, false if not
boolean connectWifi() {
  boolean state = true;
  int i = 0;
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Attempt WiFi Connection");

  // Wait for connection, 10 tries
  Serial.print("...trying...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10) {
      state = false;
      break;
    }  // end if
    i++;
  }  // end while
  if (state) {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");  // of this device from DHCP
    Serial.println(WiFi.localIP());
  }  // end if

  else
    Serial.println("Connection failed.");

  return state;
}  // end connectwifi fx

// connect to UDP (service?) – return true if successful, else false
boolean connectUDP() {
  boolean state = false;

  Serial.println();  // ? just connect to local svc, not remote device
  Serial.println("Connecting to UDP");

  if (UDP.begin(localPort) == 1) {
    Serial.println("Connection successful");
    state = true;
  } else Serial.println("UDP svc failed");

  return state;
}  // end connectUDP
