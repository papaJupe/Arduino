/* ESP82 wf shield AP mod from Ardu lib / WiFiEsp/ exam/ WebServerAP
    -- for Ardu board (pref. Mega) with ESP shield.
  works OK to iPhone and PC, iMac can't stay connected. reqCounts
  skip #s -- seems to get 2+ client requests on each refresh
  from PC Chrome, IE or iPhone

  This sketch starts an Access Point and prints its IP address (your
  ESP8266 shield AP) to the Serial monitor. Using that #, you can login
  to the AP from PC using its wifi interface, or maybe USB port wired
  to another ESP config as STA for serial comm. (works but poorly)
  ? send sensor data too? Or can use port 80 and browser if connected
  to the ESP AP on shield.

  Wiring: ESP shield TX/RX to RX/TX (17,16) of Mega with voltage divider
  gnd to 40; shield pwr from Ardu, optional, to send sensor data:
  pot voltage from 51/53, signal to A14
*/

#include "WiFiEsp.h"

// Emulate Serial1 on pins 6/7 if not present, as on Mega
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

char ssid[] = "espShe";         // your AP's network SSID (name)
char pass[] = "twerpton";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
int reqCount = 0;                // number of requests received

WiFiEspServer* server;  // obj pointer, instance later, ? safer
// was WiFiEspServer server(333); // 333 for TCP?

// WiFi instance auto-created but need to make server and Client
//   v.i., works better to make in setup

// use a ring buffer to increase speed and reduce memory allocation
// RingBuffer buf(8);  //  keeps 8 most recent 8 bytes or chars?

void setup()
{
  pinMode(40, OUTPUT);
  digitalWrite(40, LOW);  // ground for level shift resistors
  pinMode(51, OUTPUT);
  digitalWrite(51, HIGH);  // 5v for pot
  pinMode(53, OUTPUT);
  digitalWrite(53, LOW);  // ground for pot
  delay(100);

  Serial.begin(115200);   // init serial for debugging w/ ser. mon.
  Serial2.begin(115200);  // init serial comm w/ ESP shield
  WiFi.init(&Serial2);    // init wifi obj to use Ser2 (shield)
  delay(100);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) 
    {
      Serial.println("WiFi shield not present");
      while (true);        // don't continue
    }

  Serial.print("Attempting to start AP ");
  Serial.println(ssid);

  // uncomment these two lines if you want to set the IP addr of the AP
  IPAddress localIp(192, 168, 4, 1); // is some default if you don't set?
  WiFi.configAP(localIp);

  // start access point
  status = WiFi.beginAP(ssid, 10, pass, ENC_TYPE_WPA2_PSK);

  Serial.println("Access point started");
  printWifiStatus();

  // make and init server, 80 for http, 333 for TCP?
  server = new WiFiEspServer(333);
  // start the ___ server on port xxx
  server -> begin();  // call method using pointer
  Serial.println("Server started");

}  // end setup


void loop()
{
  WiFiEspClient client = server->available();  // listen for incoming clients
  // each loop refreshes client obj ?? does connection stay open
  if (client) {                          // if you get a client,
    // Serial.println("New client");     // not needed, object prints it
    // buf.init();                  // initialize the circular buffer
//    while (client.connected()) // mostly same w/ or w/o this loop
//    { // loop to read bytes while the client's connected
//      // works poorly, only prints one char, either if or while here->
      while (client.available()) // does connectn self terminate?
      { // if there's bytes coming from the client,
        char c = client.read();     // read a byte, & do something
        Serial.print(c);      // just print 1 by 1 to ser. mon.

        // buf.push(c);         // or push it to the ring buffer
        // ring buff just keeps last 4 bytes plus 4 line end chars?
        // you got two double newline characters in a row ?
        // that's the end of the HTTP request, so send a response
        //        if (buf.endsWith("\r\n\r\n")) {
        //          sendHttpResponse(client);
        //          break; // breaks from while loop?
        //        }  // end if buf.ends

      } // end if client avail (has bytes)

      // when no more bytes, send stuff back
      // sendHttpResponse(client);
      // break; // from while loop; keeps sending Response if not here
//    }  // end while client connected

    // time to receive/print the data
    delay(5);

    // close the connection (needed or should I keep open)
    client.stop();
    // Serial.println("Client disconnected"); don't need, auto prints similar
  } // end if client
} // end loop

void sendHttpResponse(WiFiEspClient client)
{ // mod to send sensor data @ times or on request?
  client.print(  // prints first 2 commands only
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Connection: close\r\n"  // the connection closes after completion of response
    "Refresh: 20\r\n" "\r\n");    // refresh the page automatically every 20 sec

  client.print("<!DOCTYPE HTML>\r\n"); // nothing after this
  client.print("<html>\r\n");
  client.print("<h1>Yello Whirl!</h1>\r\n");
  client.print("Requests received: ");
  client.print(++reqCount);  // sends only odd numbers to Win Chrome
  client.print("<br>\r\n");  // also skips #s on IE, iPhn
  client.print("Analog input A_: ");
  client.print(analogRead(A14));
  client.print("<br>\r\n");
  client.print("</html>\r\n");
}

void printWifiStatus()
{
  // print your WiFi shield's IP address (as AP)
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print page addr to point browser
  Serial.println();
  Serial.println("... sittin here thinkin, ");
  //Serial.println(ssid);
  Serial.print("just how sharp I am...");
  // Serial.println(ip);
  Serial.println();
}
