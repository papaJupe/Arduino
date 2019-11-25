
/* SFM Beest Comm 1  -- 2 motor strandbeest control from iOS/Andr
   Red Spark board hard wired to control, so this is code being used
  v. 0.1 mods of SFM RoomComm for dual motor control from 6 simblee pins,
  read batt V on one analog pin

  Code here is uploaded to Simblee board to control 2 motors.
  Simblee phone app (iOS / Android) makes BLE connection to the Simblee,
  with the app displaying the user interface created here to send
  motor controller commands. To operate, power up Beast & Simblee board,
  open app, connect to board, press Power square, then drive or other. 
  Stop square stops any action; Power down just blocks drive code

  Uses: deviceName, advertData, drawText, drawTextField, drawImage,
  drawSlider, [Serial.printf], screenWidth, update(), setEvent
  for various inputs, [prev. RooComm used xxd util to load images as 
  .h file], drawText, _Rect, _Slider, byte constants in lib for image
  id's, sprintf to convert vars to # and text into displayable string,
  analog read motor batt V on pin 6 -- display in data field
  
*/

#include <SimbleeForMobile.h>

// image of drive arrows x 6 OK; > 7 crash compiler
#include "arrowU.h"
#include "arrowL.h"
#include "arrowR.h"
#include "bottom3.h"
#include "upLeft.h"
#include "upRt.h"

// int led = 2;  pin 2 now used for motor control

// update() uses these vals to init screen objects when called in ui()
uint8_t speed = 80;

bool powerMode = 0; // power down, can't drive until power up

// vars used in drive_() for R/L motor cmds
bool r1 = 0;
bool r2 = 0;
byte rP = 0; // speed range 0-254 (pwm range)

bool l1 = 0;
bool l2 = 0;
byte lP = 0;

char control[62];  // string to show current control commands in top box
char data[52] = "";  // string to show sensor data as text in 2nd frame
// byte sensIn[26];  // todo -- byte/char? array to recv more sensor data ?

uint8_t textControl;  // reports current control variables
uint8_t textData;  // field ID for batt volt / sensor data

// volatile bool needsUpdate -- true on event input,
// --  before, if true, loop called update() to do stuff,
// now not needed, ui_event just calls update() itself
// variable set by external process that's called in loop must be volatile

uint8_t eventId;

void setup() {
  // Serial.begin(57600); // no need for serial except testing

  pinMode(6, INPUT); // anal read motor batt voltage thru divider
  
  // set pins 2 through 14 for motor control outputs: only some used
  for (int thisPin = 2; thisPin <= 14; thisPin++)
  {
    pinMode(thisPin, OUTPUT);
  }
  // this text appears in the device advertisement
  // (if deviceName and advertisementData are too long to fit the 15 byte
  // BLE advert packet allowance, then the advertData is truncated from end,
  // down to a single byte, then it truncates the deviceName)
  SimbleeForMobile.deviceName = "SimBeestCom";
  SimbleeForMobile.advertisementData = "v0.1"; // total 15 ch allowed

  // use a shared cache, why ?
  // SimbleeForMobile.domain = "simblee.com";

  SimbleeForMobile.begin();
}   // end setup

void loop()
{ // loop implements current drive vars set in update()
  static int count = 0; // counts manage occasional batt volt read
  if (SimbleeForMobile.updatable) // current cmd sent to field
  {
    SimbleeForMobile.updateText(textControl, control);
    SimbleeForMobile.updateText(textData, data);
  }

  driveR(r1, r2, rP);
  driveL(l1, l2, lP);

  if (count == 2)
  {
    int battV = analogRead(6);  // AR voltage, D3 on Lily, D6 on Spark Red
    battV = map(battV, 1, 963, 0, 8400); // map to motor batt max mV
    sprintf(data, "     batt voltage = %d ", battV);  // put int's chars in array
    // Serial.println(battV);
  }
 
  if (count > 19) count = 0; // loop delay + count ==> get data every 2 sec

  // SFM.process must be called in the loop
  SimbleeForMobile.process();

  count++;
  delay(100);

}   // end loop

void driveR(bool one, bool too, byte pwm) // (motor A)
{
  digitalWrite(3, one);
  digitalWrite(4, too);
  analogWrite(5, pwm);
}
void driveL(bool one, bool too, byte pwm) // (motor B)
{
  digitalWrite(2, one);
  digitalWrite(14, too);
  analogWrite(13, pwm);
}

// ids of objects to be created in ui for screen interface

