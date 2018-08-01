
/* SFM Beest Comm 1  -- 2 motor beest control from iOS/Andr

  v. 0.1 mods of SFM RoomComm for dual motor control from 6 simb pins

  toDo: 

  Code here is uploaded to Simblee board to control 2 motors.
  Simblee phone app (iOS / Android) makes BLE connection to the Simblee,
  with the app displaying the user interface created here to write to
  motor controller inputs. To operate, power up Beast & Simb bd,
  open app, connect to Simb, press Power square, then drive or other. Stop
  square stops any action -- hopefully; Power down just blocks drive code

  Uses: deviceName, advertData, drawText, drawTextField, drawImage,
  drawSlider, [Serial.printf], screenWidth, update(), setEvent
  for various inputs, [prev. RooComm used xxd util toload .h images],
  serial.write(byte array,len), drawText, _Rect, _Slider
  byte constants in lib, sprintf to make # and text into displayable str,
  analog read motor batt V on A4/D3 pin -- display in data field;
  
*/

#include <SimbleeForMobile.h>

// image of drive arrows x 6 OK; > 7 crash compiler, ? still
#include "arrowU.h"
#include "arrowL.h"
#include "arrowR.h"
#include "bottom3.h"
#include "upLeft.h"
#include "upRt.h"

int led = 13;  // 13 on SimbLily, 2 on red Spark BOB

// update() uses these vals to init screen objects when called in ui()
uint8_t speed = 100;

bool powerMode = 0; // power down, can't drive

// vars used in driveX() for R/L motor cmds
bool r1 = 0;
bool r2 = 0;
byte rP = 0; // speed range 0-254

bool l1 = 0;
bool l2 = 0;
byte lP = 0;

char cont[62];  // string to show current control commands in top box
char data[52] = "";  // string to show sensor data as text in 2nd frame
// byte sensIn[26];  // todo -- byte array to recv sensor data

uint8_t textControl;  // reports current control var
uint8_t textData;  // field ID for sensor data

// volatile bool needsUpdate;  // volatile -- true on event input,
// -- if true, loop called update() to do stuff,
// now ui_event just calls update() itself

uint8_t eventId;

void setup() {
  // Serial.begin(57600); // no use for serial except testing

  // onboard led flashes w/ loop
  // pinMode(led, OUTPUT);  // done below
  pinMode(3, INPUT); // to read motor batt voltage thru divider
  // set pins 5 through 15 as outputs: only some used
  for (int thisPin = 5; thisPin <= 15; thisPin++)
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
  static int count = 0; // cts manage led flash and volt read
  if (SimbleeForMobile.updatable) // current cmd sent to field
  {
    SimbleeForMobile.updateText(textControl, cont);
    SimbleeForMobile.updateText(textData, data);
  }

  driveR(r1, r2, rP);
  driveL(l1, l2, lP);

  // board led shows loop is running
  if (count == 2)
  {
    digitalWrite(led, HIGH);
    int battV = analogRead(3);  // D3 on Lily works for AR
    battV = map(battV, 1, 1023, 0, 8400); // map to motor batt max mV
    sprintf(data, "     batt voltage = %d ", battV);  // put int's chars in array
    // Serial.println(battV);
  }
  if (count == 8) digitalWrite(led, LOW);
  if (count > 19) count = 0; // delay & count ==> gets data every 2 sec

  // SFM.process must be called in the loop
  SimbleeForMobile.process();

  count++;
  delay(100);

}   // end loop

void driveR(bool one, bool too, byte pwm)
{
  digitalWrite(5, one);
  digitalWrite(6, too);
  analogWrite(9, pwm);
}
void driveL(bool one, bool too, byte pwm)
{
  digitalWrite(11, one);
  digitalWrite(12, too);
  analogWrite(15, pwm);
}

// ids of objects to be created in ui for screen interface

uint8_t textStop; // STOP label for center square

uint8_t dataRect;  // white frame around textControl & textData fields

uint8_t speedSlide;  // control abs value of speed, used in all drive() cmd

// 9 rects underlay arrow image for drive control
uint8_t rectA, rectB, rectC;
uint8_t rectD, rectE, rectF;
uint8_t rectG, rectH, rectI;

// 3 bottom butts, as named, not all used now
uint8_t rectBrush, rectPower, rectClean;
// 3 bottom labels, as named
uint8_t textBrush, textPower, textClean;

/********************************/

