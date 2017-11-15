/*
   esp82_huz_client mod from example in ~/Library/Arduino15/
  packgs/esp8266/hard/esp8266/2.3.0/lib/ESP8266WiFi/...Client
  -- this is Ardu 'core' lib for primary ESP8266 bd like Feat-Huzz,
  not shield riding on Ardu which use WiFiEsp libs
  connect prim ESP bd to PC (USB) and this code, attempt to
   connect to esp AP shield on Ardu/Mega and pass data both ways;
   connects to shield AP and server, works some; I'm not clear
   what's going on w/ client-serv connection, why only some
   chars get thru; far from transparent ser comm
*/

#include <ESP8266WiFi.h>
// not sure why he uses pointers here
const char* ssid     = "espShe"; // AP's name
const char* password = "twerpton";
//const char* ssid     = "tabuRaza"; // AP on iMac
//const char* password = "crap0-la4711";
int value = 0; // loop counter, I guess
const char* host = "192.168.4.1"; // ip or dns

void setup() {
  Serial.begin(115200);  // local comm w/ PC over USB
  delay(10);

  // try connect to a WiFi network (or AP on ESP shield)

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    value++;  // otherwise runs off window
    if (value > 40) {
      value = 0;
      Serial.println();
    } // end if
  } // end while waiting to connect


  Serial.println();
  Serial.println("WiFi connected, localIP:");
  Serial.println(WiFi.localIP());  // this bd's, got from DHCP? Y

}   // end setup

void loop() {
  delay(5000);  // why so long, ? prevents fast sending of input
  // ++value;
  // not specifying a protocol, so prob. TCP, but maybe that's 
  // not suitable for purpose of transparent serial conn.
  
  // Use WiFiClient class to create TCP connection (new each loop?)
  WiFiClient client;
  const int tcPort = 333; //.connect can use host or IP, port
  // tried client.remoteIP() for host, but failed; maybe doesn't
  // know it until good connect?
  if (!client.connect(host, tcPort))
  {
    Serial.println("connection to server failed");
    return;  // does this skip to start loop again? Y
  }
  else Serial.println("got connection to host");

  // if any local input, send chars to the server on AP
  while (Serial.available()) // theoretically should buffer all
    client.print((char)Serial.read()); // & send 1 by 1
    
  // I'm not clear about connection start and stop
  //  unsigned long timeout = millis();
  //
  //  while (client.available() == 0) {
  //    if (millis() - timeout > 5000) {
  //      Serial.println(">>> Client Timeout !");
  //      client.stop();
  //      return;  // exits while or loop too
  //    }
  //  }

  //  // Read all the lines of the reply from server and print them to Serial
  //  while(client.available()){
  //    String line = client.readStringUntil('\r');
  //    Serial.print(line);
  //  }

  // Read each char from server and print to Serial 1 by 1
  while (client.available())
    Serial.print((char)client.read());

  Serial.println();
  // Serial.println("closing connection");
}  // end loop

