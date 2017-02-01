/*
 * Web Client Google Finance sketch
 * get the stock value for google and write to analog pin 3.
 */

#include <SPI.h>      // needed for Arduino versions later than 0018
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char serverName[] = "www.google.com";

EthernetClient client;
float value;

void setup()
{
  Serial.begin(9600);
  if(Ethernet.begin(mac) == 0) { // start ethernet using mac & IP address
    Serial.println("Failed to configure Ethernet using DHCP");  
    while(true)   // no point in carrying on, so stay in endless loop:
      ;
  }
  delay(1000); // give the Ethernet shield a second to initialize
}

void loop()
{
  Serial.print("Connecting...");
  if (client.connect(serverName, 80)>0) {
    client.println("GET //finance?q=google HTTP/1.0"); // todo
    client.println("User-Agent: Arduino 1.0");
    client.println();
  }
  else
  {
    Serial.println("connection failed");
  }
  if (client.connected()) {
     if(client.find("<span class=\"pr\">"))
     {
       client.find(">");  // seek past the next '>'
       value = client.parseFloat();
       Serial.print("google stock is at ");
       Serial.println(value);  // value is printed
     }
    else
      Serial.print("Could not find field");
  }
  else {
    Serial.println("Disconnected");
  }
  client.stop();
  client.flush();
  delay(5000); // 5 seconds between each connect attempt
}
