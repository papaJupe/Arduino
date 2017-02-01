/*
  ReceiveBinary
 This sketch  receives six integer values as binary data
 Connect the Receiver data pin to Arduino pin 11
 */
#include <VirtualWire.h>

const int numberOfAnalogPins = 6; // how many analog integer values to receive
int data[numberOfAnalogPins];  // the data buffer

// the number of bytes in the data buffer
const int dataBytes = numberOfAnalogPins * sizeof(int); 

byte msgLength = dataBytes;


void setup()
{
  Serial.begin(9600);
  Serial.println("Ready");

  // Initialize the IO and ISR
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);            // Bits per sec

  vw_rx_start();              // Start the receiver
}

void loop()
{
  if (vw_get_message((byte*)data, &msgLength)) // Non-blocking
  {
    Serial.println("Got: ");
    if(msgLength == dataBytes)
    {
      for (int i = 0; i <  numberOfAnalogPins; i++)
      {
        Serial.print("pin ");
        Serial.print(i);
        Serial.print("=");
        Serial.println(data[i]);
      }
    }
    else
    {
       Serial.print("unexpected msg length of ");
       Serial.println(msgLength);
    }
    Serial.println();
  }
}
      
    
