/*
 WiFiEsp example: ConnectWPA  -- mod for Mega from lib/wifiESP/exampl/
 
 hardware: ESP shield wired to Mega Serial2 pins, RX pin 17 to shield TX,
 TX pin 16 w/ voltage divider to shield RX (shield has 3.3 max safe input)
 grnd end to pin 40
 
 This example connects to an encrypted WiFi network using an ESP8266 module.
 Then it prints the MAC address of the WiFi shield, the IP address obtained
 and other network details.

 For more details see: http://yaab-arduino.blogspot.com/p/wifiesp-example-connect.html
*/

#include "WiFiEsp.h"

// Emulate Serial1 on pins 6/7 if not present
//#ifndef HAVE_HWSERIAL1
//#include "SoftwareSerial.h"
//SoftwareSerial Serial1(6, 7); // RX, TX
//#endif

char ssid[] = "hny_Trp";            // your network SSID (name) 2.4 ghz only
char pass[] = "hunniBunch69";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

void setup()
{
  pinMode(40, OUTPUT);
  digitalWrite(40, LOW);  // ground for level shift resistor
  
  // initialize serial for debugging, connect to ser. mon.
  Serial.begin(115200);
  // initialize serial for comm w/ ESP module
  Serial2.begin(115200);
  // initialize ESP module
  WiFi.init(&Serial2);
  

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue, must reset ?
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED ) {
    Serial.print("Attempt connect to secure SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");
  
}   // end setup

void loop()
{
  // print the network connection information every 10 seconds
  Serial.println();
  printCurrentNet();
  printWifiData();
  
  delay(10000);
}

void printCurrentNet()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to
  byte bssid[6];
  WiFi.BSSID(bssid);
  char buf[20];
  sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", bssid[5], bssid[4], bssid[3], bssid[2], bssid[1], bssid[0]);
  Serial.print("BSSID: ");
  Serial.println(buf);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI): ");
  Serial.println(rssi);
}

void printWifiData()
{
  // print your WiFi shield's IP address (assigned by dhcp from AP?) to ser.mon.
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print your MAC address
  byte mac[6];
  WiFi.macAddress(mac);
  char buf[20];  // format seems to be hex, 2 places, 1 byte
  sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", mac[5], mac[4], mac[3], mac[2], mac[1], mac[0]);
  Serial.print("MAC address: ");
  Serial.println(buf);
}

