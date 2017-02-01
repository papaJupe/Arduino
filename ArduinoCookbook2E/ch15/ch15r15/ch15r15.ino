/*
 * Monitor Pachube feed
 * Read feed using V2 API using CSV format
 */

#include <SPI.h>
#include <Ethernet.h>

const unsigned long feedID =    504; // this is the ID of the
                                     // remote Pachube feed that
                                     // you want to connect to
const int  streamCount     =      4; // Number of data streams to get
const long PACHUBE_REFRESH = 600000; // Update every 10 minutes
const long PACHUBE_RETRY   = 10000;  // if connection fails/resets
                                     // wait 10 seconds before trying
                                     // again - should not be less than 5

#define PACHUBE_API_KEY  "your key here . . ." // fill in your API key

// mac address, make sure this is unique on your network
byte mac[] = { 0xCC, 0xAC, 0xBE, 0xEF, 0xFE, 0x91 }; 
char serverName[] = "api.pachube.com";      

int streamData[streamCount];    // change float to long if needed for your data

EthernetClient client;

void setup()
{
  Serial.begin(9600);
  if (Ethernet.begin(mac) == 0) {
    Serial.println(F("Failed to configure Ethernet using DHCP"));
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
}

void loop()
{
   if( getFeed(feedID, streamCount) == true)
   {
      for(int id = 0; id < streamCount; id++){
        Serial.println( streamData[id]);
      }
      Serial.println("--");
      delay(PACHUBE_REFRESH);
   }
   else
   {
      Serial.println(F("Unable to get feed"));
      delay(PACHUBE_RETRY);
   }
}

// returns true if able to connect and get data for all requested streams
boolean getFeed(int feedId, int streamCount )
{
boolean result = false;
  if (client.connect(serverName, 80)>0) {
    client.print(F("GET /v2/feeds/"));
    client.print(feedId);
    client.print(F(".csv HTTP/1.1\r\nHost: api.pachube.com\r\nX-PachubeApiKey: "));
    client.print(PACHUBE_API_KEY);
    client.print("\r\nUser-Agent: Arduino 1.0");
    client.println("\r\n");
  }
  else {
    Serial.println("Connection failed");
  }
  if (client.connected()) {
    Serial.println("Connected");
    if(  client.find("HTTP/1.1") && client.find("200 OK") )
       result = processCSVFeed(streamCount);
    else
       Serial.println("Dropping connection - no 200 OK");
  }
  else {
    Serial.println("Disconnected");
  }
  client.stop();
  client.flush();
  return result;
}

int processCSVFeed(int streamCount)
{
  int processed = 0;
  client.find("\r\n\r\n"); // find the blank line indicating start of data
  for(int id = 0; id < streamCount; id++)
  {
    int id = client.parseInt(); // you can use this to select a specific id
    client.find(","); // skip past timestamp
    streamData[id] = client.parseInt();
    processed++;      
  }
  return(processed == streamCount );  // return true if got all data
}
