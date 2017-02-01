/*
  Simple Client Parsing sketch
  Arduino 1.0 version
 */
#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char serverName[] = "search.yahoo.com";

EthernetClient client;

int result; // the result of the google calculation

void setup()
{
  Serial.begin(9600);
  if(Ethernet.begin(mac) == 0) { // start ethernet using mac & IP address
    Serial.println("Failed to configure Ethernet using DHCP");  
    while(true)   // no point in carrying on, so stay in endless loop:
      ;
  }
  delay(1000); // give the Ethernet shield a second to initialize

  Serial.println("connecting...");
}

void loop()
{
  if (client.connect(serverName, 80)>0) {
    Serial.print("connected... ");
    client.println("GET /search?p=50+km+in+miles HTTP/1.0");
    client.println();
  } else {
    Serial.println("connection failed");
  }
  if (client.connected()) {
    if(client.find("<b>50 Kilometers")){
      if(client.find("=")){
         result = client.parseInt();
         Serial.print("50 km is " );
         Serial.print(result);
         Serial.println(" miles");
      }
    }
    else
       Serial.println("result not found");
    client.stop();
    delay(10000); // check again in 10 seconds
  }
  else {
    Serial.println();
    Serial.println("not connected");
    client.stop();
    delay(1000);
  }
}
