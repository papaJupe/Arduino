/* All About EE: Electrical Engineering Website
ESP8266 Arduino Code and Schematic to Send AT Commands and Print Output
by MIGUEL on DECEMBER 27, 2014  -- mod for MEGA and faster serial & serial1-3
The goal of this code is simple: to receive AT commands from the Arduino’s serial 
window, to send them to the ESP8266, and to print the ESP8266’s response to 
the command or to other actions (such as receiving an HTTP request).
*/
   //#include <SoftwareSerial.h>  // speed limited to <= 57K if that
   // if mega, just use Serial1-3.begin(115200)
   
// mega's Ser1 receive Rx on pin 19, send Tx on pin 18.
  //  connect the Tx line from the esp to Arduino's pin 19
  // and the Rx input of the esp to the Arduino's Tx pin 18
  
void setup()
{
  Serial.begin(115200); // need LF and CR enabled in Ser. Mon. 
  // this seems to be factory default for ESP
  Serial2.begin(115200); // your esp's baud rate might be different
  pinMode(40, OUTPUT);
  digitalWrite(40, LOW);  // ground for level shift resistor
}
 
void loop()
{
  if(Serial2.available()) // check if the esp is sending; is IF block necessary? 
      {  // could you just use while?
        while(Serial2.available())
            {
              // The esp sends data so display it to the serial monitor 
              char c = Serial2.read();     // read the next character.
              Serial.write(c);
             }  
      }  // end if
  
  
  if(Serial.available())  // something entered to Ser. Mon.
  {
    // some delay good, I use << orig. 1000
        delay(10); 
    
    String command="";
    
    while(Serial.available()) // read the command character by character
        {
            // read one character
          command+=(char)Serial.read();
          delay(5);
        }
    // print in Ser Mon
    Serial.println();
    Serial.print(">>>> ");
    Serial.println(command);
    
    // send chars to ESP
    Serial2.print(command);
  }  // end if
}   // end loop
 
 
