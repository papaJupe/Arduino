/**
 * GettingStarted
 *
 * A sketch to list the available serial ports
 * and display characters received
 */


import processing.serial.*;

Serial myPort;      // Create object from Serial class
int portIndex = 0;  // set this to the port connected to Arduino
int val;            // Data received from the serial port

void setup()
{
  size(200, 200);
  println(Serial.list()); // print the list of all the ports
  println(" Connecting to -> " + Serial.list()[portIndex]);
  myPort = new Serial(this, Serial.list()[portIndex], 9600);
}

void draw()
{
  if ( myPort.available() > 0) // If data is available,
  {
    val = myPort.read();         // read it and store it in val
    print(val);
  }
}
