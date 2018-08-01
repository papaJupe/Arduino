/*
 * SimbComBidir example edited AM
 * hard to follow this. presume 2 mods using same code connect, 
 * each have global var ch. Sender button press wakes mod, sends stuff, what?
 */

#include "SimbleeCOM.h"

int packets = 0;
int errors = 0;

char ch = 0;

void setup()
{
  Serial.begin(9600);
  
  pinMode(5, INPUT);
  pinMode(6, INPUT);

  Simblee_pinWake(5, HIGH);
  Simblee_pinWake(6, HIGH);
  
  SimbleeCOM.begin();
}

void loop()
{
  if (Simblee_pinWoke(5))
  {
    printf("starting\n");  // each wake, global ch is set back to 0, sent
    ch = 0;   // to other, set to 1, then mod sleeps again
    SimbleeCOM.send(&ch, sizeof(ch));
    ch++;
    delay(250);  // debounce
    Simblee_resetPinWake(5);
  }
  
  if (Simblee_pinWoke(6))
  {    // each wake prints vars set in __onRecv fx, then resets 
    // all globals to 0 and sleeps
    printf("Results: packets=%d, errors=%d\n", packets, errors);
    packets = 0;
    errors = 0;
    ch = 0;
    delay(250);  // debounce
    Simblee_resetPinWake(6);
  }
}
    // 1 ch sent back from other module, which using same code starts w/ 0
    // 
void SimbleeCOM_onReceive(unsigned int esn, const char *payload, int len, int rssi)
{
  if (payload[0] != ch)  // there's only one ch sent back, so this is it.
    //  orig 0, then on sending mod it's 1 after a send,
  {  // on receive mod, this if code forces the recv global to same incoming value
    // and augments it one to hopefully match the augmented one on sender
    errors++;
    ch = payload[0];
  }
  packets++;
  ch++;  // receiver augments what it got in, sends back, then augments again
  SimbleeCOM.send(&ch, sizeof(ch));
  ch++;   // nothing to stop send/recv loop except pin 6 wakeup on one or the other
}
