/*
  SendBinary
  Sends digital and analog pin values as binary data using VirtualWire library
  See SendBinary in Chapter 4
*/

#include <VirtualWire.h>

const int numberOfAnalogPins = 6; // how many analog pins to read

int data[numberOfAnalogPins];  // the data buffer

// the number of bytes in the data buffer:
const int dataBytes = numberOfAnalogPins * sizeof(int); 

void setup()
{
    // Initialize the IO and ISR
    vw_setup(2000);           // Bits per sec
}

void loop()
{
  int values = 0;
  for(int i=0; i <= numberOfAnalogPins; i++)
  {
    // read the analog ports
    data[i] = analogRead(i); // store the values into the data buffer
  }
  send((byte*)data, dataBytes);
  delay(1000); //send every second
}


void send (byte *data, int nbrOfBytes)
{
  vw_send(data, nbrOfBytes);
  vw_wait_tx(); // Wait until the whole message is gone
}
      
    
