/* ESP82 wf shield AP mod from Ardu lib / WiFiEsp/ exam/ WebServerAP
    -- for Ardu board (pref. Mega) with ESP shield
  works OK to iPhone and PC, iMac now stays connected. reqCounts
  did skip #s -- now OK from iPh/Safa, iMac/Safa, PC Chrome

  This sketch starts an Access Point and prints its IP address (your
  ESP8266 shield AP) to the Serial monitor. Using that #, you can login
  to the AP from PC using its wifi interface, or maybe USB port wired
  to another ESP config as STA for serial comm. (works but poorly)
  ? send sensor data too? Or can use port 80 and browser if you're
  connected by wifi adapter to the ESP AP on shield: iPh connect auto,
  iMac must set wifi adapter to DHCP, login to AP's SSID & IP, then OK
  w/ Safari

  Wiring: ESP shield TX/RX to RX/TX (17,16) of Mega with voltage div.
  (gnd to TX pin); shield pwr from Ardu; optional, to send sensor data:
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
int state = WL_IDLE_STATUS;     // the Wifi radio's status
int reqCount = 0;                // number of requests received

WiFiEspServer* server;  // obj pointer, instance later in setup, ? safer
// was WiFiEspServer server(333); // 333 for TCP?

// WiFi instance auto-created but need to make server and Client
//   v.i., possib. works better to make in setup

// use a ring buffer to increase speed and reduce memory allocation
// RingBuffer buf(8);  //  keeps 8 most recent 8 bytes or chars?

void setup()
{
  pinMode(15, OUTPUT);
  digitalWrite(15, LOW);  // ground for level shift resistors
  pinMode(53, OUTPUT);
  digitalWrite(53, HIGH);  // 5v for pot, hw grnd
  delay(100);

  Serial.begin(115200);   // init serial for debugging w/ ser. mon.
  Serial2.begin(115200);  // init serial comm w/ ESP shield
  WiFi.init(&Serial2);    // init wifi obj to use Ser2 (shield)
  delay(100);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) //
  {
    Serial.println("WiFi shield not present");
    while (true);        // don't continue
  }

  Serial.print("Attempt to start AP on ");
  Serial.println(ssid);

  // uncomment these two lines to set the IP addr of AP
  // IPAddress localIp(192, 168, 4, 1); // <- default if you don't set
  // WiFi.configAP(localIp);   // optional to set IP

  // start access point, state is global int w/ various uses
  state = WiFi.beginAP(ssid, 10, pass, ENC_TYPE_WPA2_PSK);
  delay(100);
  Serial.println("WiFi.begin state: " + String(state));
  if (state)
    {
    Serial.println("Access point started on ");
    printWifiStatus();
    }
  else Serial.println("AP start failed; reboot?\n");

  // make and init server, 80 for http, 333 for TCP?
  server = new WiFiEspServer(80);
  // start the ___ server on port xxx
  server -> begin();  // call method using pointer
  Serial.println("Server started");

}  // end setup


void loop()
{
  WiFiEspClient client = server->available();  // listen for incoming clients
  // each loop refreshes client obj ?? does connection stay open, doubtful
  if (client) {                          // if you get a client,
    // Serial.println("New client");     // not needed, object prints it
    // buf.init();                  // initialize the circular buffer
    //    while (client.connected()) // mostly same w/ or w/o this loop
    //    { // loop to read bytes while the client's connected
    //      // works poorly, only prints one char, either if or while here->
    while (client.available()) // 
    { // if there's bytes coming from the client,
      char c = client.read();     // read a byte, & do something
      Serial.print(c);      // just print http req 1 by 1 to ser. mon.
                        // GET etc
      // buf.push(c);         // or push it to the ring buffer
      // ring buff just keeps last 4 bytes plus 4 line end chars?
      // you got two double newline characters in a row ?
      // that's the end of the HTTP request, so send a response
      //        if (buf.endsWith("\r\n\r\n")) {
      //          sendHttpResponse(client);
      //          break; // breaks from while loop?
      //        }  // end if buf.ends

    } // end while client avail (has bytes)

    // when no more bytes, send response
     sendHttpResponse(client);
      // break; // from while loop; keeps sending Response if not here
    //    }  // end while client connected

    // time to receive/print the data
    delay(5);

    // close the connection (ever need to keep open ? or http must close)
    client.stop();
    Serial.println("---------");// stop prints disconnect, this shows end
  } // end if client
} // end loop

void sendHttpResponse(WiFiEspClient client)
{ // mod to send sensor data @ times or on request?
  client.print(  // prints first 2 commands only
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Connection: close\r\n"  // the connection closes after completion of response
    "Refresh: 20\r\n" "\r\n"); // browser refreshes page every 20 sec
               // sending new GET request and prompting this response
  client.print("<!DOCTYPE HTML>\r\n");
  client.print("<html>\r\n");
  client.print("<h1>Yello Whirl!</h1>\r\n");
  client.print("Requests received: ");
  client.print(++reqCount);  // sends only odd numbers to Win Chrome
  client.print("<br>\r\n");  // ? IE; OK on iPhn, iMac Safa
  client.print("Analog input A_: ");
  client.print(analogRead(A14));
  client.print("<br>\r\n");
  client.print("</html>\r\n");
}  // end sendHTTP

void printWifiStatus()
{
  // print your WiFi shield's IP address (as AP)
  IPAddress ip = WiFi.localIP();
  Serial.print("AP's IP Address: ");
  Serial.println(ip);

  // print AP name & page addr to point browser
  Serial.println();
  Serial.print("1st connect to AP: ");
  Serial.println(ssid);
  Serial.print("then browse to: ");
  Serial.println(ip);
  Serial.println();
}   // end printWiFistatus
