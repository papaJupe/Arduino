#include "Arduino.h"
#include "LCD_DFR_Keypad.h"

static int DEFAULT_KEY_PIN = 0;
static int DEFAULT_THRESHOLD = 20;

static int NOKEY_ARV = 1023;  // arv means "analog read value"
static int SELKEY_ARV = 638;
static int LEFTKEY_ARV = 407;  // these set for my 1602 board
static int DOWNKEY_ARV = 255;
static int UPKEY_ARV = 99;
static int RIGHTKEY_ARV = 0;

DFR_Key::DFR_Key()
{
  _refreshRate = 10;
  _keyPin = DEFAULT_KEY_PIN;  // analog pin they all feed
  _threshold = DEFAULT_THRESHOLD;
  _keyIn = NO_KEY;
  _curInput = NO_KEY;
  _curKey = NO_KEY;
  _prevInput = NO_KEY;
  _prevKey = NO_KEY;
  _oldTime = 0;
}

int DFR_Key::getKey()
{
 if (millis() > _oldTime + _refreshRate)
 {
    _prevInput = _curInput;
    _curInput = analogRead(_keyPin);

    if (_curInput == _prevInput)  // keep current value
    {
      _change = false;
      _curKey = _prevKey;
    }
    else
    {
      _change = true;
      _prevKey = _curKey;

/*
      if (_curInput > UPKEY_ARV - _threshold && _curInput < UPKEY_ARV + _threshold ) _curKey = UP_KEY;
      else if (_curInput > DOWNKEY_ARV - _threshold && _curInput < DOWNKEY_ARV + _threshold ) _curKey = DOWN_KEY;
      else if (_curInput > RIGHTKEY_ARV - _threshold && _curInput < RIGHTKEY_ARV + _threshold ) _curKey = RIGHT_KEY;
      else if (_curInput > LEFTKEY_ARV - _threshold && _curInput < LEFTKEY_ARV + _threshold ) _curKey = LEFT_KEY;
      else if (_curInput > SELKEY_ARV - _threshold && _curInput < SELKEY_ARV + _threshold ) _curKey = SELECT_KEY;
      else _curKey = NO_KEY;
  */
    // simpler & more efficient than above
    if (_curInput > 980 ) _curKey = NO_KEY;
        else if (_curInput > SELKEY_ARV - _threshold) _curKey = SELECT_KEY;
        else if (_curInput > LEFTKEY_ARV - _threshold) _curKey = LEFT_KEY;
        else if (_curInput > DOWNKEY_ARV - _threshold) _curKey = DOWN_KEY;
        else if (_curInput > UPKEY_ARV - _threshold) _curKey = UP_KEY;
        else if (_curInput < 50 ) _curKey = RIGHT_KEY;
    }  // end else

   if (_change) return _curKey; else return SAMPLE_WAIT;

   _oldTime = millis();
 }  // end if millis
}  // end getKey

void DFR_Key::setRate(int rate)
{
  _refreshRate = rate;
}