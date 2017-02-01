/**
 * GoogleEarth_FS.pde 
 * 
 * Drives Google Flight Sim using CSV sensor data
 */

import java.awt.AWTException;
import java.awt.Robot;
import java.awt.event.InputEvent;
import processing.serial.*;
import java.awt.Dimension;

Serial myPort;  // Create object from Serial class

arduMouse myMouse;

String message = null;
int maxDataFields = 7; // 3 axis accel, 2 buttons, 2 joystick axis
boolean isStarted = false;
int accelX, accelY, btnZ; // data from msg fields will be stored here          


void setup() {
  size(260, 260);
  PFont fontA = createFont("Arial.normal", 12);  
  textFont(fontA);

  short portIndex = 1;  // select the com port, 0 is the first port
  String portName = Serial.list()[portIndex];
  println(Serial.list());
  println(" Connecting to -> " + portName) ;
  myPort = new Serial(this, portName, 57600);
  myMouse = new arduMouse();

  fill(0); 
  text("Start Google FS in the center of your screen", 5, 40);
  text("Center the mouse pointer in Google earth", 10, 60);
  text("Press and release Nunchuck Z button to play", 10, 80);  
  text("Press Z button again to pause mouse", 20, 100);
}

void draw() {
  processMessages();
  if (isStarted == false) {
    if ( btnZ != 0) {      
      println("Release button to start");
      do{ processMessages();}
         while(btnZ != 0);
      myMouse.mousePress(InputEvent.BUTTON1_MASK); // starts the F
      isStarted = true;
    }
  }
  else 
  {
    if ( btnZ != 0) { 
      isStarted = false;
      background(204);
      text("Release Z button to play", 20, 100);
      print("Stopped, ");
    }      
    else{
      myMouse.move(accelX, accelY); // move mouse to received x and y position
      fill(0); 
      stroke(255, 0, 0);
      background(#8CE7FC);
      ellipse(127+accelX, 127+accelY, 4, 4);
    }
  }
}

void processMessages() {
  while (myPort.available () > 0) {
    message = myPort.readStringUntil(10); 
    if (message != null) {
      //print(message);  
      String [] data  = message.split(","); // Split the CSV message       
      if ( data[0].equals("Data"))// check for data header    
      {
        try {
          accelX = Integer.parseInt(data[1]);  
          accelY = Integer.parseInt(data[2]); 
          btnZ = Integer.parseInt(data[3]);
        }
        catch (Throwable t) {
          println("."); // parse error
        }
      }
    }
  }
}

class arduMouse {
  Robot myRobot;     // create object from Robot class;
  static final short rate = 4; // pixels to move
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
  // method to simulate pressing mouse button
  void mousePress( int button) {
    myRobot.mousePress(button) ; 
  }
} 

