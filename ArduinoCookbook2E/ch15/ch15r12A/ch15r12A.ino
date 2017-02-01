/*
 * Send tweet selected by multiple sensors
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

char frontOpen[] = "The front door was opened";
char backOpen[] = "The back door was opened";

const int frontSensor = 2;  // sensor pins
const int backSensor  = 3;

boolean frontMsgSent = false;
boolean backMsgSent = false;

void setup()
{
//  Ethernet.begin(mac,ip);
  Serial.begin(9600);    
  if(Ethernet.begin(mac) == 0) { // start ethernet using mac & IP address
    Serial.println("Failed to configure Ethernet using DHCP");  
    while(true)   // no point in carrying on, so stay in endless loop:
      ;
  } 
  pinMode(frontSensor, INPUT);
  pinMode(backSensor, INPUT);
  digitalWrite(frontSensor, HIGH);  // pull-ups
  digitalWrite(backSensor, HIGH);
  delay(1000);
  Serial.println("ready");  
}

void loop()
{
  if(digitalRead(frontSensor) == LOW)
  { // here if door is open
      if (frontMsgSent == false) { // check if message already sent
         frontMsgSent = sendMessage(frontOpen);
      }
  }
  else{
      frontMsgSent = false;  // door closed so reset the state
  }
  if(digitalRead(backSensor) == LOW)
  {
    if(frontMsgSent == false) {
       backMsgSent = sendMessage(backOpen);
    }
  }
  else {
      backMsgSent = false;
  }
  delay(100);
}

// add the sendMesage function from the sketch above   
