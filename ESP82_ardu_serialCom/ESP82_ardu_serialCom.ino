/* All About EE: Electrical Engineering Website
ESP8266 Arduino Code and Schematic to Send AT Commands and Print Output
by MIGUEL on DECEMBER 27, 2014
The goal of this code is simple: to receive AT commands from the Arduino’s serial 
window to send them to the ESP8266, and to print the ESP8266’s response to 
the command or to other actions (such as receiving an HTTP request).
*/
#include <SoftwareSerial.h>
   // if mega, just use Serial1.begin(9600)
   
SoftwareSerial esp8266(2,3); // set Arduino receive Rx to pin 2,  Arduino send Tx to pin 3.
            // This means that you need to connect the TX line from the esp to Arduino's pin 2
            // and the RX input to the esp to the Arduino's Tx pin 3
void setup()
{
  Serial.begin(9600);
  esp8266.begin(9600); // your esp's baud rate might be different and will be
     // unless reset
}
 
void loop()
{
  if(esp8266.available()) // check if the esp is sending a message 
  {
    while(esp8266.available())
    {
      // The esp has data so display its output to the serial window 
      char c = esp8266.read(); // read the next character.
      Serial.write(c);
    }  
  }
  
  
  if(Serial.available())
  {
    // the following delay is required because otherwise the arduino will read the first letter of the command but not the rest
    // In other words without the delay if you type AT+RST, for example, the Arduino will read the letter A, then read the rest and send it
    // but we want to send everything at the same time.  Not sure this is correct
    delay(1000); 
    
    String command="";
    
    while(Serial.available()) // read the command character by character
    {
        // read one character
      command+=(char)Serial.read();
    }
    esp8266.println(command); // send the read character to the esp8266
  }
}
 
 
