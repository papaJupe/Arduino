void setup() 
{
  // setup code run once:
pinMode(8, OUTPUT);
}

void loop() 
  // put your main code here, to run repeatedly:
 {
      for (byte i = 0; i <= 4; i++) // beep tone every 20 sec
//      { // passive beeper uses this
//        tone(8, 2800);   // don't need pin config for toner
//        delay(500);
//        noTone(8);
//        delay(200);
//      } // end tone loop

      {  // active piezo beeper just needs voltage
        digitalWrite(8, HIGH);   //  need pin config for digiWrite to work
        delay(500);
        digitalWrite(8, LOW);
        delay(200);
      } // end tone loop
      delay (3000); 
    
} // end loop
