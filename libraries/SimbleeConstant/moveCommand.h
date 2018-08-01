void drive( int velocity, int radius) {
    byte cmd[] = { 
      (byte)DRIVE, (byte)(velocity>>8 & 0xff), (byte)(velocity & 0xff), 
      (byte)(radius >> 8 & 0xff), (byte)(radius & 0xff)
    };
 // Serial.write( cmd, 5 ); // worked as well as loop
   for (int i = 0; i < sizeof(cmd); i++)
    {
       Serial.write(cmd[i]);
       delay(2);
       //Serial.print(" ");
      }     // prints bytes as number to Ser Mon
      //Serial.println();

  }