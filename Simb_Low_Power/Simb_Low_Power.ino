/*************************************************************
   Sibl Low Power example: uses SFM lib but just does local, board
   actions, nothing sent or recvd from mobile app. Some delay code
   imported from SFM buttonLEDdemo

   Uses: SFM.txPowerLevel, various pinWake callbacks, ULPdelay,
   pinWakeReset, delay until button,

   Orig: 29 Jan 2016 https://github.com/sparkfun/Simblee_Tutorials

   Development environment: Arduino.cc IDE v1.6.7
   edit AM 180302 for Spark Simblee BOB
 * *************************************************************/

// To use the SimbleeForMobile library, you must include this file at top
// of your sketch. **DO NOT** include the SimbleeBLE.h file.
#include <SimbleeForMobile.h>

const byte led = 13; // sparky BOB (WRL-13632) LED on pin 2, Lily is 13
const int button = 3;  // BOB button is wired to D3, on Lily I use wire
const int callbutt = 9; // pin 9 wakes & runs a specific fx, 3 blinx

void setup()
{
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  pinMode(button, INPUT_PULLUP);
  pinMode(callbutt, INPUT_PULLUP);
  Serial.begin(9600);
  // advertisementData shows in the app as a line under deviceName. Note
  // that the length of these TWO fields combined must be < 16 chars!
  SimbleeForMobile.deviceName = "SprkSimbl";  // 9+6=15 OK
  SimbleeForMobile.advertisementData = "LowPow";

  // txPowerLevel can be any multiple of 4 between -20 and +4, inclusive. The
  // default value is +4; at -20 range is only a few feet.
  SimbleeForMobile.txPowerLevel = -4;
  // wake setup for button is in loop but for callbutt I set here
  Simblee_pinWakeCallback(callbutt, LOW, buttonCallback); // 3 blinx
  // This must be called *after* you've set up the variables above; setup
  // values are only written by this function.
  SimbleeForMobile.begin();
}  // end setup

// I believe static pinWake vars should be called in setup but
// this fx() also configs ULPdelay which must be inside loop I think
int delay_until_button(int sumNum) // copy from buttonLEDemo; call
// from setup/loop() sets options for pin/delay Wake: if param=0, pinWake
// on HIGH wakes when button unpressed and ULP delay = 0, so same as
// having no sleep code -- option never used unless code customized;
// if param = 1, pinWake on LOW, so if long (default) ULPdelay, button
// press gets immediate action on app, but if no new press, board reacts
// w/ delay; if param is # > 1, pinWake on LOW (same), delay = param
{ // does not look at callbutt until pinWoke() -- will it detect?
  if (sumNum == 0) // if zero, same as no sleep code, unless customized
  { Simblee_pinWake(button, HIGH); // wake on unpressed button
    Simblee_ULPDelay(sumNum); // custom config: if switch ON is HIGH,
    // use INFINITE for ULPdelay--> would wake only on switch ON
  }
  else if (sumNum == 1)
  {
    Simblee_pinWake(button, LOW);  // wake on pressed
    // some default long delay -- use to mainly wake on some hardware
    // trigger from board (i.e. to request data or send an event)
    Simblee_ULPDelay(2500); // lo/hi # sets response speed for app-board
    // comm and rate of on-board loop tasks
  }
  else   // assume param is some positive number, use it for delay
  { Simblee_pinWake(button, LOW);  // wake on pressed
    // custom delay -- affects responsiveness to phone app events,
    // still wakes immediately if hardware trigger from board
    Simblee_ULPDelay(sumNum);  // e.g. 100 still looks instantaneous
  }

  // if multiple buttons were configured,
  // was [this is how you would determine what woke you up], but I think
  // you'd need multiple if blox here to execute different actions,
  // which would show what had been pressed
  if (Simblee_pinWoke(button)) // true when button N was the trigger
  {
    // execute code here
    Serial.printf("woke by button %d \n", button);
    Simblee_resetPinWake(button); // if this not here, won't stay asleep
    // ??? if ULP delay also invalid if not here
  }
}   // end delay until button

