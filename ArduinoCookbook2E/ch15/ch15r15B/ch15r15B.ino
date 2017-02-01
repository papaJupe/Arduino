/*
 * Monitor Pachube feed
 * uses DHCP for IP address V2 API using XML format 
 * Parses Time
 */

#include <SPI.h>
#include <Ethernet.h>

#include <Time.h>

const unsigned long feedID  =    504; // this is the ID of the
// remote Pachube feed that you want to connect to
const int  streamCount      =      4; // Number of data streams to get
const long REFRESH_INTERVAL = 600000; // Update every 10 minutes
const long RETRY_INTERVAL   = 10000;  // if connection fails/resets
                                      // wait 10 seconds before trying  
                                      // again - should not be less than 5

#define PACHUBE_API_KEY  "your key here . . ." // fill in your API key

// mac address, make sure this is unique on your network
byte mac[] = { 0xCC, 0xAC, 0xBE, 0xEF, 0xFE, 0x91 }; 
char serverName[] = "api.pachube.com";      

float streamData[streamCount];    // change float to long if needed for your data
//char findBuffer     [11];       // holds one numeric field - room for 10 characters
                                  // and the terminating null

char findBuffer [32];  // holds one numeric field - room for 10 characters

EthernetClient client;

void setup()
{
  Serial.begin(9600);
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
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
    delay(REFRESH_INTERVAL);
  }
  else
  {
    Serial.println("Unable to get feed");
    delay(RETRY_INTERVAL);
  }
}

// returns true if able to connect and get data for all requested streams
// in this feed
boolean getFeed(int feedId, int streamCount )
{
  boolean result = false;
  if (client.connect(serverName, 80)) {
    Serial.print("Connecting feed "); 
    Serial.print(feedId); 
    Serial.print(" ... ");
    client.print("GET /v2/feeds/");
    client.print(feedId);
    // client.print(".csv HTTP/1.1\nHost: pachube.com\nX-PachubeApiKey: ");
    client.print(".xml HTTP/1.1\nHost: api.pachube.com\nX-PachubeApiKey: ");
    client.print(PACHUBE_API_KEY);
    client.print("\nUser-Agent: Arduino");
    client.println("\n");
  }
  else {
    Serial.println("Connection failed");
  }
  if (client.connected()) {
    Serial.println("Connected");
    if(  client.find("HTTP/1.1") && client.find("200 OK") )
      result = processXMLFeed(streamCount);
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


// returns time_t for time string in given sream formated as: yyyy-mm-ddThh:mm:ss
time_t parseTime()
{
  tmElements_t tm; // the raw time structure
  tm.Year  = client.parseInt() - 1970;
  tm.Month = client.parseInt(); 
  tm.Day   = client.parseInt();
  // the 'T' is ignored
  tm.Hour = client.parseInt();
  tm.Minute = client.parseInt();
  tm.Second = client.parseInt();

  return makeTime(tm);
}

// temp
void digitalClockDisplay(time_t t){
  // digital clock display of the time
  Serial.print(hour(t),DEC);
  printDigits(minute(t));
  printDigits(second(t));
  Serial.print(" ");
  Serial.print(day(t),DEC);
  Serial.print(" ");
  Serial.print(month(t),DEC);
  Serial.print(" ");
  Serial.print(year(t),DEC); 
  Serial.println(); 
}

void printDigits(byte digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits,DEC);
}


boolean processCSVFeed(int streamCount)
{
  int processed = 0;
  client.find("\r\n\r\n"); // find the blank line indicating start of data
  for(int id = 0; id < streamCount; id++)
  {
    int feed = client.parseInt();
    int id = client.parseInt();
    client.find(","); // skip past the time
    streamData[id] = client.parseInt();
    processed++;      
  }
  return(processed == streamCount );  // return true if got all data
}

boolean processXMLFeed(int streamCount)
{
  client.find("<environment updated=");
  client.find("T");
  client.readBytesUntil('\"',findBuffer, sizeof(findBuffer));    // get the time
  Serial.print("Values updated at ");
  Serial.println(findBuffer);

  int processed = 0;
  for(int id = 0; id < streamCount; id++)
  {
    if( client.find( "<data id=" ) )  //find next data field
    {
      if(client.find("<current_value at="))
      {
        time_t t =  parseTime();
        digitalClockDisplay(t);
        client.find(">"); // seek to the angle brackets
        streamData[id] = client.parseInt();
        processed++;
      }
    }
    else {
      Serial.print("unable to find Id field ");
      Serial.println(id);
    }
  }
  return(processed == streamCount);  // return true if got all data
}  

