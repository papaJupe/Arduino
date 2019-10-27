
/* SFM RoomComm04  -- working R control from iOS/Andr

    v. 0.1 has images for ui and few trial controls
    v. 0.2 adds libs for byte constants, links screen events
    to commands: some control of power, drive, stop, brush, clean
    v 0.3 fused bottom images into bottom3, add STOP txt, made fast turns
    slower, add field for sensor data (not filled yet)
    v 0.4 adds sensor reading, display some data to text field, Red,Lily vers
    v 0.5 minor mods for Red and Lily bds, uses readBytes for sensor read

  Code here is uploaded to Simblee board for Serial comm w/ Roomba port.
  Simblee phone app (iOS / Android) makes BLE connection to the Simblee,
  with the app displaying the user interface created here to control &
  see sensor data on the R. To operate, Enable R by grnding DD, or button,
  open app, connect to Simb, press Power square, then drive or other. Stop
  square stops anything; Reds: always Power Down with Power square. Grays
  can disable w/ button.
  To see params while charging, power up then down w/ Power sq.

  Uses: deviceName, advertData, drawText, drawTextField, drawImage,
  drawSlider, Serial.printf, screenWidth, update(), setEvent
  for various inputs, volatile var (changed in event thread), xxd util to
  load .h images, serial.write(byte array,len), drawText, _Rect, _Slider
  byte constants in lib, sprintf to make # and text into displayable str,
  enhanced if (ternary operator) to adjust turn speed, readBytes

*/

#include <SimbleeForMobile.h>
#include <constant.h>  // control references in lib/SimbCons folder

// image of drive arrows x 6 OK, control butts; > 7 crash compiler
#include "arrowU.h"
#include "arrowL.h"
#include "arrowR.h"
#include "bottom3.h"
#include "upLeft.h"
#include "upRt.h"

int led = 2;  // 13 on SimbLily, 2 on red Spark BOB

// update() uses these vals to init screen objects when called @ end of ui()
int16_t speed = 120;
// int16_t radius = 0; // can be sent to drive() but I use constants here
bool powerMode = 0;  // 0 is power down

char cont[62];  // string to show current control commands in top box
char data[52] = "";  // string to show sensor data as text in 2nd frame
char sensIn[26];  // char array to recv sensor data

uint8_t textData;  // field for select sensor feedback data

// volatile because depends on event input, set in
// ui-event -- if true loop calls update() to do stuff
volatile bool needsUpdate;

uint8_t eventId;

void setup() {
  Serial.begin(57600);
  //  Serial.println("something Started");
  //  Serial.printf("faux set safe code: %d", MODE_SAFE);
  //  Serial.println();
  //  Serial.printf("faux send power down: %d \n", POWER);
  // if really sent, would set powerMode true (on)

  // set the onboard led as output
  pinMode(led, OUTPUT);  // flash when serial/sensor request sent

  // set initial color
  //analogWrite(led1, red);

  // this is the data that appears in the advertisement of device
  // (if deviceName and advertisementData are too long to fit the 15 byte
  // BLE advert packet allowance, then the advertData is truncated from end,
  // down to a single byte, then it truncates the deviceName)
  SimbleeForMobile.deviceName = "SimRooComRed";
  SimbleeForMobile.advertisementData = "0.5"; // total 15 ch allowed

  // use a shared cache   -- would this help Android?
  // SimbleeForMobile.domain = "simblee.com";

  SimbleeForMobile.begin();
}   // end setup

