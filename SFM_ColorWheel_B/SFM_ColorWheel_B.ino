

/* SFM Color Wheel B -- adapted from SFM/Color Wheel example
  version B added stepper control, sending bytes to other device

  Code here is uploaded to Simblee board that can be freestanding w/ (batt
  power) or USB powered to keep Serial comm to Ardu's IDE Ser Mon for
  debugging or whatever. Board's Serial could also comm to / from
  another serial device like Mega (v.i.).
  For wireless comm to whatever Simblee's Serial is connected to -- Simblee
  phone app (iOS / Android) makes BLE connection to the Simblee, with the
  app displaying the user interface created here. For this sketch:

  Sliders on phone app change RGB byte values that could be used by
  Simblee to vary its onboard lites, or sent over Serial to print to
  IDE Ser Mon, or if attached to a Mega's Ser2, could vary color of
  LEDs or Neopixels. Color wheel object is pre-loaded rainbow circle
  in app, that sends the RGB color value you touch (both iOS and Android).

  Author claims ' sketch will maintain the color selected when disconnected.'
  -- probably depends on caching being enabled and not updating in ui()

  Uses: deviceName, advertData, drawText, drawTextField, drawImage,
  drawSlider, Serial.printf, screenWidth, update(), color_wheel, setEvent
  for color press, volatile var (changed in event thread),
  serial.write(byte array,len)
*/

#include <SimbleeForMobile.h>

// I don't have RGB shield so I use 1 blue lite on SimbLily or BOB
int led1 = 13;  // 13 on SimbLily, 2 on Spark BOB

//// pin 3 on the RGB shield is the green led
//int led2 = 3;
//// pin 4 on the RGB shield is the blue led
//int led3 = 4;

// current color (start with white)
// update() uses these to init screen objects, when 1st called @ end of ui()
uint8_t red = 255;
uint8_t green = 128;
uint8_t blue = 255;
uint8_t brite = 128; // global brightness goes to value of stepper and field

volatile bool needsUpdate;  // volatile -- depends on event input
uint8_t eventId;

void update();

void setup() {
  Serial.begin(57600);
  Serial.println("Colorwheel Started");

  // setup the leds for output
  pinMode(led1, OUTPUT);
  //  pinMode(led2, OUTPUT);
  //  pinMode(led3, OUTPUT);

  // set initial color
  analogWrite(led1, red);
  //  analogWrite(led2, green);
  //  analogWrite(led3, blue);

  // this is the data that appears in the advertisement for device
  // (if the deviceName and advertisementData are too long to fit the 15 byte
  // BLE advert packet allowance, then the advertData is truncated from end,
  // down to a single byte, then it truncates the deviceName)
  SimbleeForMobile.deviceName = "SimbColor";
  SimbleeForMobile.advertisementData = "rgb123"; // name 9 + ad 6 = 15 allowed

  // use a shared cache
  // SimbleeForMobile.domain = "Simblee.com";

  SimbleeForMobile.begin();
}   // end setup

void loop() {  // loop farms out all action to update(), why ? speed

  if (needsUpdate) {
    update();
    needsUpdate = false;
  }

  // SFM.process must be called in the loop
  SimbleeForMobile.process();
  delay(50);
}   // end loop

uint8_t rtextfield;
uint8_t rslider;
uint8_t gtextfield;
uint8_t gslider;
uint8_t btextfield;
uint8_t bslider;
uint8_t swatch;
uint8_t britestep;  // stepper control sets brite var
uint8_t britefield; // value field displays brite var
uint8_t color_wheel;