uint8_t textStop; // STOP label for center square

uint8_t dataRect;  // white frame around textControl & textData fields

uint8_t speedSlide;  // control abs value of speed, used in all drive() cmd

// 9 rects underlay arrow images for drive control
uint8_t rectA, rectB, rectC;
uint8_t rectD, rectE, rectF;
uint8_t rectG, rectH, rectI;

// 3 bottom buttons, as named, not all used in Beest
uint8_t rectBrush, rectPower, rectClean;
// 3 bottom labels, as named
uint8_t textBrush, textPower, textClean;

/********************************/

void update() // any ui_event calls this fx;
{
  // its actions could have been done in loop.
  // instead this fx resets control vars according to
  // eventId from ui_event; get sensor cmd and data display
  // to phone app will be sent in loop.

  // on 1st call from ui(), there's no eventId, so this inits things
  // if event is from other object, this could assign those values:
  // if (eventId != speedSlide) SimbleeForMobile.updateValue(speedSlide, speed);
  // if only one event can affect var, or I don't care which, update the value
  // SimbleeForMobile.updateValue(britefield, brite);

  memset(control, 0, 62); // best way to clear the array
  // on any update I put speed into control field first
  sprintf(control, "     spd = %d ", speed);  // put int's as chars in array

  //  pressing center square stops all drive, does not Power Down
  if (eventId == rectE | eventId == textStop)
  {
    r1 = 0; r2 = 0; rP = 0;
    l1 = 0; l2 = 0; lP = 0;
    sprintf(control, "     drive stopped ");
  }

  // turn power ON -- each press toggles power mode
  else if (eventId == rectPower &&  powerMode == 0)
  { char specific[] = "    pressed power ON ";
    strcat(control, specific);
    powerMode = 1;
  }

  // turn power OFF
  else if (eventId == rectPower &&  powerMode == 1)
  { // if power up already, stop and power down
    memset(control, 0, 62);
    strcat(control, "    pressed power OFF");
    powerMode = 0;
  }

  // straight forward
  else if (eventId == rectB && powerMode)
  {
    r1 = 1; r2 = 0; rP = speed;
    l1 = 1; l2 = 0;  lP = speed;
    char specific[] = "    drive FWD ";
    strcat(control, specific);
  }

  // straight back
  else if (eventId == rectH && powerMode)
  {
    r1 = 0; r2 = 1; rP = speed;
    l1 = 0; l2 = 1; lP = speed;
    char specific[] = "    drive REV ";
    strcat(control, specific);
  }

  else if (eventId == rectD && powerMode)
  {
    r1 = 1; r2 = 0; rP = (speed*4)/5;
    l1 = 0; l2 = 1; lP = (speed*4)/5;
    char specific[] = "   L spin";
    strcat(control, specific);

  }

  else if (eventId == rectF && powerMode)
  {
    r1 = 0; r2 = 1; rP = (speed*4)/5;
    l1 = 1; l2 = 0; lP = (speed*4)/5;
    char specific[] = "   R spin";
    strcat(control, specific);
  }

  else if (eventId == rectA && powerMode)
  {
    r1 = 1; r2 = 0; rP = (speed*5)/4;
    l1 = 1; l2 = 0; lP = (3*speed) / 4;
    char specific[] = "   L fwd turn";
    strcat(control, specific);
  }

  else if (eventId == rectC && powerMode)
  {
    char specific[] = "   R fwd turn";
    strcat(control, specific);
    r1 = 1; r2 = 0; rP = (speed*3)/4;
    l1 = 1; l2 = 0; lP = (speed*5)/4;
  }

  else if (eventId == rectG && powerMode)
  {
    char specific[] = "   L bak turn";
    strcat(control, specific);
    r1 = 0; r2 = 1; rP = (speed*5)/4;
    l1 = 0; l2 = 1; lP = (speed*3)/4;
  }

  else if (eventId == rectI && powerMode)
  {
    char specific[] = " R bak turn";
    strcat(control, specific);
    r1 = 0; r2 = 1; rP = (speed*3)/4;
    l1 = 0; l2 = 1; lP = (speed*5)/4;
  }

}  // end update


/**************************/

