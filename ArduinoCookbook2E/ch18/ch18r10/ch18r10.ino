#include <Narcoleptic.h>

void setup() {
  pinMode(2,INPUT);
  digitalWrite(2,HIGH);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
}

void loop() {
  int a;

  // Merlin the cat is snoozing... Connect digital pin 2 to ground to wake him up.
  Narcoleptic.delay(500); // During this time power consumption is minimized

  while (digitalRead(2) == LOW) {
    // Wake up CPU. Unfortunately, Merlin does not like waking up.

    // Swipe claws left
    digitalWrite(13,HIGH);
    delay(50);

    // Swipe claws right
    digitalWrite(13,LOW);
    delay(50);
  }

  // Merlin the cat goes to sleep...
}
      
    
