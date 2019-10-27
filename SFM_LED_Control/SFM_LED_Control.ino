/****************************************************************************
 * SFM LED_Control -- control LED on board from smart phone app
 
 Uses: txPowerLevel, color_t setup, drawButton, drawSwitch, setEvents
 
 Mike Hord @ SparkFun Electronics 26 Jan 2016
  https://github.com/sparkfun/Simblee_Tutorials
 * 
 * This example demonstrates the use of the SimbleeForMobile library to control
 * a pin on a Simblee module from a phone app. We show both a button and a
 * switch in this example.
 * 
 * Resources: Please install the Simblee support files before attempting to use
 * this sketch; see
 * https://learn.sparkfun.com/tutorials/simblee-concepts#setting-up-arduino for
 * details.
 * 
 * Development environment: Arduino IDE v1.6.7

 * ****************************************************************************/

// To use the SimbleeForMobile library, you must include this file at the top
// of your sketch. **DO NOT** include the SimbleeBLE.h file, as it will cause
// the library to break.
#include <SimbleeForMobile.h>

const int led = 2; // The Spark Simblee BOB (WRL-13632) has an LED on pin 2.
int ledState = LOW;

/* Every draw command returns a uint8_t value which is the object's id that was
  created. If you wish to modify or catch an event from the object later, you'll
  use this variable. I make these global id variables outside of any function,
  to use them later in various blox.
*/
uint8_t btnID;
uint8_t switchID;

void setup() 
{
  pinMode(led, OUTPUT);
  digitalWrite(led, ledState);
  
  // advertisementData shows up in the app as a line under deviceName. Note
  // that the length of these two fields combined must be less than 16
  // characters!
  SimbleeForMobile.deviceName = "SparkySimb";
  SimbleeForMobile.advertisementData = "Blink"; // 5+10=15 char works


  // txPowerLevel can be any multiple of 4 between -20 and +4, inclusive. The
  // default value is +4; at -20 range is only a few feet.
  SimbleeForMobile.txPowerLevel = -4;

  // This must be called *after* you've set up the variables above, as those
  // variables are only written during this function and changing them later
  // won't actually propagate the settings to the device.
  SimbleeForMobile.begin();
}   // end setup

void loop() 
{
  // This function must be called regularly to process UI events.
  SimbleeForMobile.process();
}  // end loop

// ui() is a SimbleeForMobile specific function which specifies
// the GUI on the mobile device the Simblee connects to.
void ui()
{
  // color_t is a special type which contains red, green, blue, and alpha 
  // (transparency) information packed into a 32-bit value. The functions rgb()
  // and rgba() can be used to create a packed value.
  color_t darkgray = rgb(85,85,85);

  // These variable names are long...let's shorten them. They allow us to make
  // an interface that scales and scoots appropriately regardless of the screen
  // orientation or resolution.
  uint16_t wid = SimbleeForMobile.screenWidth;
  uint16_t hgt = SimbleeForMobile.screenHeight;

  // The beginScreen() function both sets the background color and serves as a
  // notification that the host should try to cache the UI functions which come
  // between this call and the subsequent endScreen() call.
  SimbleeForMobile.beginScreen(darkgray);

  // Create a button slightly more than halfway down the screen, 100 pixels
  // wide, in the middle of the screen. The last two parameters are optional;
  // see the tutorial for more information about choices for them. The BOX_TYPE
  // button has a bounding box which is roughly 38 pixels high by whatever the
  // third parameter defines as the width.
  btnID = SimbleeForMobile.drawButton(
              (wid/2) - 75,          // x location
              (hgt/2) - 22,          // y location
              150,                   // width of button
              "Reverse LED",         // text shown on button
              WHITE,                 // color of button
              BOX_TYPE);             // type of button

  // Buttons, by default, produce only EVENT_PRESS type events. We want to also
  // do something when the user releases the button, so we need to invoke the
  // setEvents() function. Note that, even though EVENT_PRESS is default, we
  // need to include it in setEvents() to avoid accidentally disabling it.
  SimbleeForMobile.setEvents(btnID, EVENT_PRESS | EVENT_RELEASE);

  // Create a switch above the button. Note the lack of a title option; if you
  // want to label a switch, you'll need to create a textBox object separately.
  // A switch's bounding box is roughly 50 by 30 pixels.
  switchID = SimbleeForMobile.drawSwitch(
              (wid/2) - 25,          // x location
              (hgt/2)+22,            // y location
              BLUE);                 // color (optional)
  SimbleeForMobile.endScreen();
}   // end ui

// This function is called whenever a UI event occurs. Events are fairly easy
// to predict; for instance, touching a button produces a "PRESS_EVENT" event.
// UI elements have default event generation settings that match their expected
// behavior, so you'll only rarely have to change them.
void ui_event(event_t &event)
{
  // We created the btnID and switchID variables as globals, set them in the
  // ui() fx & use them here. Pressing app button turns on LED, release OFF

  if (event.id == btnID)
  {
    if (event.type == EVENT_PRESS)
    {
      if (ledState == HIGH) digitalWrite(led, LOW);
      else digitalWrite(led, HIGH);
    }    
    if (event.type == EVENT_RELEASE)
    {
      if (ledState == HIGH) digitalWrite(led, HIGH);
      else digitalWrite(led, LOW);
    }
  }

  // If the event was a switch press, we want to toggle the ledState variable
  // and then write it to the pin. Since LED starts off, it goes ON/OFF with
  // switch being on or off too
  if (event.id == switchID)
  {
    if (ledState == HIGH) ledState = LOW;
    else ledState = HIGH;
    digitalWrite(led, ledState);
  }
}  // end ui_event

