/* Works for Universal time display on 128x32 0.96" OLED display using OLED library 
 * https://github.com/squix78/esp8266-oled-ssd1306

   UDP listiner for Tardis Time Server by: M. Ray Burnette 20160816
   Rehash of the 2015 Tardis Time Receiver to use a new OLED library, link above
   Time Portal sends a UDP broadcast to all listeners on a random port (usually 4097)

   Compiled under Arduino.cc IDE 1.6.9 on Linux Mint 17.3 on 20160808
   ESP8266 core: http://arduino.esp8266.com/staging/package_esp8266com_index.json

    Sketch uses 249,137 bytes (23%) of program storage space. Maximum is 1,044,464 bytes.
    Global variables use 41,552 bytes (50%) of dynamic memory, leaving 40,352 bytes for local variables. Maximum is 81,920 bytes.

   NOTE: After power-up with a "TardisTime" GPS over UDP transmitter functioning, 
   allow 3 - 10 minutes to initialize  and Serial output is started.
   AM: TTserver seems to be GPS to UDP device, role of EPS bd unclear: 
   display +/- broadcast?
*/


#define DEMO true
#define DIAG true

#include <Streaming.h>                 // \Documents\Arduino\libraries\Streaming (legacy) user-installed
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include "./OLED_LIB/SSD1306.h" // alias for `#include "SSD1306Wire.h"`


// forward declarations seem to be necessary in 1.6.7+ under Linux
void Listener();


uint8_t      hours, minutes, seconds;  // hours, minure, seconds,
uint8_t      day, month, year;         // year, month, day;
unsigned int localPort = 8888;         // any unused port on LAN
char         packetBuffer[UDP_TX_PACKET_MAX_SIZE];         // buffer to hold incoming packet,
char         ReplyBuffer[] = "";       // a null
IPAddress    apIP(10, 10, 10, 1);      // Private network address: local & gateway
IPAddress    broadcastIP(255, 255, 255, 255);              // https://en.wikipedia.org/wiki/Multicast

String Snumbers[] = {"00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", 
                    "12", "13", "14", "14", "16", "17", "18", "19", "20", "21", "22", "23",
                    "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35",
                    "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47",
                    "48", "49", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59"};

String Smonths[] = {"???", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

WiFiUDP           UDP;                 // UDP protocol on STA interface, localPort
SSD1306           display(0x3c, 0, 2); // display(0x3c, D3, D4)


void setup()
{
  Serial.begin(115200);                // Initialise Serial for older PMB-648
  if(DIAG) {
   Serial << endl << endl;
   Serial << (F("2015 Ray Burnette")) << endl;
   Serial << (F("Tardis Time Listener Version 0.20151008")) << endl;
   Serial << (F("Visit my project web page on http://www.hackster.io/rayburne")) << endl << endl;
  }

  WiFi.mode(WIFI_STA);                 // station mode
  WiFi.disconnect();
  WiFi.begin("TardisTime", "");        // The GPS server

  Serial.println("OLED Init...");      // Wire.pins(int sda, int scl), etc
  display.init();                      // Initialising the UI will init the display too.
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_24);
  display.clear();
  display.drawString(0, 26, " START-UP");
  display.display();

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  display.clear(); delay(500);
  display.drawString(0, 26, "Connected");
  display.display();

  while (! UDP.begin(localPort) )      // UDP protocol connected to localPort variable
  {
    delay(250);
  }

  display.clear(); delay(500);
  display.drawString(0, 26, " UDP Up  ");
  display.display(); delay(500);
  display.clear();
  // char wifiIP = WiFi.localIP();
  // char *P = &wifiIP;
  // display.drawString(0, 0, P);
  // display.display();

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Signal Strength: ");
  Serial.println(WiFi.RSSI());

  delay(1000);
}


void loop()
{
  display.clear();
  Listener();                          // UPD (can you hear me now?)
}


void Listener() {
  int packetSize = UDP.parsePacket();  // if there’s data available, read a packet
  char cTD[8];
  String Sdisplay;
  Sdisplay.reserve(8);

  if (packetSize)
  {
    IPAddress remote = UDP.remoteIP(); // Server provided UDP listening port

    if (DIAG) {                        // For diags only
      Serial << endl;
      Serial << (F("Received packet of size: "));
      Serial << (packetSize);
      Serial << (F(" From remote: "));
    
      for (int k = 0; k < 4; k++)
      {
        Serial << (remote[k], DEC);
        if (k < 3)
        {
          Serial << (F("."));
        }
      }
    
      Serial << (F(", port: "));
      Serial << " UDPremotePort: " << (UDP.remotePort()) << endl;
    }

    UDP.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);        // read the packet into packetBufffer
    UDP.beginPacket(apIP, UDP.remotePort());               // send a reply, to the IP address and port that sent us the packet we received
    UDP.write(ReplyBuffer);            // should not be necessary but it seems that the ESP8266
    UDP.endPacket();                   // is stablized by the inclusion of the send
    char *p = packetBuffer;            // grab a pointer to buffer

    p = strchr(p, ',') + 1;            // position after 1st comma
    yield();
    uint32_t  time  = atoi(p);

    hours    = time / 10000;
    minutes  = (time % 10000) / 100;
    seconds = (time % 100);

    Serial << "Signal Strength: " << WiFi.RSSI() << endl;

    Serial << (F("Time: ")) << ((hours<10)?"0":"") << hours << ":";
    Serial << ((minutes<10)?"0":"") << minutes << ":";
    Serial << ((seconds<10)?"0":"") << seconds << endl;

    Sdisplay += Snumbers[hours] + ":" + Snumbers[minutes] + ":" + Snumbers[seconds];
    Sdisplay.toCharArray(cTD, 9);
    display.drawString(10, 20, cTD);
    display.display();

    yield();
    // Parse to integer date field like Ladyada
    p = strchr(p, ',') + 1; // A/V?
    p = strchr(p, ',') + 1; // lat
    p = strchr(p, ',') + 1; // N/S?
    p = strchr(p, ',') + 1; // lon
    p = strchr(p, ',') + 1; // E/W?
    p = strchr(p, ',') + 1; // speed
    p = strchr(p, ',') + 1; // angle
    p = strchr(p, ',') + 1; // move pass for date DDMMYY

    yield();
    
    // nmea date field looks like: 090914 (European)
    uint32_t fulldate = atoi(p);
    day               = (fulldate / 10000);
    month             = (fulldate % 10000) / 100;
    year              = (fulldate % 100);
    Serial << "Date: " << ((month<10)?"0":"") << month << "/"; 
    Serial << ((day<10)?"0":"") << day << "/" << "20"; 
    Serial << ((year<10)?"0":"") << year << endl;
    // Sdisplay = "";
    // Sdisplay += Smonths[month] + Snumbers[day];
    // Sdisplay.toCharArray(cTD, 9);
    // display.drawString(0, 0, cTD);
    // display.display();

    yield();
  }                                    // if(packetSize)
}                                      // Listener()

