
/* SFM RoomCommTest02  -- experiments for R control from iOS/Andr
    v. 0.1 has images for ui and few trial controls
    v. 0.2 adds libs for byte constants, links screen events
    to commands: some control of power, drive, stop, brush, clean
    fwd turns still fail here

  Code here is uploaded to Simblee board that can be freestanding w/ (batt
  power) or USB powered to keep Serial comm to Ardu's IDE Ser Mon for
  debugging or whatever. Board's Serial could also comm I / O w/
  another serial device like Roomba.
  For wireless comm to whatever Simblee's Serial is connected to -- Simblee
  phone app (iOS / Android) makes BLE connection to the Simblee, with the
  app displaying the user interface created here.

  Uses: deviceName, advertData, drawText, drawTextField, drawImage,
  drawSlider, Serial.printf, screenWidth, update(), color_wheel, setEvent
  for various inputs, volatile var (changed in event thread), xxd util to
    load .h images, serial.write(byte array,len), drawText, _Rect, _Slider
    byte constants in lib, sprintf to make # and text into displayable str

  todo: for v. 0.3 -- sensor feedback display, soften turns if speed fast,
 fuse bottom images into 1

*/

#include <SimbleeForMobile.h>
#include <constant.h>  // control vals in lib/SimbCons folder
// #include <moveCommand.h> // hi level drive fx, done here for now

// image of drive arrows x 6 OK, control butts; > 7 crash compiler
#include "arrowU.h"
#include "arrowD.h"
#include "arrowL.h"
#include "arrowR.h"
//#include "downRt.h"
//#include "downLeft.h"
#include "upLeft.h"
#include "upRt.h"
//#include "brushButtJ.h"
//#include "cleanButtJ.h"
//#include "powerButtJ.h"

int led1 = 13;  // 13 on SimbLily, 2 on Spark BOB

// update() uses these vals to init screen objects when called @ end of ui()
int16_t speed = 120;
int16_t radius = 0;

char data[100] = "";  // string to show 'data' in top frame
bool powerMode = 0;  // 0 is power down

volatile bool needsUpdate;  // volatile -- depends on event input, set in
// ui-event -- if true loop calls update to do stuff

uint8_t eventId;

void setup() {
  Serial.begin(57600);
  //  Serial.println("something Started");
  //  Serial.printf("faux set safe code: %d", MODE_SAFE);
  //  Serial.println();
  //  Serial.printf("faux send power down: %d \n", POWER);
  // if really sent, would set powerMode true (on)

  // setup the leds for output
  // pinMode(led1, OUTPUT);

  // set initial color
  //analogWrite(led1, red);

  // this is the data that appears in the advertisement of device
  // (if deviceName and advertisementData are too long to fit the 15 byte
  // BLE advert packet allowance, then the advertData is truncated from end,
  // down to a single byte, then it truncates the deviceName)
  SimbleeForMobile.deviceName = "SimRooCom";
  SimbleeForMobile.advertisementData = "v0.2"; // total 15 ch allowed

  // use a shared cache
  // SimbleeForMobile.domain = "simblee.com";

  SimbleeForMobile.begin();
}   // end setup

void loop() {  // loop farms out all action to update(), ? speedier ?
  if (SimbleeForMobile.updatable)
  {
    if (needsUpdate)
    {
      update();
      needsUpdate = false;
    }
  }
  // SFM.process must be called in the loop
  SimbleeForMobile.process();

}   // end loop

// ids of objects created in ui for screen interface
uint8_t txtfielData;  // reports var state or sensor feedback
uint8_t dataRect;  // frame around txtfielData
uint8_t speedSlide;  // control abs value of speed, used in all drive() cmd

// 9 rects underlay arrow image for drive control
uint8_t rectA, rectB, rectC;
uint8_t rectD, rectE, rectF;
uint8_t rectG, rectH, rectI;

// 3 bottom butts, as named
uint8_t rectBrush, rectPower, rectClean;
// 3 bottom labels, as named
uint8_t textBrush, textPower, textClean;

