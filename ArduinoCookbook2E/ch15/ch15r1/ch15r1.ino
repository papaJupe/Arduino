/*
 * Simple Web Client
 * Arduino 1.0 version
 */

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 1, 177 };    // change to a valid address for your network
byte server[] = { 209,85,229,104  }; // Google
                                     // see text for more on IP addressing

EthernetClient client;

void setup()
{
  Serial.begin(9600);      // start the serial library:
  Ethernet.begin(mac,ip);
  delay(1000);             // give the ethernet hardware a second to initialize

  Serial.println("connecting...");

  if (client.connect(server, 80)) {
    Serial.println("connected");
    client.println("GET /search?q=arduino HTTP/1.0"); // the HTTP request
    client.println();
  } 
  else {
    Serial.println("connection failed");
  }
}

void loop()
{
  if (client.available()) {
    char c = client.read();
    Serial.print(c);  // echo all data received to the Serial Monitor
  }

  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    for(;;)
      ;
  }
}
