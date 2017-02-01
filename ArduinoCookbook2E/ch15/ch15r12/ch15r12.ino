/*
 * Send tweet when switch on pin 2 is pressed
 * uses api.thingspeak.com  as a Twitter proxy
 * see: http://community.thingspeak.com/documentation/apps/thingtweet/
 */ 

#include <SPI.h>       
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte server[]  = { 184, 106, 153, 149 };         // IP Address for the ThingSpeak API

char *thingtweetAPIKey = "YourThingTweetAPIKey";  // your ThingTweet API key

EthernetClient client;

boolean MsgSent = false;
const int Sensor = 2;

void setup()
{
  Serial.begin(9600);    
  if (Ethernet.begin(mac) == 0) { // start ethernet using mac & DHCP address
    Serial.println("Failed to configure Ethernet using DHCP");  
    while(true)   // no point in carrying on, so stay in endless loop:
      ;
  } 
  pinMode(Sensor, INPUT);
  digitalWrite(Sensor, HIGH);  //turn on  pull-up resistors
  delay(1000);
  Serial.println("Ready");  
}

void loop()
{
  if(digitalRead(Sensor) == LOW)
  { // here if mailbox is open
    
    if(MsgSent == false){ // check if message already sent
       MsgSent = sendMessage("Mail has been delivered");
       if(MsgSent)
           Serial.println("tweeted successfully");
       else    
           Serial.println("Unable tweet");
    }
  }
  else{
      MsgSent = false;  // door closed so reset the state
  }
  delay(100);
}

boolean sendMessage( char *message)
{
boolean result = false;

  const int tagLen = 16; // the number of tag character used to frame the message
  int msgLen = strlen(message) + tagLen + strlen(thingtweetAPIKey);
  Serial.println("connecting ...");
  if (client.connect(server, 80) ) {
    Serial.println("making POST request...");
    client.print("POST /apps/thingtweet/1/statuses/update HTTP/1.1\r\n");
    client.print("Host: api.thingspeak.com\r\n");
    client.print("Connection: close\r\n");
    client.print("Content-Type: application/x-www-form-urlencoded\r\n");
    client.print("Content-Length: ");
    client.print(msgLen);
    client.print("\r\n\r\n");
    client.print("api_key=");          // msg tag
    client.print(thingtweetAPIKey);    // api key 
    client.print("&status=");          // msg tag 
    client.print(message);             // the message
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