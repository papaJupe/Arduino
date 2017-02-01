/*
adxl345 to Proc, code adapted for FRC breakout board from
http://codeyoung.blogspot.com/2009/11/adxl345-accelerometer-breakout-board.html
    -- Ardu gets accel readings over I2C, sends by Serial to Proc to display graphic

*/

#include <Wire.h>

#define DEVADDR 0x1D    // device address for FRC _345; was (0x53) for Adafr_, Spark_
#define TO_READ 6     // num of bytes to read each time (two bytes for each axis)

byte buff[TO_READ] ;    // buff array holds 6 bytes for each data set read from the device
char str[20];          //string array to hold data as ints, before sending to the serial port
                   // size was [512] in original !!, but only holds 2 spaces and 3 ints
      
void setup()
{
  pinMode(A2,OUTPUT); // for convenience I use anal pins as voltage sources for device
  pinMode(A3,OUTPUT);   // A4-5 are SDA and SCL, so 1 4 pin header takes all wires
  digitalWrite(A2,LOW);  // grnd
  digitalWrite(A3,HIGH);  // '5' v

  delay(1000); // allow time for device to power up

  Serial.begin(9600);  // start serial output
  Wire.begin();        // join i2c bus (address optional for master)
    
  // enabling the ADXL345
  writeTo(DEVADDR, 0x2D, 0);      
  writeTo(DEVADDR, 0x2D, 16);
  writeTo(DEVADDR, 0x2D, 8);
}  // end setup
 
void loop()
{
  byte regAddress = 0x32;  // first (x) axis-acceleration-data register on the ADXL345
  int x, y, z;
  
  readFrom(DEVADDR, regAddress, TO_READ, buff); //read the accel byte data, put it in buffer
  
   // each axis reading comes in 10 bit resolution, in 2 bytes, lo Byte first !
   // we are converting both bytes into one int
  x = (((int)buff[1]) << 8) | buff[0];  // shift hi byte left 8 places, add it to lo byte ==>int
  y = (((int)buff[3])<< 8) | buff[2];  // cast needed because byte is only 8 bits ?
  z = (((int)buff[5]) << 8) | buff[4];
  
  //we send the x y z values as a string to the serial port
  sprintf(str, "%d %d %d", x, y, z);  
  Serial.print(str);
  Serial.write(10);  // end line for parsing on Proc
  
  //It appears that some delay (few ms) is needed in order not to clog the port
  delay(20);  // use 10-20 for Proc display, 1000+ for printing to Ser. Mon./LCD
}  // end loop

/*---------------- Functions -----------------*/

          // Writes a single byte 'val' to 'reg'-ister on 'device'(addr)
void writeTo(byte device, byte reg, byte val) // param1 was int but it's always <127
 {
    Wire.beginTransmission(device); // config transmission to this dev. address 
    Wire.write(reg);        // register address to send to
    Wire.write(val);        // the value to send
    Wire.endTransmission(); // do the data transmission
 }

    //reads 'num' bytes into 'buff' array, starting from a 'reg'(-ister addr) on 'dev'(-ice addr)
void readFrom(byte dev, byte reg, byte num, byte buff[]) // why were param1,3 int?, should be bytes
                                                        // either work, but types must match
 {
   Wire.beginTransmission(dev); // setup transmission to this device addr
   Wire.write(reg);            // the register address to read from
   Wire.endTransmission();     // send it
   
       // line was in online code, but irrelevant to operation
     // Wire.beginTransmission(dev); //setup transmission to device @ this addr
   Wire.requestFrom(dev, num);    // request 6 bytes from this device, param types should match
   
   int i = 0;
   while(Wire.available())    //device may send less than requested (abnormal)
	   { 
	  buff[i] = Wire.read(); // put bytes received into buffer
	  i++;
	   }
   // Wire.endTransmission(); // stop receive operation; line was in online code, but irrelevant to operation
 }  // end readFrom

/* alternate way to encode data for Serial, still abstruse code, nothing saved or gained I can see

// If you want to decrease the code size by 25% (sic), you can try this loop(). 
// It works because the CPU (? 328) is little endian, just like x86 CPU.

 void loop()
 {
 int axis[3] ; // 3 ints = 6 bytes buffer, temporarily hold each data set read
 
 //each axis reading comes in 10 bit resolution, 2 bytes -- lo Byte first!
 //thus we are converting both bytes to one int
 // not clear why the (byte*) cast is needed here, makes it a pointer to a byte ?
 readFrom(DEVADDR, regAddress, sizeof(axis), (byte*)axis); //read the acceleration data from device
 
 // send the x y z values as strings to the serial port
 Serial.print(axis[0], DEC);
 Serial.print('\t');
 Serial.print(axis[1], DEC);
 Serial.print('\t');
 Serial.println(axis[2], DEC);
 }  // end loop

*/
