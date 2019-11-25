/*
  SFM button LED demos a full bi-directional Bluetooth Low
  Energy 4 connection between an phone app and a Sparky Simblee BOB
  or SimbLily.

  ... based on Example SFM/LedButton (SimbleeForMobile)

  The app's screen button can be used to turn the board's led on/off.
  The on/off state of hardware button on Simblee is sent to phone
  and shown in the app screen as changing box color.

  Uses: hardware button that wakes from sleep and (debounced) sends
  signal to app on phone, app & board button reads, delayUntilButton()
  sets options for sleep [off, fixed slow, custom], adjustable debounce
  timing, update draw rect, momentary app button
*/

#include <SimbleeForMobile.h>
// pin 2 on the spkfun BOB, 13 on SimbLily is the LED,
// (lite turns on/off from the iPhone app)
int led = 13;

// pin 3 on the spkfun BOB is button
// (button press to change display on the iPhone app)
int button = 11;  //  ground pin 11 on SimbLily to turn on

// debounce time (in ms)
int debounce_time = 10;

// maximum debounce timeout (in ms)
int debounce_timeout = 100;

void setup() {
  Serial.begin(9600);
  Serial.println("Started");

  // led turned on/off from the iPhone app
  pinMode(led, OUTPUT);

  // button press, effect is shown on the iPhone screen)
  pinMode(button, INPUT_PULLUP);  //unstable w/o pullup

  // this is the data to appear in the advertisement field
  // if len(deviceName + advertisementData) are > the 15 char allowed
  // in BLE adv packet, then the adv...Data is truncated first to
  // a single byte min. prn, then we truncate the deviceName to fit
  SimbleeForMobile.deviceName = "Sprk-Simb";
  SimbleeForMobile.advertisementData = "ledBtn"; // 6+9 should fit

  // use a shared cache
  // SimbleeForMobile.domain = "com.Simblee.sfm";
  SimbleeForMobile.begin();
}  // end setup

int debounce(boolean someButt)  // confirm button state lasts >10 ms
{ // loop code only checks debounce on LOW state, but would check HIGH
  // if that were the ON state to test; I don't debounce pinWake
  int start = millis();
  int debounce_start = start;

  // loop reads button until >100ms have passed, OR button==state for >10ms
  // orig had line alone, but I think needs code to check that button held
  while (millis() - start < debounce_timeout)
  {
    if (digitalRead(button) == someButt) // it's same value as sent to fx
    { // and > 10ms have passed
      if (millis() - debounce_start >= debounce_time)
        return 1;  // debounce validates state value sent, exits loop
    }
    else  // button value changed from value sent to fx, loop again
      debounce_start = millis();
  }  // end while, timeout loop
  return 0; // i.e. timed out without button holding state sent >10ms
}  // end debounce

int delay_until_button(int sumNum) // originally not called; my total
// recode sets various configs for pin/delay Wake: if param=0, pinWake
// on HIGH wakes when button unpressed and ULP delay = 0, so same as
// having no sleep code -- option never used unless code customized;
// if param = 1, pinWake on LOW, so if long (default) ULPdelay, button
// press gets immediate action on app, but if no new press, board reacts
// w/ long delay; if param is # > 1, pinWake's on LOW (same), delay = param 
{
  if (sumNum == 0) // if zero, same as no sleep code, fx() useless
  { Simblee_pinWake(button, HIGH); // wake on unpressed button
    Simblee_ULPDelay(sumNum); // custom config: if switch ON is HIGH, 
    // use INFINITE for ULPdelay--> would wake only on switch ON
  }
  else if (sumNum == 1)
  {
    Simblee_pinWake(button, LOW);  // wake on pressed
    // some default long delay -- use to mainly wake on some hardware
    // trigger from board (i.e. to request data or send an event)
    Simblee_ULPDelay(2500); // lo/hi # sets response speed for board
    // response to app, and rate of on-board loop tasks
  }
  else   // assume param is some positive number, use it for delay
  { Simblee_pinWake(button, LOW);  // wake on pressed butt;
    // custom delay -- affects responsiveness to phone app events,
    // still wakes immediately if hardware trigger from board
    Simblee_ULPDelay(sumNum);  // e.g. 100 still looks instantaneous
  }

  // do/while in example kept doing ULPdelay until button press; not
  // sure needed; just calling this in loop (or setup?) should do same,
  // switch to lower power mode until a button edge wakes us up
  //Simblee_ULPDelay(someNum);  // use param or code 2 customize sleep;
  // if INFINITE here, only pin will Wake
  //while (! debounce(digRead(button)));

  // if multiple buttons were configured,
  // was [this is how you would determine what woke you up], but I think
  // you'd need multiple if blox here to execute different actions,
  // which would show what had been pressed
  if (Simblee_pinWoke(button))
  {
    // execute code here
    Simblee_resetPinWake(button); // if this not here, won't stay asleep
    // ??? if ULP delay also invalid
  }
}   // end delay until button

int currentState, buttPress = 0; // both bools, only used in loop
uint8_t ui_button;
uint8_t ui_rect;

void loop() {

  // get events on the board, to send to app on phone if it's ready
  if (SimbleeForMobile.updatable)
  {
    // currentState = digitalRead(button);  // orig had no debounce
    // onboard button, OR some pin you can ground to turn ON
    // if d_u_b not 0, then button also wakes board from sleep
    buttPress = digitalRead(button); // false when pressed
    // it's pressed or something made it LOW
    // invert color on a valid quick button press; if held, invert 2x/sec
    // on valid press, debounce returns 1 & buttPress is 0
    if ((debounce(buttPress)) && !buttPress)
    { // toggle the color state on phone
      currentState = ! currentState;
      SimbleeForMobile.updateColor(ui_rect, currentState ? GREEN : WHITE);
      // previousState = currentState;
      delay(400);
    }
  } // end if updatable

  // .process must be called in the loop for SimbleeForMobile to work
  SimbleeForMobile.process();
  
  // v.s. fx() ops -- if called w/ 0 does nothing; do this here or setup ?
  delay_until_button(100); // 0 does nada, 1 long delay, # sets delay mS

}  // end loop

void SimbleeForMobile_onDisconnect()
{
  // don't leave the led on if they disconnect
  digitalWrite(led, LOW);
}

void ui()
{
  SimbleeForMobile.beginScreen(WHITE);

  SimbleeForMobile.drawText(60, 90, "Press and hold the button\n"
                            "here to turn on the board LED\n");

  // I prefer the button style below, uncomment the line after it for
  // the original button style -- screen button to toggle LED on board
  ui_button = SimbleeForMobile.drawButton(120, 180, 80, "Button");
  //SimbleeForMobile.drawButton(120, 180, 80, "Button", BLUE, TEXT_TYPE);

  // we need a momentary button (the default is a push button)
  SimbleeForMobile.setEvents(ui_button, EVENT_PRESS | EVENT_RELEASE);

  SimbleeForMobile.drawText(60, 300, "Press button on board\n"
                            "or ground (Lily) pin 11\n"
                            "to change color below.");

  // border
  SimbleeForMobile.drawRect(120, 400, 80, 80, BLACK);//outline

  // default state
  ui_rect = SimbleeForMobile.drawRect(122, 402, 76, 76, WHITE);

  SimbleeForMobile.endScreen();
}

void ui_event(event_t &event)
{ // events from the app, e.g. button press on screen
  if (event.id == ui_button) { // LED on only while pressed
    if (event.type == EVENT_PRESS)
      digitalWrite(led, HIGH);
    else if (event.type == EVENT_RELEASE)
      digitalWrite(led, LOW);
  }
}   // end ui event
