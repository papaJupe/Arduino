/*
 RFID sketch
 Displays RFID tag read from ID-12 reader
 */

#include <SoftwareSerial.h>
const int rxpin = 3;                    // pin used to receive from RFID
const int txpin = -1;                   // send is not used
const int RFID_BAUD = 9600; // 2400 for Parallax, 9600 for IDS-12

SoftwareSerial rfid(rxpin, txpin); // new serial port on pin 3

const int startByte   = 0x2; // this character precedes each ID
const int endByte     = 0x3; // character terminating the ID
const int tagLength   = 10;  // the number of characters in tag
const int totalLength = tagLength + 4; //tag length + start, end & 2 check bytes


char tag[tagLength + 1];  // holds the tag and a terminating null

int bytesread = 0;

void setup()
{
  Serial.begin(9600);    // built-in port used to echo the read tags
  rfid.begin(RFID_BAUD);   // initialize software serial for your reader  
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
      lookup(tag);
    }
  }  
}

const int numberIds = 4;
const char *names[numberIds] = {"David",       "Tom",      "Gianluca",      "Massimo" };    
const char *IDs[numberIds]   = { "4500F7408D","4500B90221", "38001F18DD", "0415EDAE0C"};
          
void lookup(const char *id)
{  
  for(int i=0; i < numberIds; i++)
  {
    if( strcmp(IDs[i],id) == 0)
    {
      Serial.print(" Hello ");
      Serial.println(names[i]);
      return; 
    }
  }
  Serial.print("ID not matched");  
}
