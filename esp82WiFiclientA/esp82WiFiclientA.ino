/*
  for primary ESP board being serial port to/from a local AP's wifi network,
  could provide com port to free standing device or com port to a PC ?
*/

#include <ESP8266WiFi.h>
#include <elapsedMillis.h>

#ifndef APname            // purpose of this ?
#define APname "shieldAP"  // local AP it will connect to
#define APass "nojoymudd"
#endif

// ? advantage of pointer vs. plain char
const char* ssid = "APname";
const char* password = "APass";

const char* host = "some AP";
const uint16_t port = 333;
elapsedMillis timeElapsed;

void setup() {
  delay(500);
  timeElapsed = 0;

  Serial.begin(115200);  // screen i/o w/ SerMon

  // connect to local WiFi network AP, possib another ESP

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set this ESP8266 to be a WiFi-client, otherwise, it default
     to be both a client and access-point -- could cause network problem ? */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);  // inits attempt to connect to AP

  while (WiFi.status() != WL_CONNECTED) { // does this loop locally?
    delay(500);         // blocks setup init while trying
    Serial.print(".");  // ? need a \n occasionally
    // time out after 5 sec.
    // if (timeElapsed >= 5000) {
    //   Serial.println();
    //   Serial.println("no connect, try reset?");
    //   break;  // would setup restart itself?
    // }  // end if

  }  // end while no connect

  Serial.println("");
  Serial.print("WiFi connected, IP address: ");  // this (assign by AP)
  Serial.println(WiFi.localIP());
  // if you don't see this on startup should I reset to try again ?
}

void loop() {
  static bool wait = false;
  delay(500); // assume AP network connect, now try com to its TCP port
// attempting TCP connect
  Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);

  // make fresh WiFiClient instance to make TCP connection
  WiFiClient client;
  delay(100);
  if (!client.connect(host, port)) { // need allow more time for this?
    Serial.println("TCP connect failed");
    delay(3000);
    return;  // does this restart loop p 3 sec? need break instead?
  }

  // This will send a string to the server
  Serial.println("sending data to server");
  if (client.connected()) { client.println("hello from ESP8266"); }

  // wait for data to be come from client
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      delay(10000);
      return;  // does this exit while or restart loop ?
    } // end if 5 sec wait for data then 10 sec. pause for new loop
  } // end while no incoming data in 5 sec

  // Read all the lines of the reply from server and print them to SerialMon
  Serial.println("receiving from remote server");
  // not testing 'client.connected()' since we do not need to send data here
  while (client.available()) {
    char ch = static_cast<char>(client.read()); // unclear what s_c is
    Serial.print(ch);
  } // end incoming data from client

  // Close the connection
  Serial.println();
  Serial.println("closing connection");
  client.stop();

  //if (wait) { // try putting wait in while(client not avail)
    //delay(30000);  // execute once every 0.5 minutes, don't flood remote service

}
 // end loop