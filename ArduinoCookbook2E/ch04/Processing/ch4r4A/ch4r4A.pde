// Processing Sketch to read comma delimited serial              
// expects format: H,1,2,3,

import processing.serial.*;

Serial myPort;        // Create object from Serial class
char HEADER = 'H';    // character to identify the start of a message
short LF = 10;        // ASCII linefeed

// WARNING!
// If necessary change the definition below to the correct port
short portIndex = 1;  // select the com port, 0 is the first port

void setup() {
  size(200, 200);
  println(Serial.list());
  println(" Connecting to -> " + Serial.list()[portIndex]);
  myPort = new Serial(this,Serial.list()[portIndex], 9600);
}

void draw() {
}

void serialEvent(Serial p)
{
  String message = myPort.readStringUntil(LF); // read serial data

  if(message != null)
  {
    print(message);
    String [] data = message.split(","); // Split the comma-separated message
    if(data[0].charAt(0) == HEADER && data.length > 3) // check validity
    {
      for( int i = 1; i < data.length-1; i++) // skip the header & end if line                               
      {
        println("Value " +  i + " = " + data[i]);  // Print the field values
      }
      println();
    }
  }
}
