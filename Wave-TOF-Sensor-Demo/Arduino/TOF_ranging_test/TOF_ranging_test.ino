/*
  TOF ranging test

  Receives from serial port 1, sends to the main serial (Serial 0).

  This example works only with boards with more than one serial like Arduino Mega, Due, Zero etc.

  The circuit:
  - TOF Laser Range Sensor attached to Serial port 1
  - Serial Monitor open on Serial port 0

  created 27 July 2020
  by kaloha [Waveshare] https://www.waveshare.com/

  This example code is in the public domain.
*/
#include <SoftwareSerial.h>
SoftwareSerial Serial1(11,10); //Serial1 connected to TOF Serial：10<-->RX, 11<-->TX

unsigned char TOF_data[32] = {0};   //store 2 TOF frames
unsigned char TOF_length = 16;
unsigned char TOF_header[3] {0x57,0x00,0xFF};
unsigned long TOF_system_time = 0;
unsigned long TOF_distance = 0;
unsigned char TOF_status = 0;
unsigned int TOF_signal = 0;
unsigned char TOF_check = 0;

void setup() {
  // initialize both serial ports:
  Serial.begin(115200);
  Serial1.begin(115200);
}

bool verifyCheckSum(unsigned char data[], unsigned char len){
  TOF_check = 0;

  for(int k=0;k<len-1;k++)
  {
      TOF_check += data[k];
  }

  if(TOF_check == data[len-1])
  {
      Serial.println("TOF data is ok!");
      return true;    
  }else{
      Serial.println("TOF data is error!");
      return false;  
  }

}

void loop() {
  // read from port 1:
  if (Serial1.available()>=32) {
     for(int i=0;i<32;i++)
     {
       TOF_data[i] = Serial1.read();
     }
  
    for(int j=0;j<16;j++)
    {
      if( (TOF_data[j]==TOF_header[0] && TOF_data[j+1]==TOF_header[1] && TOF_data[j+2]==TOF_header[2]) && (verifyCheckSum(&TOF_data[j],TOF_length)))
      {
        if(((TOF_data[j+12]) | (TOF_data[j+13]<<8) )==0)
        {
           Serial.println("Out of range!");
         }else{
           Serial.print("TOF id is: ");
           Serial.println(TOF_data[j+3],DEC);
    
           TOF_system_time = TOF_data[j+4] | TOF_data[j+5]<<8 | TOF_data[j+6]<<16 | TOF_data[j+7]<<24;
           Serial.print("TOF system time is: ");
           Serial.print(TOF_system_time,DEC);
           Serial.println("ms");
    
           TOF_distance = (TOF_data[j+8]) | (TOF_data[j+9]<<8) | (TOF_data[j+10]<<16);
           Serial.print("TOF distance is: ");
           Serial.print(TOF_distance,DEC);
           Serial.println("mm");
    
           TOF_status = TOF_data[j+11];
           Serial.print("TOF status is: ");
           Serial.println(TOF_status ,DEC);
    
           TOF_signal = TOF_data[j+12] | TOF_data[j+13]<<8;
           Serial.print("TOF signal is: ");
           Serial.println(TOF_signal ,DEC);
    
           Serial.println("");
         
        }
        break;
      }
    }

  }


}
