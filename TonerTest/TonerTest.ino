void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
 {
      for (byte i = 0; i <= 4; i++) // beep tone every 20 sec
      {
        tone(8, 2800);   // don't need pin config for toner
        delay(500);
        noTone(8);
        delay(200);
      } // end tone loop
      delay (5000);   // 2 + 18 = 20 sec between plays
    }
}
