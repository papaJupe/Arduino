/* 
 Sensor Reader 4 -- mod of v.3 to make 3 ints simulating timed volt/curr read;
  for testing Proc, sends 3 ints like:  elaps min, dropping v., random curr
 
 Connections: none, maybe free wire to A5
 */

//const int leftSensor = 3;    // analog input for the left arm
//const int rightSensor = 4;  // analog input for the right arm
const int thirdSensor = 5;   // hanging wire OK for random source 100-660

int leftValue = 0;           // reading from the left 
int rightValue = 0;          // reading from the right
int thirdValue = 0;          // reading from the end pin

void setup() 
{
  // configure the serial connection:
  Serial.begin(9600);  // needs to be same as Proc's for it to read serial port
  // while (Serial.available() <= 0)  // setup halts here until something typed
  Serial.println("wake up");  // should get Proc. serial event to start up
}

void loop() 
{
  //  if (Serial.available() > 0)     // type char \r to read values x 1
  //  {
  //    // read the serial buffer to clear it
  //    // you don't care about the value of the incoming byte:
  //    Serial.read();   // was  int inByte = -- don't need the ch value, 
  //   }
  // static time = millis();  // sets once
  // read the analog pins  (simulate change with time)
  for (int i = 0; i < 202; i += 13) // repeats every 10 sec
  { 
	  leftValue = i;
	  rightValue = 12800 - i;
	  thirdValue = 800 + (random(-3,3)*analogRead(thirdSensor)/9);
	  //    // print sends as char's, so OK in ser mon or Proc string input
	  Serial.println(String(leftValue) + "," + String(rightValue) + "," + String(thirdValue));
	   // Ser.write sends # as byte (0-255) so wraps around if higher than that
	   //Serial.write(pinValue);
	  delay (5000);
  } // end for
  // wnen done, send some text
  Serial.println("that's all folks");
  delay(2000);
  Serial.println("will repeat soon ...");
  delay(2000);
} // end loop