void loop()
{
  // There are 2 low power modes: Ultra Low Power and Off. ULP consumes
  // ~1/10 of the power that a fully-on Simblee consumes; in Off mode,
  // the board uses about 1/5000 of the normal power!

  // --- if used statically I would put pinWake configs in setup()---

  // The system can be brought out of ULP mode either on a timed basis v.
  // (tutorial?) for more information; broadly, pass a Number of mS to
  // the Simblee_ULPDelay() function and the processor will awaken N
  // milliseconds later) OR by a pin transition event. In Off mode, only
  // a pin event can wake the processor!

  // Simblee_pinWakeCallback() executes the passed function when a pin
  // event causes the board to leave either sleep mode. Options for the
  // second parameter are HIGH, LOW, or NONE, and specify the desired
  // triggering state of the pin. Don't use if loop calls delayUntilPin()
  // Simblee_pinWakeCallback(callbutt, LOW, buttonCallback);

  // Simblee_pinWake() sets up the device to awaken from sleep mode, but
  // does NOT specify a callback function. Following this pin wake from
  // ULP mode, execution will resume on the line AFTER the function call
  // to Simblee_ULPDelay() (immediate I think ?); in Off mode, the
  // processor will immediately revert to sleep mode unless a callback
  // function is available. [v.i. I guess this means you must use
  // Simblee_pinWakeCallback(....) following Off mode.

  // Simblee_systemOff() puts the device in a very low power mode, where
  // it consumes less than 1uA of current (compared to about 5000uA
  // during normal operation). In this mode, *only* a pin transition can
  // bring the device out of sleep, and you *must* provide a callback
  // function with Simblee_pinWakeCallback() for any code to be executed!
  // Simblee_systemOff();

  // Simblee_ULPDelay() causes the processor to sleep for some number of
  // milliseconds. There are utility macros defined for converting from
  // larger more convenient units: DAY(x), HOURS(x), MINUTES(x),
  // SECONDS(x) will each cause the processor to wait for x of that
  // number of units. Pass INFINITE to delay until a pin event occurs.
  // Simblee_ULPDelay(INFINITE);
  // Simblee_ULPDelay(5000);  // button press still exits ULP

  // If a pin event woke the processor, you MUST call the
  // Simblee_resetPinWake() function for that pin, or the processor will
  // immediately exit sleep mode upon reentering it. Doesn't affect
  // behavior from ULPdelay wakeup ? which ? resets this by itself --
  // but if delay is infinite and I reset pin here to be active, I never
  // see blink except from button --> callback
  //Simblee_resetPinWake(button);  // no effect if I put some # of mS in
  // ULPdelay -- to test this I could make a long ULPdelay and see
  // if it sleeps then wakes w/ pin; don't use if loop calls d_u_b

  // loop blinks the LED x2 & longer than in the callback fx, so
  // we can differentiate between loop and callback execution x3
  Serial.println("loop will flash x 2");
  digitalWrite(led, HIGH);  // does not flash when ULPdelay=infinite
  delay(1500);
  digitalWrite(led, LOW);
  delay(1000);
  digitalWrite(led, HIGH);
  delay(1500);
  digitalWrite(led, LOW);
  delay_until_button(1); // sets button to wake, ULPdelay 2500

  // This function must be called regularly to process UI events.
  SimbleeForMobile.process();
}   // end loop

// ui() is a SimbleeForMobile specific function which specifies
// the GUI on the mobile device the Simblee connects to.
void ui()
{
  // Empty for this sketch.
}

// This function is called whenever a UI event occurs. Events are clearly
// named; for instance, touching a button produces a "PRESS_EVENT" event.
// UI elements have default event generation settings that match their
// expected behavior, so you'll only rarely have to change them.
void ui_event(event_t &event)
{
  // Empty for this sketch.
}

// This fx (the name of which is completely arbitrary) may be called
// from a pinWake event that pulls the processor out of low power mode.
// Here, I just blink the LED x3 for 500ms, to show the callback action
int buttonCallback(uint32_t button)
{
  Serial.printf("woke on callbak from pin %d \n", button);
  digitalWrite(led, HIGH);
  delay(500);
  digitalWrite(led, LOW);
  delay(500);
  digitalWrite(led, HIGH);
  delay(500);
  digitalWrite(led, LOW);
  delay(500);
  digitalWrite(led, HIGH);
  delay(500);
  digitalWrite(led, LOW);
  delay(500);
  Simblee_resetPinWake(button); // goes back to next loop when done here
  return(0);  // even if I ret 0, ULPdelay is exited
}

