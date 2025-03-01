/*
 WiFiEsp example: ScanNetworks
 setup: ESP8266 shield wired to Mega Serial2

 This example prints the Wifi shield's MAC address, and scans
 for available Wifi (2.4 ghz only) networks using the ESP shield.
 Every ten seconds, it scans again. It doesn't attempt to
 connect to any network.

 For more details see:(actually no more detail there)
 http://yaab-arduino.blogspot.com/p/wifiesp.html
*/

#include "WiFiEsp.h"

// Emulate Serial2 on pins 6/7 if not present, as on MEGA
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial2(6, 7); // RX, TX
#endif

void setup() {
  pinMode(40, OUTPUT);  // might not compile for Uno
  digitalWrite(40, LOW);  // MEGA ground for level shift resistor
  // initialize serial for debugging
  Serial.begin(115200);
  // initialize serial for ESP module
  Serial2.begin(115200);  // soft serial needs slower
  // initialize ESP module
  WiFi.init(&Serial2);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // Print shield's WiFi MAC address
  printMacAddress();
}

void loop()
{
  // scan for existing networks
  Serial.println();
  Serial.println("Scanning available networks...");
  listNetworks();
  delay(10000);
}


void printMacAddress()
{
  // get your shield's MAC address
  byte mac[6];
  WiFi.macAddress(mac);
  
  // print MAC address
  char buf[20];
  sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", mac[5], mac[4], mac[3], mac[2], mac[1], mac[0]);
  Serial.print("MAC address: ");
  Serial.println(buf);
}

void listNetworks()
{
  // scan for nearby networks
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1) {
    Serial.println("Couldn't see any wifi AP's");
    while (true);
  }

  // print the list of networks seen
  Serial.print("Number of available networks:");
  Serial.println(numSsid);

  // print the network number and name for each network found
  for (int thisNet = 0; thisNet < numSsid; thisNet++) {
    Serial.print(thisNet);
    Serial.print(") ");
    Serial.print(WiFi.SSID(thisNet));
    Serial.print("\tSignal: ");
    Serial.print(WiFi.RSSI(thisNet));
    Serial.print(" dBm");
    Serial.print("\tEncryption: ");
    printEncryptionType(WiFi.encryptionType(thisNet));
  }
}

void printEncryptionType(int thisType) {
  // read the encryption type and print out the name
  switch (thisType) {
    case ENC_TYPE_WEP:
      Serial.print("WEP");
      break;
    case ENC_TYPE_WPA_PSK:
      Serial.print("WPA_PSK");
      break;
    case ENC_TYPE_WPA2_PSK:
      Serial.print("WPA2_PSK");
      break;
    case ENC_TYPE_WPA_WPA2_PSK:
      Serial.print("WPA_WPA2_PSK");
      break;
    case ENC_TYPE_NONE:
      Serial.print("None");
      break;
  }
  Serial.println();
}

