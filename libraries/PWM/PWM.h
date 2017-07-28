/*
Copyright (c) 2012 Sam Knight

*/

/*
This library is built to support two of the AVR Architecture 'groups' that Arduino uses
a) ATmega48/88/168/328,
b) ATmega640/1280/1281/2560/2561
*/

#ifndef PWM_H_
#define PWM_H_

#include "avr/pgmspace.h"
#include "math.h"

#if defined(__AVR_ATmega640__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__)
	#include "utility/ATimerDefs.h"
#elif defined(__AVR_ATmega48__) || defined(__AVR_ATmega88__) || defined(__AVR_ATmega88P__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
	#include "utility/BTimerDefs.h"
#endif


#if defined(__AVR_ATmega640__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__)
// 16 bit timers
extern uint32_t	GetFrequency_16(const int16_t timerOffset);
extern bool		SetFrequency_16(const int16_t timerOffset, uint32_t f);
extern uint16_t GetPrescaler_16(const int16_t timerOffset);
extern void		SetPrescaler_16(const int16_t timerOffset, prescaler psc);
extern void		SetTop_16(const int16_t timerOffset, uint16_t top);
extern uint16_t GetTop_16(const int16_t timerOffset);
extern void		Initialize_16(const int16_t timerOffset);
extern float	GetResolution_16(const int16_t timerOffset);

// 8 bit timers
extern uint32_t	GetFrequency_8(const int16_t timerOffset);
extern bool		SetFrequency_8(const int16_t timerOffset, uint32_t f);
extern uint16_t GetPrescaler_8(const int16_t timerOffset);
extern void		SetPrescaler_8(const int16_t timerOffset, prescaler psc);
extern void		SetPrescalerAlt_8(const int16_t timerOffset, prescaler_alt psc);
extern void		SetTop_8(const int16_t timerOffset, uint8_t top);
extern uint8_t	GetTop_8(const int16_t timerOffset);
extern void		Initialize_8(const int16_t timerOffset);
extern float	GetResolution_8(const int16_t timerOffset);

#endif

#if defined(__AVR_ATmega48__) || defined(__AVR_ATmega88__) || defined(__AVR_ATmega88P__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)

// 16 bit timers
extern uint32_t	GetFrequency_16();
extern bool		SetFrequency_16(uint32_t f);
extern uint16_t GetPrescaler_16();
extern void		SetPrescaler_16(prescaler psc);
extern void		SetTop_16(uint16_t top);
extern uint16_t GetTop_16();
extern void		Initialize_16();
extern float	GetResolution_16();

// 8 bit timers
extern uint32_t	GetFrequency_8(const int16_t timerOffset);
extern bool		SetFrequency_8(const int16_t timerOffset, uint32_t f);
extern uint16_t GetPrescaler_8(const int16_t timerOffset);
extern void		SetPrescaler_8(const int16_t timerOffset, prescaler psc);
extern void		SetPrescalerAlt_8(const int16_t timerOffset, prescaler_alt psc);
extern void		SetTop_8(const int16_t timerOffset, uint8_t top);
extern uint8_t	GetTop_8(const int16_t timerOffset);
extern void		Initialize_8(const int16_t timerOffset);
extern float	GetResolution_8(const int16_t timerOffset);

#endif

//common functions

extern void		InitTimers();
extern void		InitTimersSafe();										//doesn't init timers responsible for time keeping functions
extern void		pwmWrite(uint8_t pin, uint8_t val);
extern void		pwmWriteHR(uint8_t pin, uint16_t val);					//accepts a 16 bit value and maps it down to the timer for maximum resolution
extern bool		SetPinFrequency(int8_t pin, uint32_t frequency);
extern bool		SetPinFrequencySafe(int8_t pin, uint32_t frequency);	//does not set timers responsible for time keeping functions
extern float	GetPinResolution(uint8_t pin);							//gets the PWM resolution of a pin in base 2, 0 is returned if the pin is not connected to a timer

#endif /* PWM_H_ */