/*
 RFID sketch
 Displays RFID tags read from Parallax reader
 */

#include <SoftwareSerial.h>
const int rxpin = 3;            // pin used to receive from RFID
const int txpin = -1;           // send pin is not used
const int RFID_BAUD = 2400;     // 2400 for Parallax, 9600 for IDS-12

SoftwareSerial rfid(rxpin, txpin); // new serial port on pin 3

const int startByte   = 10;  // ASCII line feed precedes each tag
const int endByte     = 13;   // ASCII carriage return terminates each tag
const int tagLength   = 10;  // the number of digits in tag
const int totalLength = tagLength + 2; //tag length + start and end bytes

char tag[tagLength + 1];  // holds the tag and a terminating null

int bytesread = 0;

void setup()
{
  Serial.begin(9600);    // built-in port used to echo the read tags
  rfid.begin(RFID_BAUD); // initialize software serial for your reader  
  pinMode(2,OUTPUT);     // connected to the RFID ENABLE pin
  digitalWrite(2, LOW);  // enable the RFID reader
}


void loop()
{
  if (rfid.available() >=  totalLength) 
  {
    if(rfid.read() == startByte) // is this the start of the tag data
    {
      bytesread = 0;                // start of tag so reset count to 0
      while(bytesread < tagLength)  // read 10 digit code
      {
        if(rfid.peek() == startByte) // check for unexpected start byte
           break;  // if found here then data is invalid so break and start over

        char c = rfid.read();  // its not the start of a new tag so read the character
        if((c == endByte))  // check for unexpecteed end of tag character
          break;
        tag[bytesread] = c; // save the character into the buffer
        bytesread = bytesread + 1; // ready to read next digit
      }
      tag[bytesread] = 0; // terminate the string
      Serial.print("RFID tag is: ");
      Serial.println(tag);
    }
  }  
}
      
    
