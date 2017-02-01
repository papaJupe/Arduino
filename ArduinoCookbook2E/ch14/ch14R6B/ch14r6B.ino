/*
 * RFM12B wireless demo - struct sender - no ack
 * Sends a floating point value using a C structure 
 */

#include <RF12.h> //from jeelabs.org
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

typedef struct {  // Message data Structure, this must match Tx
  int   pin;	  // pin number used for this measurement          
  float value;	  // floating point measurement value
} 
Payload;

Payload sample;  // declare an instance of type Payload named sample

void loop()
{
  int inputPin = 0; // the input pin
  float value = analogRead(inputPin) * 0.01; // a floating point value
  sample.pin = inputPin; // send demontx.ct1=emontx.ct1+1;
  sample.value = value; 

  while (!rf12_canSend())  // is the driver ready to send?
    rf12_recvDone();       //no so service the driver
    
  rf12_sendStart(rf12_hdr, &sample, sizeof sample); 
  rf12_sendWait(RF12_NORMAL_SENDWAIT);  // wait for send completion

  Serial.print(sample.pin);                  
  Serial.print(" = ");
  Serial.println(sample.value); 
  delay(1000);
}
