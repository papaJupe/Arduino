/*
 ITG-3200 example sketch
 Based on the SparkFun quick start guide: http://www.sparkfun.com/tutorials/265 
 */
#include <Wire.h>

const int itgAddress = 0x69;

// ITG-3200 constants - see datasheet
const byte SMPLRT_DIV= 0x15;
const byte DLPF_FS   = 0x16;
const byte INT_CFG   = 0x17;
const byte PWR_MGM   = 0x3E;
const byte GYRO_X_ADDRESS = 0x1D; // GYRO_XOUT_H
const byte GYRO_Y_ADDRESS = 0x1F; // GYRO_YOUT_H
const byte GYRO_Z_ADDRESS = 0x21; // GYRO_ZOUT_H

// Configuration settings, see datasheet for details
const byte DLPF_CFG_0    = 0x1;
const byte DLPF_CFG_1    = 0x2;
const byte DLPF_CFG_2    = 0x4;
const byte DLPF_FS_SEL_0 = 0x8;
const byte DLPF_FS_SEL_1 = 0x10;

void setup()
{
  Serial.begin(9600);
  Wire.begin();

  //Configure the gyroscope
  //Set the gyroscope scale for the outputs to +/-2000 degrees per second
  itgWrite(DLPF_FS, (DLPF_FS_SEL_0|DLPF_FS_SEL_1|DLPF_CFG_0));
  //Set the sample rate to 100 hz
  itgWrite(SMPLRT_DIV, 9);
}

//read and output X,Y and Z rates to Serial Monitor
void loop()
{
  //Create variables to hold the output rates.
  int xRate, yRate, zRate;

  //Read the x,y and z output rates from the gyroscope.
  xRate = readAxis(GYRO_X_ADDRESS);
  yRate = readAxis(GYRO_Y_ADDRESS);
  zRate = readAxis(GYRO_Z_ADDRESS);

  //Print the output rates to the Serial Monitor
  int temperature = 22;
  Serial.print(temperature);
  Serial.print(',');  
  Serial.print(xRate);
  Serial.print(',');
  Serial.print(yRate);
  Serial.print(',');
  Serial.println(zRate);  

  //Wait 10ms before reading the values again. 
  delay(10);
}

//Write the given data to the given itg-3200 register
void itgWrite(char registerAddress, char data)
{

  Wire.beginTransmission(itgAddress); // initiate the send sequence
  Wire.write(registerAddress);        // the register address to write
  Wire.write(data);                   // the data to be written
  Wire.endTransmission();             // this actually sends the data
}

//Read data from the specified register on the ITG-3200 and return the value.
unsigned char itgRead(char registerAddress)
{
  //This variable will hold the contents read from the i2c device.
  unsigned char data=0;

  Wire.beginTransmission(itgAddress);
  Wire.write(registerAddress); //Send the Register Address
  Wire.endTransmission();      //End the communication sequence.

  Wire.beginTransmission(itgAddress); 
  Wire.requestFrom(itgAddress, 1);    //Ask the device for data

  if(Wire.available()){  // Wait for a response from device
    data = Wire.read();  // read the data
  }

  Wire.endTransmission();   //End the communication sequence
  return data;   //Return the read data
}

// Read X,Y or Z Axis rate of the gyroscope.
// axisRegAddress argument selects the axis to be read.
int readAxis(byte axisRegAddress)
{
  int data=0;
  data = itgRead(axisRegAddress)<<8;
  data |= itgRead(axisRegAddress + 1);  
  return data;
}