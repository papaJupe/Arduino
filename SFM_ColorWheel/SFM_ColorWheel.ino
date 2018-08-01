/* SFM Color Wheel -- adapted from SFM/Color Wheel example
  this 1st version just made example work on SimbLily
  Code here is uploaded to Simblee board that can be freestanding w/ (batt 
  power) or USB powered to keep Serial comm to Ardu's IDE Ser Mon for
  debugging or whatever. Board's Serial could also comm to / from 
  another serial device like Mega (v.i.).
  For wireless comm to whatever Simblee's Serial is connected to -- Simblee
  phone app (iOS / Android) makes BLE connection to the Simblee, with the
  app displaying the user interface created here. For this sketch:
  
  Sliders on phone app change RGB byte values that could be used by
  SimBoard to vary its onboard lites, or sent over Serial to print to
  IDE Ser Mon, or if attached to a (Mega's Ser2), could vary colors of
  LEDs or Neopixels. Color wheel object is pre-loaded rainbow circle
  in app, that sends the RGB color value you touch (both iOS and Android).
  
  Author claims ' sketch will maintain the color selected when disconnected.'
  -- probably depends on caching being enabled and not updating in ui()

  Uses: deviceName, advertData, drawText, drawTextField, drawImage,
  drawSlider, Serial.printf, screenWidth, update(), color_wheel.
  volatile var (changed in event thread)
*/


#include <SimbleeForMobile.h>

// I don't have RGB shield so I use 1 blue lite on SimbLily or BOB
int led1 = 13;  // 13 on SimbLily, 2 on Spark BOB

//// pin 3 on the RGB shield is the green led
//int led2 = 3;
//// pin 4 on the RGB shield is the blue led
//int led3 = 4;

// current color (start with white)
uint8_t red = 255;
uint8_t green = 255;
uint8_t blue = 255;

volatile bool needsUpdate;  // volatile -- depends on event input
uint8_t eventId;

void update();

void setup() {
  Serial.begin(9600);
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
}

void loop() {  // loop farms out all real action to update(), why?
  // switch to lower power mode
  // todo: Simblee_ULPDelay(INFINITE);

  if (needsUpdate) {
    update();
    needsUpdate = false;
  }

  // .process must be called in the loop for SimbleeForMobile
  SimbleeForMobile.process();

}   // end loop

uint8_t rtextfield;
uint8_t rslider;
uint8_t gtextfield;
uint8_t gslider;
uint8_t btextfield;
uint8_t bslider;
uint8_t swatch;
uint8_t color_wheel;

void ui() // this may just set up the initial graphics, no role in
// updating values ?
{ // where is this supposed to print? C editor console ?
  // printf("UI screen size: %dx%d", SimbleeForMobile.screenWidth, SimbleeForMobile.screenHeight);
  Serial.printf("ScreenW: %d \n", SimbleeForMobile.screenWidth);
  Serial.printf("ScreenH: %d \n", SimbleeForMobile.screenHeight);

  color_t darkgray = rgb(85, 85, 85);

  SimbleeForMobile.beginScreen(darkgray);

  // put controls at top, so the keyboard (never used) doesn't cover them ?

  SimbleeForMobile.drawText(25, 71, "R:", WHITE);
  rslider = SimbleeForMobile.drawSlider(55, 65, 175, 0, 255);
  rtextfield = SimbleeForMobile.drawTextField(245, 65, 50, 255, "", WHITE, darkgray);

  SimbleeForMobile.drawText(25, 116, "G:", WHITE);
  gslider = SimbleeForMobile.drawSlider(55, 110, 175, 0, 255);
  gtextfield = SimbleeForMobile.drawTextField(245, 110, 50, 255, "", WHITE, darkgray);

  SimbleeForMobile.drawText(25, 161, "B:", WHITE);
  bslider = SimbleeForMobile.drawSlider(55, 155, 175, 0, 255);
  btextfield = SimbleeForMobile.drawTextField(245, 155, 50, 255, "", WHITE, darkgray);

  // border and swatch to display active color set
  SimbleeForMobile.drawRect(25, 200, 270, 40, WHITE);
  swatch = SimbleeForMobile.drawRect(26, 201, 268, 38, WHITE);

  color_wheel = SimbleeForMobile.drawImage(COLOR_WHEEL, 10, 250);

  SimbleeForMobile.setEvents(color_wheel, EVENT_COLOR);

  SimbleeForMobile.endScreen();

  // populate with the above red/green/blue values -- i.e. white
  // (this must be done after endScreen() to force white on startup,
  // otherwise the initial values will fill from the cache)

  update();
}  // end ui

void update() // loop calls this fx, when loop sees that ui_event
// thread has set needsUpdate to true -- (tho update() actions could
// have been done in loop). Instead this fx does all changes
{ // both to the board hardware and display on the phone app

  analogWrite(led1, red);  // write current red level to my 1 LED
  //  analogWrite(led2, green);
  //  analogWrite(led3, blue);

  // if event is from color_wheel tap, send those values to sliders
  if (eventId != rslider) SimbleeForMobile.updateValue(rslider, red);
  if (eventId != rtextfield) SimbleeForMobile.updateValue(rtextfield, red);

  if (eventId != gslider) SimbleeForMobile.updateValue(gslider, green);
  if (eventId != gtextfield) SimbleeForMobile.updateValue(gtextfield, green);

  if (eventId != bslider) SimbleeForMobile.updateValue(bslider, blue);
  if (eventId != btextfield) SimbleeForMobile.updateValue(btextfield, blue);
  
  // refresh color on screen swatch with current values
  SimbleeForMobile.updateColor(swatch, rgb(red, green, blue));
  //print current RGB to Ser Mon
  Serial.printf("R %d  G %d  B %d \n", red, green, blue);

}  // end update

void ui_event(event_t &event)
{ // just resets vars according to event type; probably kept
  // small so it can react fast, farm out reaction to loop & update()
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

  needsUpdate = true;  // this value read by loop, calls update()
} // end ui_event
