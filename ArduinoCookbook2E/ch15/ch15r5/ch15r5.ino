/*
 * Simple Client Google Weather
 * gets xml data from http://www.google.com/ig/api?weather=london,uk
 * reads temperature from field:  <temp_f data="66" />
 * writes temperature  to analog output port.
 */

#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char serverName[] = "www.google.com";

const int temperatureOutPin = 3; // analog output for temperature
const int humidityOutPin    = 5; // analog output for humidity

EthernetClient client;

void setup()
{
  Serial.begin(9600);
  if(Ethernet.begin(mac) == 0) { // start ethernet using mac & IP address
    Serial.println("Failed to configure Ethernet using DHCP");  
    while(true)   // no point in carrying on, so stay in endless loop:
      ;
  }
  delay(1000); // give the Ethernet shield a second to initialize

  Serial.println("connecting...");
}

void loop()
{
  if (client.connect(serverName,80)>0) {
  // get google weather for London
    client.println("GET /ig/api?weather=london HTTP/1.0");  
    client.println();
  }
  else {
    Serial.println(" connection failed");
  }
  if (client.connected()) {
     // get temperature in fahrenheit (use field "<temp_c data=\"" for Celsius)
     if(client.find("<temp_f data=") )
     {
        int temperature = client.parseInt();
        analogWrite(temperatureOutPin, temperature); // write analog output
        Serial.print("Temperature is ");  // and echo it to the serial port
        Serial.println(temperature);
     }
    else
      Serial.print("Could not find temperature field");
     // get temperature in fahrenheit (use field "<temp_c data=\"" for Celsius)
     if(client.find("<humidity data=") )      
     {
        int humidity = client.parseInt(); 
        analogWrite(humidityOutPin, humidity); // write value to analog port
        Serial.print("Humidity is ");  // and echo it to the serial port
        Serial.println(humidity); 
     }
     else
       Serial.print("Could not find humidity field");
  }
  else {
    Serial.println("Disconnected");
  }
  client.stop();
  client.flush();
  delay(60000); // wait a minute before next update
}