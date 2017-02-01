/*
 * Web Client DNS sketch
 * Arduino 1.0 version
 */

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char serverName[] = "www.google.com";

EthernetClient client;

void setup()
{
  Serial.begin(9600);
  if (Ethernet.begin(mac) == 0) { // start ethernet using mac & IP address
    Serial.println("Failed to configure Ethernet using DHCP");  
    while(true)   // no point in carrying on, so stay in endless loop:
      ;
  }
  delay(1000); // give the Ethernet shield a second to initialize
  
  int ret = client.connect(serverName, 80);
  if (ret == 1) {
    Serial.println("connected"); //  report successful connection
    // Make a HTTP request:
    client.println("GET /search?q=arduino HTTP/1.0");
    client.println();
  } 
  else {
    Serial.println("connection failed, err: ");
    Serial.print(ret,DEC);
  }
}

void loop()
{
  // Read and print incoming butes from the server:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // stop the client if disconnected:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    while(true) ;  // endless loop    
  }
}