void update() // any ui_event calls this fx;
{
  // its actions could have been done in loop.
  // instead this fx resets control vars according to
  // eventId from ui_event; cmd and sensor data display
  // to phone app will be sent in loop.

  // on 1st call from ui(), there's no eventId, so this inits things
  // if event is from other object, this could assign those values:
  // if (eventId != speedSlide) SimbleeForMobile.updateValue(speedSlide, speed);
  // if only one event can affect var, or I don't care which, update the value
  // SimbleeForMobile.updateValue(britefield, brite);

  memset(cont, 0, 62);
  // on any update I put speed into control field first
  sprintf(cont, "     spd = %d ", speed);  // put int's as chars in array

  //  pressing center square stops all drive, does not Power down
  if (eventId == rectE | eventId == textStop)
  {
    r1 = 0; r2 = 0; rP = 0;
    l1 = 0; l2 = 0; lP = 0;
    sprintf(cont, "     drive stopped ");
  }

  // turn power ON -- each press toggles power mode
  else if (eventId == rectPower &&  powerMode == 0)
  { char spin[] = "    pressed power ON ";
    strcat(cont, spin);
    powerMode = 1;
  }

  // turn power OFF
  else if (eventId == rectPower &&  powerMode == 1)
  { // if power up already, stop and power down
    memset(cont, 0, 62);
    strcat(cont, "    pressed power OFF");
    powerMode = 0;
  }

  // straight forward
  else if (eventId == rectB && powerMode)
  {
    r1 = 1; r2 = 0; rP = speed;
    l1 = 0; l2 = 1;  lP = speed;
    char spin[] = "    drive FWD ";
    strcat(cont, spin);
  }

  // straight back
  else if (eventId == rectH && powerMode)
  {
    r1 = 0; r2 = 1; rP = speed;
    l1 = 1; l2 = 0; lP = speed;
    char spin[] = "    drive REV ";
    strcat(cont, spin);
  }

  else if (eventId == rectD && powerMode)
  {
    r1 = 1; r2 = 0; rP = speed / 2;
    l1 = 0; l2 = 1; lP = speed / 2;
    char spin[] = "   L spin";
    strcat(cont, spin);

  }

  else if (eventId == rectF && powerMode)
  {
    r1 = 0; r2 = 1; rP = speed / 2;
    l1 = 1; l2 = 0; lP = speed / 2;
    char spin[] = "   R spin";
    strcat(cont, spin);
  }

  else if (eventId == rectA && powerMode)
  {
    r1 = 1; r2 = 0; rP = speed;
    l1 = 0; l2 = 1; lP = speed / 3;
    char spin[] = "   L fwd turn";
    strcat(cont, spin);

  }

  else if (eventId == rectC && powerMode)
  {
    char spin[] = "   R fwd turn";
    strcat(cont, spin);
    r1 = 1; r2 = 0; rP = speed / 3;
    l1 = 0; l2 = 1; lP = speed;
  }

  else if (eventId == rectG && powerMode)
  {
    char spin[] = "   L bak turn";
    strcat(cont, spin);
    r1 = 0; r2 = 1; rP = speed;
    l1 = 1; l2 = 0; lP = speed / 3;

  }

  else if (eventId == rectI && powerMode)
  {
    char spin[] = " R bak turn";
    strcat(cont, spin);
    r1 = 0; r2 = 1; rP = speed / 3;
    l1 = 1; l2 = 0; lP = speed;
  }

}  // end update


/**************************/

void ui() // ui() loads the screen graphics, no role in
// updating values. Screen params print once @ first update event.
{ // where could printf print ? C editor console ?
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
#define upRt 6  // only allowed 6-7 img, adding more crashes app
  //  #define downLeft 7

  SimbleeForMobile.beginScreen(medblu);

  // drwTexFld's 4th param for value is soon overwritten by update(), but
  // type is used (int vs. str) to configure field as Text or Numeric
  dataRect = SimbleeForMobile.drawRect(4, 24, 312, 88, WHITE);
  textControl = SimbleeForMobile.drawTextField(5, 26, 310, cont,
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

  //  can't add >7 images?: compiles, but app fails to open
  //  SimbleeForMobile.imageSource(downLeft, PNG, downLeft_png, downLeft_png_len );
  //  SimbleeForMobile.drawImage(downLeft, 11, 355);

  SimbleeForMobile.drawText(140, 248, "STOP", WHITE);
  SimbleeForMobile.drawText(120, 428, "Drive Speed", WHITE);
  SimbleeForMobile.drawRect(8, 448, 300, 32, ltblu); // color not shown either app
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
  //// will rect still work if I draw transp image over it, andr Y, iOS Y if not on it
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
  //  SimbleeForMobile.setEvents(rectBrush, EVENT_PRESS);
  //  SimbleeForMobile.setEvents(textBrush, EVENT_PRESS);
  //  SimbleeForMobile.setEvents(rectClean, EVENT_PRESS);
  //  SimbleeForMobile.setEvents(textClean, EVENT_PRESS);

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
  // eventId = event.id; //  value that update() uses to set vars

  if (event.id == speedSlide)
  {
    speed = (byte)event.value; // was uint16
    // green = event.green;
  } // I only update eventId if it's not speedSlide, so the new speed goes to
  // existing run mode immediately
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

/********************************/

//String sensorsAsString()
//{
//  return
//    (" chgState: " + String(sensIn[CHARGINGSTATE] & 0xff) +
//     " volt: " + String((sensIn[VOLTAGE_HI] << 8) | (sensIn[VOLTAGE_LO] & 0xff)) +
//     " curr: " + String(int16_t((sensIn[CURRENT_HI] << 8) | (sensIn[CURRENT_LO] & 0xff))) +
//     " temp: " + String(sensIn[TEMPERATURE] & 0xff) );
//  //      " chrg:" + charge() +
//  //      " capa:" + capacity() +
//  // current needs cast back to signed int, + = charge, - = discharge mA
//}  // end sensAsStr

