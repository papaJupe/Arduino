/* 
 Sensor Reader Grapher from Igoe MTT, ch3, proj5, 
 Context: Arduino
 orig SensorReader read two analog inputs and two digital inputs and prints 
      their values to ser. port (or mon.); I added if block for handshaking w/ Proc.; 
 this mod for graphing or byte read, sends series of single ints from one sensor only
 
 Connections:  
 analog sensor(s) on analog input pin (or pot simulates)
 // 2 momentary switches on digital I/O pins

 */
//
//const int leftSensor = 3;    // analog input for the left arm
//const int rightSensor = 4;   // analog input for the right arm
//const int resetButton = 14;   // digital input for the reset button 14=A0
//const int serveButton = 15;   // digital input for the serve button 15=A1 
//
//int leftValue = 0;           // reading from the left 
//int rightValue = 0;          // reading from the right
//bool reset = 0;               // reading from the reset button, bool = boolean
//boolean serve = 0;            // reading from the serve button

const byte analSens = 3;

void setup() 
{
  // configure the serial connection:
  Serial.begin(9600);  // needs to be same as Proc's for it to read serial port
  // configure the digital inputs to any 2 pins (not D0 or D1)
  // pinMode(resetButton, INPUT_PULLUP);  // closing switch grounds the pin, low = on = 1
  // pinMode(serveButton, INPUT_PULLUP);
  // while (Serial.available() <= 0)  // setup halts here until something typed
  Serial.println("wake up");  // should get Proc. serial event to start up

}

void loop() 
{
  //  if (Serial.available() > 0)     //type char\r to read values x 1
  //  {
  //    // read the serial buffer to clear it
  //    // you don't care about the value of
  //    // the incoming byte:
  //    Serial.read();   // was  int inByte = -- don't need the ch value, 
  //                    // just clears buffer

  // read the analog sensor:
  int pinValue = analogRead(analSens);


  //    rightValue = analogRead(rightSensor);
  //    // read the digital sensors:
  //    reset = !digitalRead(resetButton);
  //    serve = !digitalRead(serveButton);
  //
  //    // print the results:
  //    Serial.print(leftValue, DEC);
  //    Serial.print(",");
  //    Serial.print(rightValue, DEC);  // prints the AR as integer
  //    Serial.print(",");
  //    Serial.print(reset);  //DEC formatter optional here and above
  //    Serial.print(",");
  //    // each set of four readings prints on a line by itself:
  //    Serial.println(serve);  // prints the boolean as 0/1

  Serial.println(pinValue); // sends as char's, so OK in ser mon or Proc string input
  //  Ser.write sends # as byte (0-255) so wraps around if higher than that
   //Serial.write(pinValue);
  delay (2000);

} // end loop