void ui() // ui() may just set up the initial graphics, no role in
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
#define downArro 2
#define leftArro 3
#define rtArro 4
#define upLeft 5
#define upRt 6  // only allowed 6-7 img, adding more crashes app
  //  #define downLeft 7
  //  #define downRt 8
  //  #define brushB 9
  //  #define cleanB 10
  //  #define powerB 11

  SimbleeForMobile.beginScreen(medblu);

  // drwTexFld's 4th param for value is soon overwritten by update(), but
  // type is used (int vs. str) to configure field as Text or Numeric
  dataRect = SimbleeForMobile.drawRect(4, 24, 312, 88, WHITE);
  txtfielData = SimbleeForMobile.drawTextField(5, 26, 310, data, "   data display", WHITE, medblu);

  // arrows image (name, type, src name, src len variable from .h file)
  //  SimbleeForMobile.imageSource(IMAGE1, JPG, arrows_jpg, arrows_jpg_len );
  //  SimbleeForMobile.drawImage(IMAGE1, 5, 110);

  SimbleeForMobile.imageSource(upArro, PNG, upArroGrn_png, upArroGrn_png_len );
  SimbleeForMobile.drawImage(upArro, 140, 108);  //png 3x the size of compr JPG

  SimbleeForMobile.imageSource(downArro, PNG, downArroGrn_png, downArroGrn_png_len );
  // will rect still work if I draw transp image over it, andr Y, iOS Y if not on it
  SimbleeForMobile.drawImage(downArro, 142, 348);

  SimbleeForMobile.imageSource(leftArro, PNG, leftArroGrn_png, leftArroGrn_png_len );
  SimbleeForMobile.drawImage(leftArro, 6, 244);  //png 3x the size of compr JPG

  SimbleeForMobile.imageSource(rtArro, PNG, rtArroGrn_png, rtArroGrn_png_len );
  SimbleeForMobile.drawImage(rtArro, 254, 244); // but JPG lacks transparency

  // underlay arrows w/ 9 rect objects
  rectA = SimbleeForMobile.drawRect(7, 111, 102, 102, ltblu);
  rectB = SimbleeForMobile.drawRect(109, 111, 102, 102, CLEAR);// has color
  rectC = SimbleeForMobile.drawRect(213, 111, 102, 102, ltblu);
  rectD = SimbleeForMobile.drawRect(7, 214, 102, 102, CLEAR);  // show bkgn
  rectE = SimbleeForMobile.drawRect(110, 214, 102, 102, ltblu);
  rectF = SimbleeForMobile.drawRect(214, 214, 102, 102, CLEAR);
  rectG = SimbleeForMobile.drawRect(7, 317, 102, 102, ltblu);
  rectH = SimbleeForMobile.drawRect(109, 317, 102, 102, CLEAR);
  rectI = SimbleeForMobile.drawRect(214, 317, 102, 102, ltblu);

  // corner arrows overlay the rect, to see lighter color
  SimbleeForMobile.imageSource(upLeft, PNG, upLeft_png, upLeft_png_len );
  SimbleeForMobile.drawImage(upLeft, 11, 116);
  //
  SimbleeForMobile.imageSource(upRt, PNG, upRt_png, upRt_png_len );
  // will rect still work if I draw transp image over it, andr Y, iOS Y if not on it
  SimbleeForMobile.drawImage(upRt, 257, 114);

  //  can't add more images, compiles, but app fails to open
  //  SimbleeForMobile.imageSource(downLeft, PNG, downLeft_png, downLeft_png_len );
  //  SimbleeForMobile.drawImage(downLeft, 11, 355);
  //
  //  SimbleeForMobile.imageSource(downRt, PNG, downRt_png, downRt_png_len );
  //  SimbleeForMobile.drawImage(downRt, 257, 355);


  SimbleeForMobile.drawText(120, 428, "Drive Speed", WHITE);
  SimbleeForMobile.drawRect(8, 448, 300, 32, ltblu); // color not shown either app
  speedSlide = SimbleeForMobile.drawSlider(26, 449, 270, 0, 400, ltred);

  // 3 control rects, as buttons
  rectBrush = SimbleeForMobile.drawRect(4, 485, 102, 82, RED);
  rectPower = SimbleeForMobile.drawRect(108, 485, 102, 82, GREEN);
  rectClean = SimbleeForMobile.drawRect(212, 485, 102, 82, YELLOW);
  // labels for controls, also need to make events ?
  textBrush = SimbleeForMobile.drawText(30, 495, "BRUSH", BLACK);
  textPower = SimbleeForMobile.drawText(134, 495, "POWER", BLACK);
  textClean = SimbleeForMobile.drawText(236, 495, "CLEAN", BLACK);

  // RAM overflow if I include big img -- why? compile says only 35% used!
  // SimbleeForMobile.imageSource(brushB, JPG, brushButt_jpg, brushButt_jpg_len );
  //// will rect still work if I draw transp image over it, andr Y, iOS Y if not on it
  //  SimbleeForMobile.drawImage(brushB, 4, 483);
  //
  //   SimbleeForMobile.imageSource(powerB, JPG, powerButt_jpg, powerButt_jpg_len );
  //// will rect still work if I draw transp image over it, andr Y, iOS Y if not on it
  //  SimbleeForMobile.drawImage(powerB, 108, 483);
  //
  //   SimbleeForMobile.imageSource(cleanB, JPG, cleanButt_jpg, cleanButt_jpg_len );
  //// will rect still work if I draw transp image over it, andr Y, iOS Y if not on it
  //  SimbleeForMobile.drawImage(cleanB, 212, 483);

  //rtextfield = SimbleeForMobile.drawTextField(245, 65, 50, red, "", WHITE, darkgray);

  //SimbleeForMobile.drawText(114, 114, "∆", WHITE);
  //  britestep = SimbleeForMobile.drawStepper(25, 202, 80, 0, 255, WHITE);
  //  britefield = SimbleeForMobile.drawTextField(120,202,55,brite,"#", WHITE, darkgray);
  //
  //  // border and swatch to display active color
  //  SimbleeForMobile.drawRect(182, 200, 126, 40, WHITE);
  //  swatch = SimbleeForMobile.drawRect(183, 201, 124, 38, WHITE);

  // color_wheel = SimbleeForMobile.drawImage(COLOR_WHEEL, 10, 250);
  //SimbleeForMobile.setEvents(color_wheel, EVENT_COLOR);

  SimbleeForMobile.setEvents(rectA, EVENT_PRESS);
  SimbleeForMobile.setEvents(rectB, EVENT_PRESS);
  SimbleeForMobile.setEvents(rectC, EVENT_PRESS);

  SimbleeForMobile.setEvents(rectD, EVENT_PRESS);
  SimbleeForMobile.setEvents(rectE, EVENT_PRESS);
  SimbleeForMobile.setEvents(rectF, EVENT_PRESS);

  SimbleeForMobile.setEvents(rectG, EVENT_PRESS);
  SimbleeForMobile.setEvents(rectH, EVENT_PRESS);
  SimbleeForMobile.setEvents(rectI, EVENT_PRESS);

  SimbleeForMobile.setEvents(rectPower, EVENT_PRESS);
  SimbleeForMobile.setEvents(textPower, EVENT_PRESS);

  SimbleeForMobile.setEvents(speedSlide, EVENT_DRAG | EVENT_RELEASE);

  SimbleeForMobile.endScreen();

  //SimbleeForMobile.updateValue(britestep, brite); // init the step control value

  // update sets desired init values
  // (this must be done after endScreen() on startup,
  // otherwise the initial values will fill from the cache)

  update();
}  // end ui

