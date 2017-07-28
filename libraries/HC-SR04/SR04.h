#ifndef SR04_H
#define SR04_H

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif
//#include "pins_arduino.h"

#include <inttypes.h>

#define PULSE_TIMEOUT 150000L	// 100ms
#define DEFAULT_DELAY 10
#define DEFAULT_PINGS 5
class SR04 {
public:
	
	/**
	* Constructor
	* Ultrasonic sensor SR04, four pins
	* VCC, TRIGGER, ECHO, GND
	* <br>
	* \param echoPin digital input pin for measuring distance
	* \param triggerPin if high for 10us a trigger signal is generated at SR04
	*/
	SR04(int echoPin, int triggerPin);

	/**
	* Dist function performs measurement. Returns distance as long
	* int in centimeters
	*/
	long Distance();
	
	/**
	* Do 'count' measurents and calculate the average. 
	* To avoid defilement from low/high peaks, min/max values
	* are subtracted from the values used for average
	*
	* \param wait delay between measurements, default = DEFAULT_DELAY ms
	* \param count number of measurements, default DEFAULT_PINGS
	* \return long distance in centimeter
	**/
	long DistanceAvg(int wait=DEFAULT_DELAY, int count=DEFAULT_PINGS);
	
	/**
	* Do only a ping. Get result with method getDistance()
	* \param keine
	*/
	void Ping() ;
	
	/**
	* return latest distance. Method Ping() should be called before
	* \param keine  auf Deutsch, wie andere Worte hierein
	* \return Distanz in Zentimeter
	*/
	long getDistance();
	
private:
	/**
	* Do the measurement calculation and return result in centimeter
	* SR04 measure echo time to obstacle and back. 
	* <br>
	* Sound travels 340m/sec
	* <br>
	* Example: Obstacle 100cm away, pulse goes 100cm to
	* obstacle and 100cm return = 200cm
	* <br>
	* 1sec = 1000ms = 1.000.000uS
	* 1.000.000 / 340 = time in microseconds sound goes 100cm (1m)
	* 2941uS fuer 100cm = 5882 uS fuer 200cm, round trip uS time / 100cm
	*
	* (duration / 5882) * 100 = distance in cm
	*/	
	long MicrosecondsToCentimeter(long duration);
	
	long _currentDistance;
	int _echoPin, _triggerPin;
	long _duration, _distance;
	bool _autoMode;
};
#endif



