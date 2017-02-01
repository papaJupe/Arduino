/*
 * Simple Client to display IP address obtained from DHCP server 
 * Arduino 1.0 version
 */

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte server[] = { 209,85,229,104  }; // Google

EthernetClient client;

void setup()
{
  Serial.begin(9600);
  if(Ethernet.begin(mac) == 0) { // start ethernet using mac & DHCP
    Serial.println("Failed to configure Ethernet using DHCP");  
    while(true)   // no point in carrying on, so stay in endless loop:
      ;
  } 
  delay(1000); // give the Ethernet shield a second to initialize
  
  Serial.print("This IP address: ");
  IPAddress myIPAddress = Ethernet.localIP(); 
  Serial.print(myIPAddress);  
  if(client.connect(server, 80)>0) {
    Serial.println(" connected");
    client.println("GET /search?q=arduino HTTP/1.0");
    client.println();
  } else {
    Serial.println("connection failed");
  }
}

void loop()
{
  if (client.available()) {
    char c = client.read();
    // uncomment the next line to show all the received characters   
    // Serial.print(c);
  }

  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    for(;;)
      ;
  }
}