void ui() // ui() loads the screen graphics, no role in
// updating values. Screen params print once @ first update event.
{ // where could printf print ? -- C editor console ?
  // printf("UI screen size: %dx%d", SimbleeForMobile.screenWidth,
  // SFM.screenHeight); --> same 320w x 568h in Andr/iOS
  //  Serial.printf("ScreenW: %d \n", SimbleeForMobile.screenWidth);
  //  Serial.printf("ScreenH: %d \n", SimbleeForMobile.screenHeight);

  // color_t darkgray = rgb(66, 66, 66);  // screen bkgnd
  color_t medblu = rgb(66, 66, 204);
  color_t ltblu = rgb(99, 99, 255);
  color_t ltred = rgb(255, 99, 99);

#define upArro 1
#define bott3 2
#define leftArro 3
#define rtArro 4
#define upLeft 5
#define upRt 6  // app only allows 6-7 img, adding more crashes app
  //  #define downLeft 7

  SimbleeForMobile.beginScreen(medblu);

  // drwTexFld's 4th param for value is soon overwritten by update(), but
  // type is used (int vs. str) to configure field as Text or Numeric
  dataRect = SimbleeForMobile.drawRect(4, 24, 312, 88, WHITE);
  textControl = SimbleeForMobile.drawTextField(5, 26, 310, control,
                "    cmd display", WHITE, medblu);
  textData = SimbleeForMobile.drawTextField(5, 62, 310, data,
             "  data display", WHITE, medblu);

  SimbleeForMobile.imageSource(upArro, PNG, upArroGrn_png, upArroGrn_png_len );
  SimbleeForMobile.drawImage(upArro, 140, 108);  //png 3x the size of compr JPG

  SimbleeForMobile.imageSource(leftArro, PNG, leftArroGrn_png, leftArroGrn_png_len );
  SimbleeForMobile.drawImage(leftArro, 6, 244);

  SimbleeForMobile.imageSource(rtArro, PNG, rtArroGrn_png, rtArroGrn_png_len );
  SimbleeForMobile.drawImage(rtArro, 254, 244); // but JPG lacks transparency

  // underlay arrows w/ 9 rect objects
  rectA = SimbleeForMobile.drawRect(7, 111, 102, 102, ltblu); // has color
  rectB = SimbleeForMobile.drawRect(110, 111, 102, 102, CLEAR);
  rectC = SimbleeForMobile.drawRect(213, 111, 102, 102, ltblu);
  rectD = SimbleeForMobile.drawRect(7, 214, 102, 102, CLEAR);  // show bkgn
  rectE = SimbleeForMobile.drawRect(110, 214, 102, 102, ltblu);
  rectF = SimbleeForMobile.drawRect(213, 214, 102, 102, CLEAR);
  rectG = SimbleeForMobile.drawRect(7, 317, 102, 102, ltblu);
  rectH = SimbleeForMobile.drawRect(110, 317, 102, 102, CLEAR);
  rectI = SimbleeForMobile.drawRect(213, 317, 102, 102, ltblu);

  // corner and bottom arrows overlay the rect, to see lighter color
  SimbleeForMobile.imageSource(upLeft, PNG, upLeft_png, upLeft_png_len );
  SimbleeForMobile.drawImage(upLeft, 11, 116);

  SimbleeForMobile.imageSource(upRt, PNG, upRt_png, upRt_png_len );
  SimbleeForMobile.drawImage(upRt, 257, 114);

  SimbleeForMobile.imageSource(bott3, PNG, bottom3_png, bottom3_png_len );
  // draw transp image over rect, andr Y, iOS Y but may block rect event
  SimbleeForMobile.drawImage(bott3, 8, 362);

  //  can't add >7 images: compiles, but app fails to open, these were combined, v.s.
  //  SimbleeForMobile.imageSource(downLeft, PNG, downLeft_png, downLeft_png_len );
  //  SimbleeForMobile.drawImage(downLeft, 11, 355);

  SimbleeForMobile.drawText(140, 248, "STOP", WHITE);
  SimbleeForMobile.drawText(120, 428, "Drive Speed", WHITE);
  SimbleeForMobile.drawRect(8, 448, 300, 32, ltblu); // color not shown iOS/andr 
  speedSlide = SimbleeForMobile.drawSlider(26, 449, 270, 0, 254, ltred);

  // 3 control rects, as buttons
  rectBrush = SimbleeForMobile.drawRect(4, 485, 102, 82, RED);
  rectPower = SimbleeForMobile.drawRect(108, 485, 102, 82, GREEN);
  rectClean = SimbleeForMobile.drawRect(212, 485, 102, 82, YELLOW);
  //  // labels for controls, also need to make events ?
  //  textBrush = SimbleeForMobile.drawText(30, 495, "BRUSH", BLACK);
  textPower = SimbleeForMobile.drawText(134, 495, "POWER", BLACK);
  //  textClean = SimbleeForMobile.drawText(236, 495, "CLEAN", BLACK);

  // RAM overflow if I include big img -- why? compile says only 35% used!
  // SimbleeForMobile.imageSource(brushB, JPG, brushButt_jpg, brushButt_jpg_len );
  // will rect still work if I draw transp image over it: andr Y, iOS Y if not on it
  //  SimbleeForMobile.drawImage(brushB, 4, 483);

  //rtextfield = SimbleeForMobile.drawTextField(245, 65, 50, red, "", WHITE, darkgray);

  //SimbleeForMobile.drawText(114, 114, "∆", WHITE);
  //  britestep = SimbleeForMobile.drawStepper(25, 202, 80, 0, 255, WHITE);
  //  britefield = SimbleeForMobile.drawTextField(120,202,55,brite,"#", WHITE, darkgray);
  //SimbleeForMobile.updateValue(britestep, brite); // init the step control value

  //  // border and swatch to display active color
  //  SimbleeForMobile.drawRect(182, 200, 126, 40, WHITE);
  //  swatch = SimbleeForMobile.drawRect(183, 201, 124, 38, WHITE);

  // init slider value to the default speed
  SimbleeForMobile.updateValue(speedSlide, speed);

  SimbleeForMobile.setEvents(rectA, EVENT_PRESS);
  SimbleeForMobile.setEvents(rectB, EVENT_PRESS);
  SimbleeForMobile.setEvents(rectC, EVENT_PRESS);

  SimbleeForMobile.setEvents(rectD, EVENT_PRESS);
  SimbleeForMobile.setEvents(rectE, EVENT_PRESS);
  SimbleeForMobile.setEvents(rectF, EVENT_PRESS);

  SimbleeForMobile.setEvents(rectG, EVENT_PRESS);
  SimbleeForMobile.setEvents(rectH, EVENT_PRESS);
  SimbleeForMobile.setEvents(rectI, EVENT_PRESS);
  // need all such events set for Android, not iOS
  SimbleeForMobile.setEvents(rectPower, EVENT_PRESS);
  SimbleeForMobile.setEvents(textPower, EVENT_PRESS);

  SimbleeForMobile.setEvents(speedSlide, EVENT_DRAG | EVENT_RELEASE);

  SimbleeForMobile.endScreen();

  // update sets desired init values
  // (this must be done after endScreen() on startup,
  // otherwise the initial values will fill from the cache)

  update();
}  // end ui

