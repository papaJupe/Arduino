/*
  ESP8266 Arduino Code and Schematic to Send AT Commands and Print Output
  by MIGUEL on DECEMBER 27, 2014  -- All About EE: Electrical Engineering Website
  setup: ESP shield on Ardu Mega wired to Serial2 pins w/ v. divider on Ardu TX,
  I used pin 16-1K-to ESP RX-2.2K-pin 40 gnd . No WiFi used here.

  mod for MEGA to use faster serial & serial1-3
  The goal of this code: to receive AT commands from the Arduino’s serial mon.
  window, to send them to the ESP8266, and to print the ESP8266’s response to
  the command or to other actions (such as receiving an HTTP request).
*/
//#include <SoftwareSerial.h>  // speed limited to <= 57K if used
// if mega, just use Serial1-3.begin(115200), v. Uno code prn


// mega's Ser1 receive Rx is pin 19, send Tx is pin 18.
// connect the Tx line from the esp to Arduino's pin 19
// and the Rx input of the esp to the Arduino's Tx pin 18 (v. divider best)
// mega's Ser2 receive Rx on pin 17, send Tx on pin 16.(v. divider best)


void setup()
{
  pinMode(40, OUTPUT);
  digitalWrite(40, LOW);  // ground for level shift resistor
  Serial.begin(115200); // need LF and CR enabled in Ser. Mon.
  // this seems to be factory default for ESP
  Serial2.begin(115200); // your esp's baud rate might be different

}

void loop()
{
  //  if(Serial2.available()) // check if the esp is sending; is IF block necessary?
  //      {  // could you just use while? YES
  while (Serial2.available())
  {
    // The esp sends data so display it to the serial monitor
    char c = Serial2.read();     // read the next character.
    Serial.write(c);
  }
  //      }  // end if

  if (Serial.available()) // something typed in Ser. Mon.
  { // could eliminate this IF and just print if cmd !empty
    // some delay good, I use << orig. 1000
    delay(10);

    String command = "";

    while (Serial.available()) // read the command character by character
    {
      // read char; loop should put all in string before exiting
      command += (char)Serial.read();
      delay(5);
    }
    // print to Ser Mon
    Serial.println();
    Serial.print(">>>> ");
    Serial.println(command);

    // send chars to ESP
    Serial2.print(command);
    // bd also seems to echo command
  }  // end if
}   // end loop


