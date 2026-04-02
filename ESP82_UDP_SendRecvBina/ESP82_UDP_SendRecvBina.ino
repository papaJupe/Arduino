#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

// wifi connection variables
const char* ssid = "NETGEAR70";
const char* password = "grandprairie397";
boolean wifiConnected = false;
byte ip[4]       = {192,168,1,17}; // the remote IP address

// UDP variables
unsigned int localPort = 8888;
WiFiUDP UDP;
boolean udpConnected = false;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "acknowledged";       // a string to send back

// timing variables
long previousMillis = 0;
long period = 1000;
int light = 0;

void setup() {
  // Initialise Serial connection
  Serial.begin(115200);
  
  // Initialise wifi connection
  wifiConnected = connectWifi();

  // only proceed if wifi connection successful
  if(wifiConnected){
    udpConnected = connectUDP();
    if (udpConnected){
      // Notify User
      Serial.println("UDP connection established");
    }
  }
}

void loop() {
  // check if the WiFi and UDP connections were successful
  if(wifiConnected){
    if(udpConnected){
      if (previousMillis < millis() - period){

        if (light == 1){
          light = 0;
        }
        else {
          light = 1;
        }
        Serial.print("sending: ");Serial.println(light);
      
        byte message[2];
        message[0] = 0;
        message[1] = light;
        UDP.beginPacket(ip,localPort);
        UDP.write(message,sizeof(message));
        UDP.endPacket();

        previousMillis = previousMillis + period;
      }

      // if there's data available, read a packet
      int packetSize = UDP.parsePacket();
      if(packetSize)
      {
        /*Serial.println("");
        Serial.print("Received packet of size ");
        Serial.println(packetSize);
        Serial.print("From ");
        IPAddress remote = UDP.remoteIP();
        for (int i =0; i < 4; i++)
        {
          //Serial.print(remote[i], DEC);
          if (i < 3)
          {
            //Serial.print(".");
          }
        }
        Serial.print(", port ");
        Serial.println(UDP.remotePort());
        */
        // read the packet into packetBufffer
        UDP.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
        Serial.println("Contents:");
        for (int i = 0; i < packetSize; i++){
          Serial.print(packetBuffer[i]);
        }
        Serial.println("");
      }
  }
  
}
}



