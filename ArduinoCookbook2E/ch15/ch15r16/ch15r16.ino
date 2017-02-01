/*
 * Update Pachube feed
 * sends temperature read from (up to) six LM35 sensors
 * V2 API 
 */
 
#include <SPI.h>
#include <Ethernet.h>

const unsigned long feedID  =  2955;  // this is the ID of this feed
const int  streamCount      =     6;  // Number of data streams (sensors) to send
const long REFRESH_INTERVAL = 60000;  // Update every  minute
// if connection fails/resets wait 10 seconds before trying again
// should not be less than 5
const long RETRY_INTERVAL   = 10000;  

#define PACHUBE_API_KEY  "Your key here . . . " // fill in your API key

// make sure this is unique on your network
byte mac[] = { 0xCC, 0xAC, 0xBE, 0xEF, 0xFE, 0x91 }; 
char serverName[] = "www.pachube.com"; 

EthernetClient client;

void setup()
{
  Serial.begin(9600);
  Serial.println("ready");
  if (Ethernet.begin(mac) == 0) { // start ethernet using mac & IP address
    Serial.println("Failed to configure Ethernet using DHCP");  
    while(true)   // no point in carrying on, so stay in endless loop:
      ;
  }
}

void loop()
{
  String dataString = ""; 
  for (int id = 0; id < streamCount; id++)
  {       
     int temperature = getTemperature(id);
     dataString += String(id);
     dataString  += ",";
     dataString += String(temperature);
     dataString  += "\n";  
  }
  if ( putFeed(feedID, dataString, dataString.length()) == true)
  {
     Serial.println("Feed updated");
     delay(REFRESH_INTERVAL);
  }
  else
  {
      Serial.println("Unable to update feed");
      delay(RETRY_INTERVAL);
  }
}

// returns true if able to connect and send data
boolean putFeed(int feedId, String feedData, int length )
{
boolean result = false;
  if (client.connect(serverName, 80)>0) {
    Serial.print("Connecting feed "); Serial.println(feedId);
    client.print("PUT /v2/feeds/");
    client.print(feedId);
    client.print(".csv HTTP/1.1\r\nHost: api.pachube.com\r\nX-PachubeApiKey: ");
    client.print(PACHUBE_API_KEY);
    client.print("\r\nUser-Agent: Arduino 1.0");
    client.print("\r\nContent-Type: text/csv\r\nContent-Length: ");
    client.println(length+2, DEC); // allow for cr/lf
    client.println("Connection: close");
    client.println("\r\n");
    // now print the data:
    Serial.println(feedData); // optional echo to serial monitor
    client.print(feedData); 
    client.println("\r\n");
  }
  else {
    Serial.println("Connection failed");
  }
  // response string
  if (client.connected()) {
    Serial.println("Connected");
    if(client.find("HTTP/1.1") && client.find("200 OK") ){
       result = true;
    }
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

// get the temperature rounded up to the nearest degree
int getTemperature(int pin)
{
   int value = analogRead(pin);
   int celsius = (value * 500L) / 1024; // 10mv per degree
   return celsius;
}
