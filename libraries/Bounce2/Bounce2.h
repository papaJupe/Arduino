
/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
  Main code by Thomas O Fredericks (tof@t-o-f.info)
  Previous contributions by Eric Lowry, Jim Schimpf and Tom Harkaway
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef BOUNCE_LOCK
#error You are using the invalid BOUNCE_LOCK-OUT define. Please update your sources to use BOUNCE_LOCK_OUT
#endif

// Uncomment the following line for "LOCK-OUT" debounce method
//#define BOUNCE_LOCK_OUT


#ifndef Bounce2_h
#define Bounce2_h

#include <inttypes.h>

class Bounce
{
 public:
    // Create an instance of the bounce class
    Bounce();

    // Attach to a pin (and also sets initial state)
    void attach(int pin);

    // Sets the debounce interval
    void interval(uint16_t interval_millis);

    // Updates the pin
    // Returns 1 if the state changed
    // Returns 0 if the state did not change
    bool update();

    // Returns the updated pin state
    bool read();

    // Returns the falling pin state
    bool fell();

    // Returns the rising pin state
    bool rose();

 protected:
    unsigned long previous_millis;
    uint16_t interval_millis;
    uint8_t state;
    uint8_t pin;
};

#endif
