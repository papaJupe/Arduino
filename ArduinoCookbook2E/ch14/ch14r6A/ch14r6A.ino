/*
 * SimpleReceive
 * RFM12B wireless demo - receiver - no ack
 *
 */

#include <RF12.h>    //from jeelabs.org
#include <Ports.h> 

// RFM12B constants:
const byte network  = 100;   //network group (can be in the range 1-255).
const byte myNodeID = 2;     //unique node ID of receiver (1 through 30)

//Frequency of RFM12B can be RF12_433MHZ, RF12_868MHZ or RF12_915MHZ.
const byte freq = RF12_868MHZ; // Match freq to module

void setup()
{
  rf12_initialize(myNodeID,freq,network);   //Initialize RFM12 with settings defined above  
  Serial.begin(9600); 
  Serial.println("RFM12B Receiver ready");
  Serial.println(network,DEC);   // print the network 
  Serial.println(myNodeID,DEC);  // and node ID
}

const int payloadCount = 6; // the number of integers in the payload message

void loop()
{
  if (rf12_recvDone() && rf12_crc == 0 && (rf12_hdr & RF12_HDR_CTL) == 0) 
  {
    int *payload = (int*)rf12_data;  // access rf12 data buffer as an arrya of ints
    for( int i= 0; i < payloadCount; i++)
    {
      Serial.print(payload[i]);
      Serial.print(" ");   
    }	
    Serial.println();  
  }
}
