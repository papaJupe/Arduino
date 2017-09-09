/*
 WiFiEsp test: BasicTest
 
 Performs basic connectivity test and checks.  must enter firmware 
 vers. you expect, IP for this device, AP's SSID, password, baud
 rates for your setup
 
*/

#include "WiFiEsp.h"

// Emulate Serial1 on pins 7/6 if not present 
//#ifndef HAVE_HWSERIAL1 <-- expect MEGA would have this
//#include "SoftwareSerial.h"
//SoftwareSerial Serial1(6, 7); // RX, TX can't do 115200
//#endif


char ssid[] = "hny_Trp";     // your network SSID (name)
char pwd[] = "hunniBunch69";  // your network password, should PASS
char pwdErr[] = "xxxx";   // wrong password, is supposed to FAIL


void setup()
{
  Serial.begin(115200);  // these work fine on MEGA
  Serial2.begin(115200);  // soft. serial needs slower
  WiFi.init(&Serial2);
}

void loop()
{
  assertEquals("Firmware version", WiFi.firmwareVersion(), "2.0.0");
  assertEquals("Status is (WL_DISCONNECTED)", WiFi.status(), WL_DISCONNECTED);
  assertEquals("Connect", WiFi.begin(ssid, pwd), WL_CONNECTED);
  assertEquals("Check status (WL_CONNECTED)", WiFi.status(), WL_CONNECTED);
  assertEquals("Check SSID", WiFi.SSID(), ssid);

  IPAddress ip = WiFi.localIP();
  assertNotEquals("Check IP Address", ip[0], 0);
  Serial.print("IP Address: ");
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

  IPAddress localIp(192, 168, 2, 6);  // seems to be IP to assign this device
  WiFi.config(localIp);       // consistent w/ AP's IP / subnet
  
  assertEquals("Connect", WiFi.begin(ssid, pwd), WL_CONNECTED);
  assertEquals("Check status (WL_CONNECTED)", WiFi.status(), WL_CONNECTED);

  ip = WiFi.localIP();
  assertNotEquals("Check IP Address", ip[0], 0);

  Serial.println("END OF TESTS");
  delay(60000);  // will repeat 1/min
}

/////////////////////////////////

void assertNotEquals(const char* test, int actual, int expected)
{
  if(actual!=expected)
    pass(test);
  else
    fail(test, actual, expected);
}

void assertEquals(const char* test, int actual, int expected)
{
  if(actual==expected)
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