void loop() {  // loop farms out all action to update(), ? speedier
  static int count = 0;
  if (SimbleeForMobile.updatable)
  {
    if (needsUpdate)
    {
      update();
      needsUpdate = false;
    }
  }

  if (count == 2) // get sensor data once in a while
  {
    memset(sensIn, 0, 26); // clear the byte array for data

    // request SENSORS, then process response
    Serial.write(142);
    Serial.write(byte(0x00)); // 0 here gets all 26 bytes
    digitalWrite(led, HIGH);
    delay(40);  // probably could be smaller
    //    int i = 0;
    //    byte inB = 0;
    if (Serial.available())
    {
      // delay(5);
      // .readBytes(buffer,length) puts (length) bytes into (buffer) array
      Serial.readBytes(sensIn, 26); //  incoming bytes --> char array
      while (Serial.read() != -1); // clear the input buffer, should be already

    }

    // send parsed/selected data to text field
    memset(data, 0, 52);
    sensorsAsString().toCharArray(data, 52);
    SimbleeForMobile.updateText(textData, data);
  }  // end get/report sensor data

  count++;
  if (count == 10) digitalWrite(led, LOW);
  if (count > 99) count = 0; // delay + count ==> get data every 2 sec
  delay(20);

  // SFM.process must be called in the loop
  SimbleeForMobile.process();

}   // end loop

// ids of objects to be created in ui for screen interface
uint8_t textControl;  // reports current control var
//uint8_t textData;  // report sensor data in, declared above, before loop
uint8_t textStop; // STOP label for center square

uint8_t dataRect;  // white frame around textControl & textData fields
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

  //  can't add more images: compiles, but app fails to open
  //  SimbleeForMobile.imageSource(downLeft, PNG, downLeft_png, downLeft_png_len );
  //  SimbleeForMobile.drawImage(downLeft, 11, 355);

  SimbleeForMobile.drawText(140, 248, "STOP", WHITE);
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
  SimbleeForMobile.setEvents(rectBrush, EVENT_PRESS);
  SimbleeForMobile.setEvents(textBrush, EVENT_PRESS);
  SimbleeForMobile.setEvents(rectClean, EVENT_PRESS);
  SimbleeForMobile.setEvents(textClean, EVENT_PRESS);

  SimbleeForMobile.setEvents(speedSlide, EVENT_DRAG | EVENT_RELEASE);

  SimbleeForMobile.endScreen();

  // update sets desired init values
  // (this must be done after endScreen() on startup,
  // otherwise the initial values will fill from the cache)

  update();
}  // end ui

void update() // loop calls this fx, when loop sees needsUpdate = true
// -- (set to true by any input event) -- update() actions could
// have been done in loop. Instead this fx does all changes
{ // both to the board hardware and display on the phone app
  // so will also send cmd bytes from here; sensor query sent in loop.

  // on 1st call, there's no eventId, so this inits all values to defaults
  // if event is from other object, this could assign those values:
  // if (eventId != speedSlide) SimbleeForMobile.updateValue(speedSlide, speed);

  // if only one event can affect var, or I don't care which, update the value
  // SimbleeForMobile.updateValue(britefield, brite);

  // on any update I start control field with speed

  sprintf(cont, "     spd= %d ", speed);  // put int's as chars in array

  // stop any action if center square pressed, slight delay needed
  if (eventId == rectE | eventId == textStop)
  {
    drive(0, 0);
    sprintf(cont, "     drive stopped ");
    byte cmd[] = {START, CONTROL, MODE_SAFE};
    for (int i = 0; i < sizeof(cmd); i++)
    { Serial.write(cmd[i]);
      delay(240);
    }
  }

  // turn power ON -- press toggles action ON/OFF
  else if (eventId == rectPower &&  powerMode == 0)
  { char spin[] = "  pressed power ON ";
    strcat(cont, spin);
    byte cmd[] = {byte(START), byte(CONTROL), byte(MODE_SAFE)};
    //  Serial.write(cmd, 4); // need delay between startup bytes
    for (int i = 0; i < sizeof(cmd); i++)
    { Serial.write(cmd[i]);
      delay(240);
    }
    powerMode = 1;
  }

  // power OFF
  else if (eventId == rectPower &&  powerMode == 1)
  { // if power up already, stop and power down
    memset(cont, 0, 62);
    strcat(cont, "  pressed power OFF");
    powerMode = 0;
    drive(0, 0);
    delay(50);
    // powering down
    Serial.write(POWER);

  }

  // straight forward
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
    strcat(cont, spin);
  }

  else if (eventId == rectF && powerMode)
  {
    char spin[] = " R spin";
    strcat(cont, spin);
    drive(speed, -1);
  }

  else if (eventId == rectA && powerMode)
  {
    char spin[] = " L fwd turn";
    strcat(cont, spin);
    // gentler turns if going fast
    int spd = (speed > 160) ? round(speed * 0.8) : speed;
    drive(spd, 200);
  }

  else if (eventId == rectC && powerMode)
  {
    char spin[] = " R fwd turn";
    strcat(cont, spin);
    int spd = (speed > 160) ? round(speed * 0.8) : speed;
    drive(spd, -200);
  }

  else if (eventId == rectG && powerMode)
  {
    char spin[] = " L bak turn";
    strcat(cont, spin);
    int spd = (speed > 160) ? round(speed * 0.8) : speed;
    drive(spd * (-1), 200);

  }

  else if (eventId == rectI && powerMode)
  {
    char spin[] = " R bak turn";
    strcat(cont, spin);
    int spd = (speed > 160) ? round(speed * 0.8) : speed;
    drive(spd * (-1), -200);
  }

  else if (eventId == rectBrush && powerMode)
  { // power on all motors
    char spin[] = " brush up";
    strcat(cont, spin);
    Serial.write(138); Serial.write(7);
  }

  else if (eventId == rectClean && powerMode)
  {
    char spin[] = " clean up";
    strcat(cont, spin);
    Serial.write(135);
  }

  // if (eventId != txtfielData) (what do I care?)
  SimbleeForMobile.updateText(textControl, cont);

}  // end update

