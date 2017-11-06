/*
  Ardu lib / WiFiEsp/ example/ WebServerAP -- for Ardu board (pref. Mega)
  with ESP shield.
 works OK to iPhone and PC, iMac can't stay connected.  reqCounts
 only odd -- seems to get 2 client requests on each refresh
 from PC Chrome, not IE or iPhone
 
  A simple web server that shows the value of an analog input
  pin via a web page served from an ESP8266 shield module.
  This sketch will start an Access Point and print its IP address (your
  ESP8266 shield AP) to the Serial monitor. Using that #, you can login
  to the AP from PC, point web browser to that address to display the
  web page. The web page should automatically refresh every 20 sec.

  For [no more info] see: http://yaab-arduino.blogspot.com/p/wifiesp.html

  Wiring: ESP shield TX/RX to RX/TX (17,16) of Mega with voltage divider
  gnd to 40; shield pwr from Ardu, pot voltage from 51/53, signal to A14
*/

#include "WiFiEsp.h"

// Emulate Serial1 on pins 6/7 if not present already (Mega has)
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

char ssid[] = "espShe";         // your AP's network SSID (name)
char pass[] = "twerpton";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
int reqCount = 0;                // number of requests received

WiFiEspServer server(80);
// WiFi instance auto-created but need to make server instance, and Client v.i.

// use a ring buffer to increase speed and reduce memory allocation
RingBuffer buf(8);  //  presume 8 most recent 8 bytes or chars

void setup()
{
  Serial.begin(115200);   // init serial for debugging w/ ser. mon.
  Serial2.begin(115200);    // initialize serial comm for ESP module I/O
  WiFi.init(&Serial2);    // initialize ESP module to comm on Ser2

  pinMode(40, OUTPUT);
  digitalWrite(40, LOW);  // ground for level shift resistor
  pinMode(51, OUTPUT);
  digitalWrite(51, HIGH);  // 5v for pot
  pinMode(53, OUTPUT);
  digitalWrite(53, LOW);  // ground for pot

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);           // don't continue
  }

  Serial.print("Attempting to start AP ");
  Serial.println(ssid);

  // uncomment these two lines if you want to set the IP address of the AP
  //IPAddress localIp(192, 168, 111, 111); // some default otherwise ? from
  //WiFi.configAP(localIp);

  // start access point
  status = WiFi.beginAP(ssid, 10, pass, ENC_TYPE_WPA2_PSK);

  Serial.println("Access point started");
  printWifiStatus();

  // start the web server on port 80
  server.begin();
  Serial.println("Server started");
}


void loop()
{
  WiFiEspClient client = server.available();  // listen for incoming clients

  if (client) {                               // if you get a client,
    // Serial.println("New client");        // not needed, object prints it
    buf.init();                               // initialize the circular buffer
    while (client.connected()) {              // loop while the client's connected
      if (client.available()) {               // if there's bytes to read from the client,
        char c = client.read();               // read a byte, then
        buf.push(c);                          // push it to the ring buffer
        // ring buff just keeps last 4 bytes plus 4 line end chars?
        // you got two newline characters in a row
        // that's the end of the HTTP request, so send a response
        if (buf.endsWith("\r\n\r\n")) {
          sendHttpResponse(client);
          break; // breaks from while loop?
        }  // end if buf.ends
      } // end if client
    }  // end while connected

    // give the web browser time to receive the data
    delay(10);

    // close the connection
    client.stop();
    // Serial.println("Client disconnected"); not needed, auto prints similar
  } // end if client
} // end loop

void sendHttpResponse(WiFiEspClient client)
{
  client.print(
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Connection: close\r\n"  // the connection closes after completion of response
     "Refresh: 20\r\n" "\r\n");    // refresh the page automatically every 20 sec
    
  client.print("<!DOCTYPE HTML>\r\n");
  client.print("<html>\r\n");
  client.print("<h1>Yello Whirl!</h1>\r\n");
  client.print("Requests received: ");
  client.print(++reqCount);  // sends only odd numbers to Win Chrome
  client.print("<br>\r\n");
  client.print("Analog input A_: ");
  client.print(analogRead(A14));
  client.print("<br>\r\n");
  client.print("</html>\r\n");
}

void printWifiStatus()
{
  // print your WiFi shield's IP address (AP)
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print page addr to go in the browser
  Serial.println();
  Serial.print("To see page, connect to ");
  Serial.println(ssid);
  Serial.print(" and point web browser to http://");
  Serial.println(ip);
  Serial.println();
}