void ui() // ui() may just set up the initial graphics, no role in
// updating values. Screen params print once @ first update event.
{ // where could printf print ? C editor console ?
  // printf("UI screen size: %dx%d", SimbleeForMobile.screenWidth, 
  // SimbleeForMobile.screenHeight); --> same 320 x 568 in Andr/iOS
  Serial.printf("ScreenW: %d \n", SimbleeForMobile.screenWidth);
  Serial.printf("ScreenH: %d \n", SimbleeForMobile.screenHeight);

  color_t darkgray = rgb(85, 85, 85);  // screen bkgnd

  SimbleeForMobile.beginScreen(darkgray);

  // put controls at top, so the keyboard (never used) doesn't cover them ?
  // drwTexFld's 4th param for value is later overwritten by update(), but
  // may be needed (int vs. str) to configure field as Text or Numeric

  SimbleeForMobile.drawText(25, 71, "R:", WHITE);
  rslider = SimbleeForMobile.drawSlider(55, 65, 175, 0, 255);
  rtextfield = SimbleeForMobile.drawTextField(245, 65, 50, red, "",WHITE, darkgray);

  SimbleeForMobile.drawText(25, 116, "G:", WHITE);
  gslider = SimbleeForMobile.drawSlider(55, 110, 175, 0, 255);
  gtextfield = SimbleeForMobile.drawTextField(245, 110, 50, 222, "",WHITE, darkgray);

  SimbleeForMobile.drawText(25, 161, "B:", WHITE);
  bslider = SimbleeForMobile.drawSlider(55, 155, 175, 0, 255);
  btextfield = SimbleeForMobile.drawTextField(245, 155, 50, 333, "",WHITE, darkgray);

  // SimbleeForMobile.drawText(10, 204, "*", WHITE);
  britestep = SimbleeForMobile.drawStepper(25, 202, 80, 0, 255, WHITE);
  britefield = SimbleeForMobile.drawTextField(120,202,55,brite,"#", WHITE, darkgray);

  // border and swatch to display active color, much narrower
  SimbleeForMobile.drawRect(182, 200, 126, 40, WHITE);
  swatch = SimbleeForMobile.drawRect(183, 201, 124, 38, WHITE);

  color_wheel = SimbleeForMobile.drawImage(COLOR_WHEEL, 10, 250);

  SimbleeForMobile.setEvents(color_wheel, EVENT_COLOR);

  SimbleeForMobile.endScreen();

  SimbleeForMobile.updateValue(britestep, brite); // init the step control value

  // populate with the above red/green/blue values -- i.e. white
  // (this must be done after endScreen() to force white on startup,
  // otherwise the initial values will fill from the cache)

  update();
}  // end ui

void update() // loop calls this fx, when loop sees that ui_event
// thread has set needsUpdate to true -- (tho actions here could
// have been done in loop). Instead this fx does all changes
{ // both to the board hardware and display on the phone app
  // so will also send bytes from here
  analogWrite(led1, red);  // write current red level to my 1 onboard LED
  //  analogWrite(led2, green);
  //  analogWrite(led3, blue);

  // on 1st call, there's no eventId, so this inits all values to defaults
  // if event is from color_wheel tap, send those values to sliders
  if (eventId != rslider) SimbleeForMobile.updateValue(rslider, red);
  if (eventId != rtextfield) SimbleeForMobile.updateValue(rtextfield, red);

  if (eventId != gslider) SimbleeForMobile.updateValue(gslider, green);
  if (eventId != gtextfield) SimbleeForMobile.updateValue(gtextfield, green);

  if (eventId != bslider) SimbleeForMobile.updateValue(bslider, blue);
  if (eventId != btextfield) SimbleeForMobile.updateValue(btextfield, blue);
  // refresh color on screen swatch with current values
  SimbleeForMobile.updateColor(swatch, rgb(red, green, blue));

  //print current RGB to Ser Mon, vals display as ints
  //Serial.printf("R %d  G %d  B %d * %d\n", red, green, blue, brite);

  // for Mega, make byte array, Serial.write(array,len) w/ newline
  byte cmd[] = {byte(red), byte(green), byte(blue), byte(brite), 10};
  //Serial.write(cmd, 5); // fine for sending bytes
  // prints bytes as number to Ser Mon
  for (int i = 0; i < sizeof(cmd); i++)
  { Serial.print(cmd[i]);  
    Serial.print(" ");
  }
  Serial.println();
  
  SimbleeForMobile.updateValue(britefield, brite);

}  // end update

void ui_event(event_t &event)
{ // just resets vars according to event ID; kept
  // small for speed --> farm out reaction to loop & update()
  eventId = event.id; // used in update() 4 various uses of color vals

  if (event.id == color_wheel) // RGB of color tapped sets values
  {
    red = event.red;
    green = event.green;
    blue = event.blue;
  }
  else if (event.id == rslider || event.id == rtextfield)
    red = event.value;
  else if (event.id == gslider || event.id == gtextfield)
    green = event.value;
  else if (event.id == bslider || event.id == btextfield)
    blue = event.value;
  else if (event.id == britestep)
    brite = event.value;

  needsUpdate = true;  // this value read by loop, calls update()
} // end ui_event
