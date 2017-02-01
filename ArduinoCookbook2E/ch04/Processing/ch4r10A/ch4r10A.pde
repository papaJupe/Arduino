// Processing Sketch
              
/*
 * ArduinoMouse.pde  (Processing sketch)
 */

/* WARNING: This sketch takes over your mouse
 Press escape to close running sketch */

import java.awt.AWTException;
import java.awt.Robot;
import processing.serial.*;
import java.awt.Dimension;  // needed for Processing 2.x

Serial    myPort;   // Create object from Serial class
arduMouse myMouse;  // create arduino controlled mouse  
                             
public static final short LF = 10;        // ASCII linefeed
public static final short portIndex = 0;  // select the com port, 
                                          // 0 is the first port

int posX, posY, btn; // data from msg fields will be stored here   

void setup() {
  size(200, 200);
  println(Serial.list());
  println(" Connecting to -> " + Serial.list()[portIndex]);
  myPort = new Serial(this,Serial.list()[portIndex], 9600);
  myMouse = new arduMouse(); 
  btn = 0; // turn mouse off until requested by Arduino message
}

void draw() {
   if ( btn != 0)       
      myMouse.move(posX, posY); // move mouse to received x and y position
}

void serialEvent(Serial p) {
  String message = myPort.readStringUntil(LF); // read serial data
  if(message != null)
  {
    //print(message);
    String [] data  = message.split(","); // Split the comma-separated message
    if ( data[0].equals("Data"))// check for data header    
    {
      if( data.length > 3 )
      {
        try {
          posX = Integer.parseInt(data[1]);  
          posY = Integer.parseInt(data[2]); 
          btn  = Integer.parseInt(data[3]);
        }
        catch (Throwable t) {
          println("."); // parse error
          print(message);
        }          
      }
    }
  }
}

class arduMouse {
  Robot myRobot;     // create object from Robot class;
  static final short rate = 4; // multiplier to adjust movement rate
  int centerX, centerY;
  arduMouse() {
    try {
      myRobot = new Robot();
    }
    catch (AWTException e) {
      e.printStackTrace();
    }
    Dimension screen = java.awt.Toolkit.getDefaultToolkit().getScreenSize();
    centerY =  (int)screen.getHeight() / 2 ;
    centerX =  (int)screen.getWidth() / 2;
  }
  // method to move mouse from center of screen by given offset
  void move(int offsetX, int offsetY) {
    myRobot.mouseMove(centerX + (rate* offsetX), centerY - (rate * offsetY));
  }
} 
