/*
 * SimpleSend
 * RFM12B wireless demo - transmitter - no ack
 * Sends values of analog inputs 0 through 6
 *
 */

#include <RF12.h>  //from jeelabs.org
#include <Ports.h> 

// RF12B constants:
const byte network  = 100;   //network group (can be in the range 1-255).
const byte myNodeID = 1;     //unique node ID of receiver (1 through 30)

//Frequency of RF12B can be RF12_433MHZ, RF12_868MHZ or RF12_915MHZ.
const byte freq = RF12_868MHZ; // Match freq to module

const byte RF12_NORMAL_SENDWAIT = 0;

void setup()
{
  rf12_initialize(myNodeID, freq, network);   //Initialize RFM12   
}

const int payloadCount = 6; // the number of integers in the payload message
int payload[payloadCount];

void loop()
{
  for( int i= 0; i < payloadCount; i++)
  {
    payload[i] = analogRead(i);   
  }
  while (!rf12_canSend())  // is the driver ready to send?
    rf12_recvDone();       //no so service the driver

  rf12_sendStart(rf12_hdr, payload, payloadCount*sizeof(int));	 
  rf12_sendWait(RF12_NORMAL_SENDWAIT); // wait for send completion

  delay(1000);  // send every second
}
