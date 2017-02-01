/*
 * Web Client Babelfish sketch
 * Uses Post to get data from a web server
 */

#include <SPI.h>    
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char serverName[] = "babelfish.yahoo.com";

EthernetClient client;

// the text to translate
char * transText = "trtext=Ciao+mondo+da+Arduino.&lp=it_en";

const int MY_BUFFER_SIZE = 30;  // big enough to hold result
char buffer [MY_BUFFER_SIZE+1]; // allow for the terminating null

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
  postPage( "/translate_txt", transText);
  delay(5000);
}

void postPage(char *webPage, char *parameter){
  if (client.connect(serverName,80)>0) {
    client.print("POST ");
    client.print(webPage);
    client.println("  HTTP/1.0");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Host: babelfish.yahoo.com");
    client.print("Content-Length: ");
    client.println(strlen(parameter));
    client.println();
    client.println(parameter);
  }
  else {
    Serial.println(" connection failed");
  }
  if (client.connected()) {
     client.find("<div id=\"result\">");
     client.find( ">");
     memset(buffer,0, sizeof(buffer)); // clear the buffer
     client.readBytesUntil('<' ,buffer, MY_BUFFER_SIZE);
     Serial.println(buffer);
  }
  else {
    Serial.println("Disconnected");
  }
  client.stop();
  client.flush();
}
