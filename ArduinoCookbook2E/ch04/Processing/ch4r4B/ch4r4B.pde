/*
 * ShowSensorData. 
 * 
 * Displays bar graph of CSV sensor data ranging from -127 to 127
 * expects format as: "Data,s1,s2,...s12\n" (any number of to 12 sensors is supported)
 * labels can be sent as follows: "Labels,label1, label2,...label12\n");
 */

import processing.serial.*;

Serial myPort;  // Create object from Serial class
String message = null;
PFont fontA;    // font to display servo pin number 
int fontSize = 12;

int maxNumberOfLabels = 12;

int rectMargin = 40;
int windowWidth = 600;
int windowHeight = rectMargin + (maxNumberOfLabels + 1) * (fontSize *2);
int rectWidth = windowWidth - rectMargin*2;
int rectHeight = windowHeight - rectMargin;
int rectCenter = rectMargin + rectWidth / 2;

int origin = rectCenter;
int minValue = -127;
int maxValue = 127;

float scale = float(rectWidth) / (maxValue - minValue);

String [] sensorLabels = {"s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", "s12"};
int labelCount = maxNumberOfLabels;  // this will be changed to the number of labels actually received

void setup() {
  size(windowWidth, windowHeight);
  short portIndex = 1;  // select the com port, 0 is the first port
  String portName = Serial.list()[portIndex];
  println(Serial.list());
  println(" Connecting to -> " + portName) ;
  myPort = new Serial(this, portName, 57600);
  fontA = createFont("Arial.normal", fontSize);  
  textFont(fontA);
  labelCount = sensorLabels.length;
}

void drawGrid() {
  fill(0); 
  text(minValue, xPos(minValue), rectMargin-fontSize);   
  line(xPos(minValue), rectMargin, xPos(minValue), rectHeight + fontSize); 
  text((minValue+maxValue)/2, rectCenter, rectMargin-fontSize);   
  line(rectCenter, rectMargin, rectCenter, rectHeight + fontSize);
  text(maxValue, xPos(maxValue), rectMargin-fontSize);  
  line( xPos(maxValue), rectMargin, xPos(maxValue), rectHeight + fontSize);   

  for (int i=0; i < labelCount; i++) {
    text(sensorLabels[i], fontSize, yPos(i));
    text(sensorLabels[i], xPos(maxValue) + fontSize, yPos(i));
  }
}

int yPos(int index) {
  return rectMargin + fontSize + (index * fontSize*2);
}

int xPos(int value) {
  return origin  + int(scale * value);
}

void drawBar(int yIndex, int value) {             
  rect(origin, yPos(yIndex)-fontSize, value * scale, fontSize);   //draw the value
}

void draw() {

  while (myPort.available () > 0) {
    try {
      message = myPort.readStringUntil(10); 
      if (message != null) {
        print(message); 
        String [] data  = message.split(","); // Split the CSV message
        if ( data[0].equals("Labels") ) { // check for label header       
          labelCount = min(data.length-1, maxNumberOfLabels) ;
          arrayCopy(data, 1, sensorLabels, 0, labelCount );
        }
        else if ( data[0].equals("Data"))// check for data header    
        {
          background(255); 
          drawGrid();   
          fill(204);      
          println(data.length);
          for ( int i=1; i <= labelCount && i < data.length-1; i++) 
          {
              drawBar(i-1, Integer.parseInt(data[i]));            
          }
        }
      }
    }
    catch (Exception e) {
      e.printStackTrace(); // Display whatever error we received
    }
  }
}
