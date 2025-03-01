#ifndef DFR_Key_h
#define DFR_Key_h     // put in by IDE, ? to match class name

#include "Arduino.h"

#define SAMPLE_WAIT -1
#define NO_KEY 0
#define SELECT_KEY 1
#define LEFT_KEY 2
#define UP_KEY 3
#define DOWN_KEY 4
#define RIGHT_KEY 5

class DFR_Key
{
  public:
    DFR_Key();
    int getKey();
    void setRate(int);
  private:
    int _refreshRate;
    int _keyPin;
    int _threshold;
    int _keyIn;
    int _curInput;
    int _curKey;
    int _prevInput;
    int _prevKey;
    boolean _change;
    unsigned long _oldTime;
};

#endif