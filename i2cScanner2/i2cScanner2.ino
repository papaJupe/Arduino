// --------------------------------------
// i2c_scanner2 put scan loop in setup, nothing in loop (commented out)

// This sketch tests the standard 7-bit addresses
// Devices with higher bit address might not be seen properly.
//

#include <Wire.h>
byte error, address;


void setup()
{
  pinMode(A2,OUTPUT); // for convenience I use anal pins to power device
  pinMode(A3,OUTPUT);   // A4-5 are SDA and SCL
  digitalWrite(A2,LOW);  // grnd
  digitalWrite(A3,HIGH);  // '5' v
  delay(100);
  
  Wire.begin();

  Serial.begin(38400);  // could be anything
 
  Serial.println("\nI2C Scanner2  -- scanning\n");

  for (address = 1; address < 60; address++ )  // could go to 128+
  {
    Wire.beginTransmission(address);  // at the address
    Wire.write(0x00);            // we want to read this register
    
    if (Wire.endTransmission()==0) // sends pointer byte, if ret = 0, got ACK
      Serial.println("Device at " + String(address) + " responds w/ ACK");
 
    else
      { 
        Serial.println("no reply from addr " + String(address));
      }
       delay (5);
   }   // end for

    //   while(Wire.available())    // slave may send less (more?) than requested
    //     {
    //     byte b = Wire.read();    // receive a byte as byte, .print shows its dec #
    //     Serial.println("got back " + String(b));  // .print shows the byte as #
    //     }


  } // end setup


  void loop()
  {
    /*  simplified version of this put in setup above
    Serial.println("Scanning...");

    for (address = 29; address < 60; address++ )
    {
      // The i2c_scanner uses the return value of
      // the Write.endTransmission() fx to see if
      // a device did acknowledge the address.
      Wire.beginTransmission(address);
      Wire.write(0x00);  // just some data to write
      error = Wire.endTransmission();  // sends address w/ 0 then 1 @ end, gets ACK or not
      //delay(15);
      if (error == 0) // gets this only if both addr and data got ACK
      {
        Serial.print("I2C device found at address 0x");
        if (address < 16)
          Serial.print("0"); // 1st 15 need a 0 in front
        Serial.print(address, HEX);
        Serial.println("  !");

        nDevices++;
      }
      else if (error == 4)
      {
        Serial.print("Unknown error at address 0x");
        if (address < 16)
          Serial.print("0");  // 0 before hex #
        Serial.println(address, HEX);

      }
      else
      {
        Serial.print("got error at address 0x");
        if (address < 16)
          Serial.print("0");  // 0 before hex #
        Serial.print(address, HEX); Serial.print("  ");
        Serial.println(error);

      }
      // delay(200);
    }   //end for
    if (nDevices == 0)
      Serial.println("No I2C devices found");
    else
      Serial.println("done");
    delay(500);  // give time to print
    exit(1); // stops loop or could
    // delay(5000);        //  to wait 5 seconds for next scan
    */
  }  // end loop

