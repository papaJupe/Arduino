void setup() 
{
  // setup code run once:
//pinMode(3, OUTPUT); // if active beeper
}

void loop() 
  // put your main code here, to run repeatedly:
 {
      for (byte i = 0; i <= 4; i++) // beep tone series
     { // passive piezo need sq. wave ac
       tone(3, 15000);   // don't need pin config for toner
       delay(500);
       noTone(3);
       delay(200);
     } // end tone loop

      // {  // active piezo beeper just needs voltage
      //   digitalWrite(8, HIGH);   //  need pin config for digiWrite to work
      //   delay(500);
      //   digitalWrite(8, LOW);
      //   delay(200);
      // } // end tone loop
      delay (1000); 
    
} // end loop
