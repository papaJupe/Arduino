/*
   Copyright (c) 2015, Majenko Technologies  All rights reserved.
 
   AMod from wifiAccessPt example w/ web serv. wf serv (tcp); listen
    port xxx, when client connect, can pass msg 2 way via SerMon;
    for coding primary ESP8266, comm/upld via usb-ttl <--> tx/rx serial pin
    PC serial app or ESP82 STA mode can ? connect to this AP and tx/rx string
    w/ udp svc. I hope.
*/

#include <ESP8266WiFi.h>  // lib for primary ESP to use wifi com
// #include <WiFiClient.h>   // ? needed for AP to com w/ peripheral
#include <elapsedMillis.h>
#include <WiFiUDP.h>

/* Set these to your desired name for this device to be wifi AP */
const char *ssid = "shieldAP";  // ? use pointer when 'char' type pts 2 string
const char *password = "nojoymudd";
const int port = 4220;  // udp server listen this local port
char recvPackBuff[8];   // if 2 byte treat as int, else char string <=7 ch long?
char replyBuff[] = "AP loves U\r\n";

// WiFiServer server(port);  // incoming data port, ? handle udp
// WiFiClient client;        // ? refers to remote device that can connect to
//                           this AP? can i/o data ?
WiFiUDP udp;  // nb instance in small letter
boolean udpConnected = false;
int doomct = 0;
elapsedMillis timeElapsed;     // declare global so not reset every loop
unsigned int interval = 5000;  // (ms) for local ser mon print q 5 sec

/* browse http://192.168.4.1 or ping to test */

void setup() {

  Serial.begin(115200);  // presume source = keybd -- SerMon
  Serial.println();
  Serial.print("setting softAP...");

  //WiFi.persistent(false);  // true to store current config
  WiFi.mode(WIFI_AP);

  /* can use APconfig to make AP addr = 192.168.0.1 / Gateway = 192.168.0.1 / 
     Subnet = 255.255.255.0; otherwse def is __.4.1
     if you specify the ESP8266's IP-address 192.168.0.1, the function softAPConfig()
      sets the DHCP range as 192.168.0.100 - 192.168.0.200  
   WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
   You can remove password parameter if you want open (unprotect) AP
*/

  boolean result = WiFi.softAP(ssid, password);
  delay(20);
  if (result) {
    Serial.println("AP ready");
  } else {
    Serial.println("soft AP Failed!");
  }

  IPAddress myIP = WiFi.softAPIP();  // uses deflt x.x.4.1 ? Y
  Serial.print("this device IP: ");
  Serial.println(myIP);

  udpConnected = connectUDP(); // v.i. fx body; local udp server recv 4220

  if (udpConnected) {
    pinMode(15, OUTPUT);  // led ON indicate port open
    Serial.println("udp svc active");
    Serial.println("");
  }
  // timeElapsed = 10000;
  // delay(10000);
}  // end setup


void loop() {
  if (udpConnected) {

    int pktLen = udp.parsePacket();  // not clear this repeats unless I break

    // if (!pktLen)   // len 0 if nothing came
    // {
    //   //Serial.println("no packet yet");
    //   delay(200);  // break here ? to restart loop
    // }

    digitalWrite(15, false);
    if (pktLen) {         // len 0 if nothing came, led stay off
      Serial.print("packet rcvd, len = ");
      Serial.println(pktLen);
      digitalWrite(15, true);
      // got a packet, read the data
      udp.read(recvPackBuff, pktLen);  // read data into the buffer
      // if > 8 byte (array size )recv, are they just ignored, ? overwrite array
      // print what came, as number or string, depending
      Serial.print("Content: ");
      if (pktLen <= 2) {  
        // reconstruct int, assumes sent as 2 bytes, BIN format?
        // int val = recvPackBuff[0] * 256 + recvPackBuff[1];
        // convert numeric chars into 2 digit int, could check that they are
        // int newNum = Serial.parseInt();
        Serial.print("recv int = ");
        Serial.write(recvPackBuff); // assumes num recvd as 1-2 char
      }  // end if len 1 or 2
      else {
        recvPackBuff[7] = 0;  // put null ascii 0 at end of array, ? now a string
        int b;               // byte array, .write should print as ch, so may not
                            // need this added to end
        for (b = 0; b < pktLen; b++) Serial.write(recvPackBuff[b]);
        Serial.println();
        //  prints len chars recd from remote; string stops @ null?
        // Serial.println(packetBuffer);  // ? fails if byte[] vs. string
      }  //end else >2
  // send ack reply to the IP address & port that sent us the packet
        udp.beginPacket(udp.remoteIP(), udp.remotePort());
        udp.write(replyBuff);
        udp.endPacket();

    }  // end if got pkt

    // send via udp if local keybd entry on host PC
    int s = Serial.available();      // how big
    char outBuff[s];                // make buffer = byte size of Ser input
    while (Serial.available()) {     // while loop here repeat evaluate?
      for (int i = 0; i < s; i++) {  // load into array all char typed
        outBuff[i] = Serial.read();
      }
      udp.beginPacket(udp.remoteIP(), udp.remotePort());
      udp.write(outBuff);
      udp.endPacket();

      // Serial.read();                     // trigger sending method
      // sendPacket(remoteIP, remotePort);  // send probe pkt to remote device
      // Serial.println("sent bytes for 6,9");
    }  // end Ser input send

  }  // end if udpConn


  else {
    Serial.print("connect Failure");
    doomct++;
    if (doomct > 150) exit(0);  //does Ardu C respect cmd ?
  }
  delay(100);
  // if (wait) {
  //   delay(300000);  // execute once every 5 minutes, don't flood remote service
  // }
}  // fin loop

// connect to udp (service?) – return true if success, else false
boolean connectUDP() {
  boolean state = false;

  Serial.println();  // ? just connect to local svc, not remote device
  Serial.println("Connecting to udp");
  // local udp svc accessed?
  if (udp.begin(port) == 1) {
    Serial.println("Connection successful");
    state = true;
  } else Serial.println("udp svc connect failed");

  return state;
}  // end connectUDP