void update() // loop calls this fx, when loop sees needsUpdate = true
// -- (set to true by any input event) -- update() actions could
// have been done in loop. Instead this fx does all changes
{ // both to the board hardware and display on the phone app
  // so will also send cmd bytes from here
  // analogWrite(led1, red);  // write current red level to my 1 onboard LED

  // on 1st call, there's no eventId, so this inits all values to defaults
  // if event is from other object, this sends those values to these things
  // if (eventId != speedSlide) SimbleeForMobile.updateValue(speedSlide, speed);

  // on any update I start data with speed
  sprintf(data, "     spd= %d ", speed);  // put int's as chars in array

  // stop if center pressed
  if (eventId == rectE)
  {
    drive(0, 0);
    sprintf(data, "   drive stopped ");
    byte cmd[] = {START, CONTROL, MODE_SAFE};
    for (int i = 0; i < sizeof(cmd); i++)
    { Serial.write(cmd[i]);
      delay(240);
    }
  }

  // power ON
  else if (eventId == rectPower &&  powerMode == 0)
  { char spin[] = "  pressed power ON ";
    strcat(data, spin);
    byte cmd[] = {byte(START), byte(CONTROL), byte(MODE_SAFE)};
    //  Serial.write(cmd, 4); // need delay for startup bytes
    for (int i = 0; i < sizeof(cmd); i++)
    { Serial.write(cmd[i]);
      delay(240);
    }
    powerMode = 1;
  }

  // power OFF
  else if (eventId == rectPower &&  powerMode == 1)
  { // if power up already, turn it all off
    char spin[] = "  pressed power OFF";
    strcat(data, spin);
    powerMode = 0;
    drive(0, 0);
    delay(10);
    // powering down
    Serial.write(POWER);
    // if only one event can affect var, send the new value
    // SimbleeForMobile.updateValue(britefield, brite);
  }

  // forward
  else if (eventId == rectB && powerMode)
  {
    drive(speed, 32768);
  }

  // straight back
  else if (eventId == rectH && powerMode)
  {
    drive(speed * (-1), 32768);
  }

  else if (eventId == rectD && powerMode)
  {
    char spin[] = " L spin";
    drive(speed, 1);
    strcat(data, spin);
  }

  else if (eventId == rectF && powerMode)
  {
    char spin[] = " R spin";
    strcat(data, spin);
    drive(speed, -1);
  }

  else if (eventId == rectA && powerMode)
  {
    char spin[] = " L fwd turn";
    drive(speed, 200);
    strcat(data, spin);
  }

  else if (eventId == rectC && powerMode)
  {
    char spin[] = " R fwd turn";
    strcat(data, spin);
    drive(speed, -200);
  }

  else if (eventId == rectG && powerMode)
  {
    char spin[] = " L bak turn";
    drive(speed * (-1), 200);
    strcat(data, spin);
  }

  else if (eventId == rectI && powerMode)
  {
    char spin[] = " R bak turn";
    strcat(data, spin);
    drive(speed * (-1), -200);
  }
  
  else if (eventId == rectBrush && powerMode)
  {
    char spin[] = " brush up";
    strcat(data, spin);
    Serial.write(138); Serial.write(7);
  }

  else if (eventId == rectClean && powerMode)
  {
    char spin[] = " clean up";
    strcat(data, spin);
    Serial.write(135);
  }
  // if (eventId != txtfielData) what do I care?
  SimbleeForMobile.updateText(txtfielData, data);

  //print current RGB to Ser Mon, vals display as ints
  //Serial.printf("R %d  G %d  B %d * %d\n", red, green, blue, brite);

  // for R control, make byte array, Serial.write(array,len) +/- nl
  //  byte cmd[] = {byte(red), byte(green), byte(blue), byte(brite), 10};
  //  Serial.write(cmd, 5); // fine for sending bytes
  // prints bytes as number to Ser Mon
  //    for (int i = 0; i < sizeof(cmd); i++)
  //    { Serial.print(cmd[i]);
  //      Serial.print(" ");
  //    }
  //    Serial.println();
  // if only one event can affect var, send the new value
  // SimbleeForMobile.updateValue(britefield, brite);

}  // end update

void ui_event(event_t &event)
{ // just resets vars according to event ID; kept
  // small for speed --> farm out reaction to loop & update()
  eventId = event.id; //  # update() uses 4 various var dependent stuff

  if (event.id == speedSlide)
  {
    speed = event.value;
    // green = event.green;
  }
  //  else if (event.id == rslider || event.id == rtextfield)
  //    red = event.value;

  // event.types to do various stuff
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

  needsUpdate = true;  // when true, loop calls update() to process vars
} // end ui_event

void drive( int velo, int radi)
{
  byte cmd[] = {DRIVE, (velo >> 8), (velo & 0xff), (radi >> 8), (radi & 0xff)};

  char temp[28];
  sprintf(temp, " spd %d %d, rad %d %d ", cmd[1], cmd[2], cmd[3], cmd[4]);
  strcat(data, temp);
  Serial.write( cmd, 5 );
}  // end drive

// ? called by .process, need this to power off R onDisc
//void SimbleeForMobile_onDisconnect() {
//  isConnected = 0; // reset some vars to defaults
//  unit = FAHRENHEIT;
//  digitalWrite(ledHot, LOW);
//  digitalWrite(ledCold, HIGH);
//  Serial.write(POWER); probably no Serial
// }  // end disconnect