/********************************/

void ui_event(event_t &event)
{ // just resets vars according to event ID; kept small
  // for speed --> most actions done in loop & update()

  if (event.id == speedSlide)
  {
    speed = (byte)event.value; // was uint16
    // green = event.green;
  } // I only update eventId if it's not speedSlide, so the new speed goes to
  // existing run mode immediately
  // 'eventId = event.id' -- update() uses this global var to set drive params
  else eventId = event.id;

  //  else if (event.id == rslider || event.id == rtextfield)
  //    red = event.value;

  // event.types to do various stuff, would mostly do in update() or loop
  //  if (event.type == EVENT_PRESS) {
  //    if (event.id == tempID) {
  //      lastCheck = millis() - timeout;
  //    } else if (event.id == unitID) {
  //      unit = !unit;
  //      SimbleeForMobile.updateText(unitID, unit == FAHRENHEIT ? "F" : "C");
  //      lastCheck = millis() - timeout;
  //    }
  //  } else if (event.id == sliderID && event.type == EVENT_RELEASE) {
  //    timeout = event.value * 1000;
  //    SimbleeForMobile.updateValue(timeoutText, event.value);
  //    lastCheck = millis() - timeout;
  //  }

  update();

} // end ui_event

/*

String sensorsAsString() inherited from RoomComm; no use in Beest; masks/ shifts seemed
to work on char element as well as byte
{
  return
    (" chgState: " + String(sensIn[CHARGINGSTATE] & 0xff) +
     " volt: " + String((sensIn[VOLTAGE_HI] << 8) | (sensIn[VOLTAGE_LO] & 0xff)) +
     " curr: " + String(int16_t((sensIn[CURRENT_HI] << 8) | (sensIn[CURRENT_LO] & 0xff))) +
     " temp: " + String(sensIn[TEMPERATURE] & 0xff) );
  //      " chrg:" + charge() +
  //      " capa:" + capacity() +
  // current needs cast back to signed int, + = charge, - = discharge mA
}  // end sensAsStr

*/