/*
 WiFiEsp test: BasicTest -- mod for Mega from wifiESP lib/ exampl/ test/
 hardware: ESP shield wired to Mega Serial2 pins, RX pin 17 to shield TX,
 TX pin 16 w/ voltage divider to shield RX (shield has 3.3 max safe input)
 grnd end to pin 40
 
 Performs basic connectivity test and checks.  must enter firmware 
 vers. you expect, IP for this device, wifi net's AP's SSID, password, baud
 rates for your setup. all works as it should but I don't understand how 
 this static IP connects to my totally differently numbered wifi AP
 
*/

#include "WiFiEsp.h"

// Emulate Serial1 on pins 7/6 if not present 
//#ifndef HAVE_HWSERIAL1 <-- expect MEGA would have this
//#include "SoftwareSerial.h"
//SoftwareSerial Serial1(6, 7); // RX, TX can't do 115200
//#endif


char ssid[] = "hny_Trp";     // your network SSID (name), 2.4 ghz only
char pwd[] = "hunniBunch69";  // your network password, should PASS
char pwdErr[] = "xxxx";   // wrong password, is supposed to FAIL


void setup()
{
  Serial.begin(115200);  // these work fine on MEGA
  Serial2.begin(115200);  // soft. serial needs slower
  WiFi.init(&Serial2);
  pinMode(40, OUTPUT);
  digitalWrite(40, LOW);  // ground for level shift resistor
  
}

void loop()
{
  assertEquals("Firmware version", WiFi.firmwareVersion(), "2.0.0");
  assertEquals("Status is (WL_DISCONNECTED)", WiFi.status(), WL_DISCONNECTED);
  assertEquals("Connect", WiFi.begin(ssid, pwd), WL_CONNECTED);
  assertEquals("Check status (WL_CONNECTED)", WiFi.status(), WL_CONNECTED);
  assertEquals("Check SSID", WiFi.SSID(), ssid);

  IPAddress ip = WiFi.localIP();  // not set by DHCP, where from, 1st loop?
  assertNotEquals("Check IP Address", ip[0], 0);
  Serial.print("IP Addr .locIP 1stchk: ");
  Serial.println(ip);
  
  byte mac[6]={0,0,0,0,0,0};
  WiFi.macAddress(mac);

  Serial.print("MAC: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);
  Serial.println();
  
  assertEquals("Disconnect", WiFi.disconnect(), WL_DISCONNECTED);
  assertEquals("Check status (WL_DISCONNECTED)", WiFi.status(), WL_DISCONNECTED);
  assertEquals("IP Address", WiFi.localIP(), 0);
  assertEquals("Check SSID", WiFi.SSID(), "");
  assertEquals("Wrong pwd", WiFi.begin(ssid, pwdErr), WL_CONNECT_FAILED);

  IPAddress localIp(192, 168, 2, 80);  // static IP to assign this device
  // but is it device # on wifi LAN or its AP IP to clients
  // LAN it connects to has totally different #s; may be allowed to
  // change only last # ? others work here, not other sketches?
  WiFi.config(localIp);       // consistent w/ AP's IP / subnet ?
  
  assertEquals("Connect", WiFi.begin(ssid, pwd), WL_CONNECTED);
  assertEquals("Check status (WL_CONNECTED)", WiFi.status(), WL_CONNECTED);

  ip = WiFi.localIP(); // is this .localIP as set above 192.168.x.x ?
  assertNotEquals("Check IP Address", ip[0], 0);  // should pass

  Serial.println("END OF TESTS");
  delay(30000);  // repeats 3/min
}  // end loop

/////////////////////////////////

void assertNotEquals(const char* test, int actual, int expected)
{
  if(actual != expected)
    pass(test);
  else
    fail(test, actual, expected);
}

void assertEquals(const char* test, int actual, int expected)
{
  if(actual == expected)
    pass(test);
  else
    fail(test, actual, expected);
}

void assertEquals(const char* test, char* actual, char* expected)
{
  if(strcmp(actual, expected) == 0)
    pass(test);
  else
    fail(test, actual, expected);
}

void pass(const char* test)
{
  Serial.println(F("*********************************** "));
  Serial.print(test);
  Serial.println(" > PASSED");
  Serial.println();
}

void fail(const char* test, char* actual, char* expected)
{
  Serial.println(F("*********************************** "));
  Serial.print(test);
  Serial.print(" > FAILED");
  Serial.print(" (actual=\"");
  Serial.print(actual);
  Serial.print("\", expected=\"");
  Serial.print(expected);
  Serial.println("\")");
  Serial.println();
  delay(10000);
}

void fail(const char* test, int actual, int expected)
{
  Serial.println(F("********************************* "));
  Serial.print(test);
  Serial.print(" > FAILED");
  Serial.print(" (actual=");
  Serial.print(actual);
  Serial.print(", expected=");
  Serial.print(expected);
  Serial.println(")");
  Serial.println();
  delay(10000);
}

