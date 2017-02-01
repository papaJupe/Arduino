// Processing sketch to draw line that follows nunchuck data

import processing.serial.*;

Serial myPort;  // Create object from Serial class
public static final short portIndex = 1;

void setup()
{
  size(200, 200);
  // Open whatever port is the one you're using - See Chapter 4
  myPort = new Serial(this,Serial.list()[portIndex], 9600);
}

void draw()
{
  if ( myPort.available() > 0) {  // If data is available,
    int y = myPort.read();        // read it and store it in val
    background(255);              // Set background to white
    line(0,63-y,127,y);           // draw the line
  }
}