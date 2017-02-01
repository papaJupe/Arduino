/**
 * Displays values coming from a 3 axis sensor
 * values are comma seperated preceeded by "H,")
 *
 * based on: MoveEye example sketch
 */

import processing.serial.*;

Serial myPort;  // Create object from Serial class
short portIndex = 1;  // select the com port, 0 is the first port

public static final short scrnWidth = 600;
public static final short scrnHeight = 600;

int temp = 0;
int x = 0;
int y = 0; 
int z = 0; 

int lf = 10; // 10 is '\n' in ASCII
byte[] inBuffer = new byte[100];

void setup() {
  size(600, 600, P3D);
  fill(204);

  String portName = Serial.list()[portIndex];
  println(Serial.list());
  println("Connecting to -> " + Serial.list()[portIndex]);
  myPort = new Serial(this, portName, 9600);
}

void draw() {
   
  String inputString  = myPort.readStringUntil(lf);
  if (inputString != null)  {
    println(inputString); 
    String [] data = split(inputString, ',');  
    if ((inputString.charAt(0) == 'H') && (data.length > 3)) {
      // valid msg if here
      x =int(data[1]);       
      println("x = " + x);  //Print the value for each field
      y =int(data[2]);   
      println("y = " + y);  //Print the value for each field
      z = int(data[3]);   
      println("z = " + z);  //Print the value for each field
      x = (int)map(x,75,185,10, 100);
      y = (int)map(y,75,185,0,255);
      z = (int)map(z,75,185,200,255);    
      show();
    }
  }
}

void show()
{
  lights();
  background(0);

  camera(x, y, z,      // eyeX, eyeY, eyeZ
       0.0, 0.0, 0.0,  // centerX, centerY, centerZ
       0.0, 1.0, 0.0); // upX, upY, upZ

  noStroke();
  box(90);
  stroke(255);
  line(-100, 0, 0, 100, 0, 0);
  line(0, -100, 0, 0, 100, 0);
  line(0, 0, -100, 0, 0, 100);
}
