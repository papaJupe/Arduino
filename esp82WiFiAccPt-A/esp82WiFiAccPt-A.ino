/*
   Copyright (c) 2015, Majenko Technologies
   All rights reserved.
 
   AMod from wifiAccessPt example w/ web serv. wf serv (tcp) listen
    port 333, when client connect, can pass msg 2 way via SerMon;
    for coding primary ESP8266, com w/ its serial pin
*/

#include <ESP8266WiFi.h>  // lib for primary ESP doing wifi com
#include <WiFiClient.h>   // ? needed for AP to com w/ peripheral(s)
#include <elapsedMillis.h>
#include <WiFiUdp.h>

/* Set these to your desired names for this device as AP */
const char *ssid = "shieldAP";
const char *password = "nojoymudd";
const int port = 4220;  // server listen this local port

WiFiServer server(port);  // incoming data port, ? handle UDP
WiFiClient client;        // ? refers to remote device that can connect to
// this AP? can i/o data ?, TCP default?

elapsedMillis timeElapsed;     // declare global so not reset every loop
unsigned int interval = 5000;  // (ms) for local ser mon print

/* browse http://192.168.4.1 or ping to test. */

void setup() {
  
  Serial.begin(115200);  // presumably to/from SerMon, + any other ? use
  Serial.println();
  Serial.print("setting softAP...");

  //WiFi.persistent(false);  // true to store current config
  WiFi.mode(WIFI_AP);

  /* can use APconfig to make AP addr = 192.168.0.1 / Gateway = 192.168.0.1 / 
     Subnet = 255.255.255.0; otherwse def is __.4.1
     if you specify the ESP8266's IP-address 192.168.0.1, the function softAPConfig()
      sets the DHCP range as 192.168.0.100 - 192.168.0.200
  */
  // WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

  // You can remove password parameter if you want open (unprotect) AP
   boolean result = WiFi.softAP(ssid, password);
  if(result)
  {
    Serial.println("Ready");
  }
  else
  {
    Serial.println("Failed!");
  }

  IPAddress myIP = WiFi.softAPIP();  // uses deflt x.x.4.1 ?
  Serial.print("this AP IP addr: ");
  Serial.println(myIP);

  server.begin();
  timeElapsed = 10000;
  delay(10000);
}  // end setup


void loop() {
  static bool gotClient = false;  // just applies to first loop

  // check if there is new client login, connect & send ack
  if (server.hasClient() && !gotClient) {  // hC means login try/success ?
    client = server.accept();
    Serial.println("New client");
    gotClient = true;
delay(200);
    // sends ack msg back to client
    Serial.println("sending ack to client");
    if (client.connected()) { client.println("wassup client?"); }
  }  // end if new client present

  // if new client or refreshing post-paws
  if (gotClient && timeElapsed > 10000) { // pause 10 sec to restart recv window
    // start 20 sec timeout window to recv, pause here while nulla bona
    unsigned long startPaws = millis();
    while (gotClient && client.available() == 0) {  // does 'while' block loop
      if (millis() - startPaws > 20000) {           // here while no incoming msg?
        Serial.println(">>> Client Timeout -- nulla bona");
        client.stop();      // does this close client object on server too ? prob Y
        gotClient = false;  // assumes Y
        timeElapsed = 0;    // start 10 sec pause before recv windo open again
        // delay(60000);  // then wait a minute
        break;  // exits while loop
      }          // end if 20 sec. elapsed w/o msg input
    }            // end while client but no data coming
  }              //end if gotClient and timeout @ 20 sec

  // Read msg in from client, print to Serial Mon at sane interval
  if (gotClient && timeElapsed > interval) // 5 sec
    {Serial.println("client recv window open");
    timeElapsed = 0;
    }

  while (client.available()) {  // if client send char string ending /n ? /r
                                // will this read packet as whole? 
                                //  ? N, looks char by char
                                // what is stat_cast vs. cast
    char ch = static_cast<char>(client.read());
    Serial.print(ch);
  }  // end while data incoming

  // need 'client.connected()' to send it data
  if (gotClient && Serial.available()) {
    delay(10);
    while (Serial.available()) {
      char ch = Serial.read();
      client.write(ch);
    }  // end while
  }    // end if gotClient

  // another way to capture & fwd a SerMon string
  //   String command = "";

  //   while (Serial.available())
  //   // build cmnd string char by char
  //   {  command += (char)Serial.read();
  //   } // if echo is on as usual, also see chars sent on Ser mon.
  //   esp8266.println(command);  // send Ser string to the esp8266
  // }

  // Close the connection
  // Serial.println();
  // Serial.println("closing connection");
  // client.stop();

  // if (wait) {
  //   delay(300000);  // execute once every 5 minutes, don't flood remote service
  // }
}  // fin loop
