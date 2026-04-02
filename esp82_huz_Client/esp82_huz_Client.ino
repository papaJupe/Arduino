/*
   esp82_huz_client mod from example in ~/Library/Arduino15/
  packgs/esp8266/hard/esp8266/2.3.0/lib/ESP8266WiFi/...Client
  -- this is Ardu 'core' lib for primary ESP8266 bd like Feat-Huzz,
    or direct connx to ESP on shield, not shield riding on Ardu 
        which use WiFiEsp libs
   connect prim ESP to PC (USB dongle + ardu SM) load this, attempt to
   connect to (other) AP (esp + shield/usb dongle to ardu CM), pass data;
   OR this could connect as slave to AP on Mac doing wifi Sharing, but
   could Ardu or Proc see this as serial port?
   connects to shield AP, works some; I'm not clear
   what's going on w/ client-serv connection, why only some
   chars get thru; far from transparent seri comm <-- huge 5 ?
   sec. delay in loop
*/

#include <ESP8266WiFi.h>
// not sure why he uses pointers here
const char* ssid     = "slugs24"; // remote esp/other AP's name
const char* password = "bl0-Kesmadder2";
//const char* ssid     = "tabuRaza"; // AP on iMac
//const char* password = "crap0-la4711";
int value = 0; // loop counter, I guess
const char* host = "192.168.50.1"; // host ip ?

void setup() {
  Serial.begin(115200);  // local IDE's SM to ESP over USB port
  delay(50);

  // try connect to a WiFi network (or AP on another ESP)

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
    } // end if ; should this time out and tell you ?
  } // end while waiting to connect


  Serial.println();
  Serial.println("WiFi connected, local dev IP:");
  Serial.println(WiFi.localIP());  // this bd's, got from DHCP? Y

}   // end setup

void loop() {
  delay(2000);  // was 5K, why ? v. slow sending of input
  // ++value;
  // not specifying a protocol, so prob. TCP, but maybe that's 
  // not suitable for purpose of transparent serial conn.
  
  // Use WiFiClient class to create TCP connection (new each loop?)
  WiFiClient client;  // assuming ? this dev's ser port is client
  const int tcPort = 333; //.connect can use host (ssid) or IP ?
  // tried client.remoteIP() for host, but failed; maybe doesn't
  // know it until goodly connect?
  if (!client.connect(host, tcPort))
  {
    Serial.println("connect to AP server fail");
    return;  // does this skip to start loop again? Y
  }
  else Serial.println("got connection to host");

  // if any local SM input, send chars to the server on host AP
  while (Serial.available()) // theoretically should buffer all
   // content of SM msg when return typed
    client.print((char)Serial.read()); // & send the lot
    
  // I'm not clear about connection start and stop
  //  unsigned long timeout = millis();
  //
  //  while (client.available() == 0) {
  //    if (millis() - timeout > 5000) {
  //      Serial.println(">>> Client Timeout !");
  //      client.stop();
  //      return;  // exits while or loop too
  //    } // likely would waken on next loop
  //  }

  //  // Read all the lines of the reply from server and print them to Serial
  //  while(client.available()){
  //    String line = client.readStringUntil('\r');
  //    Serial.print(line);
  //  }

  // Read each char from remote server and print to Serial Mon 1 by 1 ?
  while (client.available())
    Serial.print((char)client.read());

  Serial.println(); // after a packet?
  // Serial.println("closing connection");
}  // end loop

