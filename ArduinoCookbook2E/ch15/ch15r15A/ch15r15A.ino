/*
 * Monitor Pachube feed
 * V2 API using XML format 
 * controls a servo using value of a specified stream 
 */

#include <SPI.h>
#include <Ethernet.h>

#include <Servo.h> // this sketch will control a servo

const unsigned long feedID  =   504; // desired pachube feed
const int  streamToGet      =     0;  // data id of the desired stream

const long PACHUBE_REFRESH = 600000; // Update every 10 minutes
const long PACHUBE_RETRY   = 10000;  // if connection fails/resets

#define PACHUBE_API_KEY  "your key here . . ." // fill in your API key

// mac address, make sure this is unique on your network
byte mac[] = { 0xCC, 0xAC, 0xBE, 0xEF, 0xFE, 0x91 }; 
char serverName[] = "api.pachube.com";      

EthernetClient client;

// stream values returned from pachube will be stored here
int currentValue; // current reading for stream
int minValue;     // minimum value for stream
int maxValue;     // maximum value for stream

Servo myservo;  // create servo object to control a servo

void setup()
{
  Serial.begin(9600);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
}

void loop()
{
  if( getFeed(feedID, streamToGet) == true)
  {
    Serial.print(F("value="));
    Serial.println(currentValue);
    // position proportionaly within range of 0  to 90 degreees 
    int servoPos = map(currentValue, minValue, maxValue, 0,90);
    myservo.write(servoPos);
    Serial.print(F("pos=")); 
    Serial.println(servoPos);
    delay(PACHUBE_REFRESH);
  }
  else
  {
    Serial.println(F("Unable to get feed"));
    delay(PACHUBE_RETRY);
  }
}

// returns true if able to connect and get data for requested stream
boolean getFeed(int feedId, int streamId )
{
  boolean result = false;
  if (client.connect(serverName, 80)>0) {
    Serial.print("Connecting feed "); 
    Serial.print(feedId); 
    Serial.print(" ... ");
    client.print("GET /v2/feeds/");
    client.print(feedId);
    client.print(".xml HTTP/1.1\r\nHost: api.pachube.com\r\nX-PachubeApiKey: ");
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
      result = processXMLFeed(streamId);
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

boolean processXMLFeed(int streamId)
{
  client.find("<environment updated=");
  for(int id = 0; id <= streamId; id++)
  {
    if( client.find( "<data id=" ) ){  //find next data field
      if(client.parseInt()== streamId){ // is this our stream?
        if(client.find("<min_value>")){
          minValue = client.parseInt();  
          if(client.find("<max_value>")){
            maxValue = client.parseInt();  
            if(client.find("<current_value ")){
              client.find(">"); // seek to the angle brackets
              currentValue = client.parseInt();
              return true; // found all the neeed data fields 
            }
          }
        }
      }
    }
    else {
      Serial.print(F("unable to find data for ID "));
      Serial.println(id);
    }
  }
  return false;  // unable to parse the data
}