void ui_event(event_t &event)
{ // just resets vars according to event ID; kept small
  // for speed --> farm out most actions to loop & update()
  eventId = event.id; //  # update() uses 4 various var dependent stuff

  if (event.id == speedSlide)
  {
    speed = event.value;
    // green = event.green;
  }
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

  needsUpdate = true;  // when true, loop calls update() to process vars & do stuff
} // end ui_event

void drive(int velo, int radi)
{
  byte cmd[] = {DRIVE, (velo >> 8), (velo & 0xff), (radi >> 8), (radi & 0xff)};

  char temp[28];  // view current control bytes in control textField
  sprintf(temp, " spd %d %d, rad %d %d ", cmd[1], cmd[2], cmd[3], cmd[4]);
  strcat(cont, temp); // for actual: "spd %d rad %", velo, rad
  Serial.write( cmd, 5 );
}  // end drive

String sensorsAsString()
{ // to display more data , would need bigger data[]
  return
    //      "bump:" + // need to put in constant call instead of these aliases
    //      (bumpLeft()?"l":"_") +
    //      (bumpRight()?"r":"_") +
    //      " wheel:" +
    //      (wheelDropLeft()  ?"l":"_") +
    //      (wheelDropCenter()?"c":"_") +
    //      (wheelDropLeft()  ?"r":"_") +
    //      " wall:" + (wall() ?"Y":"n") +
    //      " cliff:" +
    //      (cliffLeft()       ?"l":"_") +
    //      (cliffFrontLeft()  ?"L":"_") +
    //      (cliffFrontRight() ?"R":"_") +
    //      (cliffRight()      ?"r":"_") +
    //      " dirtL:"+ dirtLeft()+
    //      " dirtR:"+ dirtRight()+
    //      " vwal:" + virtual_wall() +
    //      " motr:" + motor_overcurrents() +
    //      " dirt:" + dirt_left() + "," + dirt_right() +
    //      " remo:" + hex(remote_opcode()) +
    //      " butt:" + hex(buttons()) +
    //      " dist:" + distance() +
    //      " angl:" + angle() +
    (" chgState: " + String(sensIn[CHARGINGSTATE] & 0xff) +
     " volt: " + String((sensIn[VOLTAGE_HI] << 8) | (sensIn[VOLTAGE_LO] & 0xff)) +
     " curr: " + String(int16_t((sensIn[CURRENT_HI] << 8) | (sensIn[CURRENT_LO] & 0xff))) +
     " temp: " + String(sensIn[TEMPERATURE] & 0xff) );
  //      " chrg:" + charge() +
  //      " capa:" + capacity() +
  // current needs cast back to signed int, + = charge, - = discharge mA
}  // end sensAsStr

