/*
 * Enhanced Client Google Weather
 * gets xml data from one of four cities
 * reads temperature from field:  <temp_f data="66" />
 * writes temperature  to analog output port.
 */

#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char serverName[] = "www.google.com";
const int analogOutPin = 3;

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

char *cityString[4] = { "London", "New%20York", "Rome", "Tokyo"};
int   city;

void loop()
{
  city = random(4); // get a random city
  if (client.connect(serverName,80)) {
  // get google weather for London
    client.println("GET /ig/api?weather="); 
    client.print(cityString[city]); // print one of 4 random cities
    client.println(" HTTP/1.0");    
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
        analogWrite(analogOutPin, temperature); // write to analog output pin
        Serial.print(cityString[city]);
        Serial.print(" temperature is ");  // and echo it to the serial port
        Serial.println(temperature);
     }
    else
      Serial.println("Could not find temperature field");
     // get temperature in fahrenheit (use field "<temp_c data=\"" for Celsius)
     if(client.find("<humidity data=") )      
     {
        int humidity = client.parseInt();
        analogWrite(4, humidity);      // write value to analog port
        Serial.print("Humidity is ");  // and echo it to the serial port
        Serial.println(humidity); 
     }
     else
       Serial.println("Could not find humidity field");
  }
  else {
    Serial.println("Disconnected");
  }
  client.stop();
  client.flush();
  delay(60000); // wait a minute before next update
}
