/* All About EE: Electrical Engineering Website
ESP8266 Arduino Code and Schematic to Send AT Commands and Print Output
by MIGUEL on DECEMBER 27, 2014
Hardware: ESP shield on Ardu (as for wireless serial) but no WiFi here

The goal of this code: to receive AT commands from the Arduino’s serial mon.
window, to send them to the ESP8266, and to print the ESP8266’s response to 
the command or to other actions (such as receiving an HTTP request).
*/

#include <SoftwareSerial.h>
   // if mega, just use Serial1.begin(9600), et al methods
   
SoftwareSerial esp8266(2,3); // set Arduino receive Rx to pin 2,  Arduino send Tx to pin 3.
            // --> you need to connect the TX line from the esp to Arduino pin 2
            // and the RX input to the esp to Arduino Tx pin 3 (using v. divider)
            
void setup()
{
  Serial.begin(9600);
  esp8266.begin(9600); // your esp's baud rate might be different and will be
     // unless reset w/ AT command
}
 
void loop()
{
  if(esp8266.available()) // check if the esp is sending a message to Ardu
  {
    while(esp8266.available())
    {
      // The esp has data so display its output to the serial mon. window 
      char c = esp8266.read(); // read the next character.
      Serial.write(c);
    }  
  }  // end if available
  
  
  if(Serial.available()) // ser. mon. input, send to shield
  {
    // some delay good but orig code was wrong, had 1000
    delay(10); 
    
    String command="";
    
    while(Serial.available()) 
    // read the command character by character, adds all to string
    {
        // read one character
      command+=(char)Serial.read();
    }
    esp8266.println(command); // send the read string to the esp8266
  }  // end if available
}   // end loop
 
 
