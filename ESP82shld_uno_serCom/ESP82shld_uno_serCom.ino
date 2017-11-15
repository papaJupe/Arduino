/* All About EE: Electrical Engineering Website
ESP8266 Arduino Code and Schematic to Send AT Commands and Print Output
by MIGUEL on DECEMBER 27, 2014
Hardware: ESP shield on Ardu (as for wireless serial) but no WiFi here

goal of this code: to receive AT commands typed to Arduino’s serial mon.
window, send them to the ESP8266 over soft ser., then get ESP8266’s response to 
the command or to other actions (such as receiving an HTTP request) over soft ser.
and print to ser. mon. window
*/

#include <SoftwareSerial.h>
   // if mega, just use Serial1.begin(9600), et al methods
   
SoftwareSerial esp8266(2,3); // set Arduino receive Rx to pin 2,  Arduino send Tx to pin 3.
            // --> you need to connect the TX line from the esp to Arduino pin 2
            // and the RX input of the esp to Arduino Tx pin 3 (using v. divider)
            
void setup()
{
  Serial.begin(9600);
  esp8266.begin(9600); // your esp's baud rate might be different and will be
     // unless reset w/ AT command, def. is 115200?
}
 
void loop()
{     // not sure I need the if here
  if(esp8266.available()) // check if the esp is sending stuff to Ardu w. soft ser.
  {
    while(esp8266.available())
    {
      // The esp has data so display its output to the serial mon. window 
      char c = esp8266.read(); // read the next character.
      Serial.write(c);
    }  
  }  // end if available
  
  
  if(Serial.available()) // get Ser. mon. input, send to shield via soft ser
  {  // not sure if if needed here, maybe to bracket the buffered input printout?
    // some delay good but orig code was wrong, had 1000
    delay(10); 
    
    String command="";
    
    while(Serial.available()) 
       // read the command character by character, add all to string
    {      // read one character
      command+=(char)Serial.read();
    }   // if echo is on as usual, also see chars sent echoed to Ser mon.
    esp8266.println(command); // send the read string to the esp8266
  }  // end if available
}   // end loop
 
 
