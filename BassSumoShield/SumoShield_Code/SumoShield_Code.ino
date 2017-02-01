/*
    SumoShield_Code.ino
    Copyright 2011 Solarbotics Ltd.

    Written by Jeremie Boulianne 2011
    Ported from .C code by Bob Cook, based on the original by Grant McKee
    Cleaned up and revised by Dan Gates 2006
    Assistance from: Mark Martens

    Designed for Rev 1.0 SumoShield.
    Intended to be compiled with Arduino 1.0 (or later).

    Solarbotics: http://www.solarbotics.com/
    Arduino    : http://www.arduino.cc/

    This example code is in the public domain.

    Description: Minisumo/Linefollower program Ver 1.0

    If outside edge sensors see black during startup the program will branch
    to sumo after a 5 second delay, indicated by the green LEDS. 
    If outside edge sensors see white during startup the program will
    branch to Linefollower.

    Sumo mode is fairly basic:

        - Wait 5 seconds before moving
        - After 5 seconds go straight forward
        - If a white line is detected on outside Left sensor
            * Reverse both motors briefly
            * Stop left motor
            * Reverse Right motor
            * After set time continue straight forward
        - If a white line is detected on outside Right sensor
            * Reverse both motors briefly
            * Stop Right motor
            * Reverse Left motor
            * After set time continue straight forward
        - If opponent detected on right Side
            * Stop Right motor
        - If opponent detected on Left Side
            * Stop Left motor
        - If both sensors detect opponent
           * Turn on both motors forward
      -- No effective search programmed
    Linefollower mode:

        - Start immediately
        - If center sensor sees black all is well- go straight forward
        - If center right sensor sees black, make a gentle right turn by
          slowing down right motor
        - If center left sensor sees black, make a gentle left turn by slowing
          down left motor
        - If far right sensor sees black, make a very sharp right turn by
          reversing right motor
        - If far left sensor sees black, make a very sharp left turn by
          reversing left motor
        - If all is white, all is lost - go looking for the line -- but does he?

 */

/*
 *  Definitions
 *
 */

/* White line sensitivity; higher is more sensitive (1024 max) */
int kWhiteThreshold = 128; //Original value 128
int linesensor_left;    //why waste an int on these when they will just be on or off ?
int linesensor_midleft;
int linesensor_center;
int linesensor_midright;  // 4 IR sensors defined here, but not given input pins below?
int linesensor_right;
int irsensors;   // this will only go 0 to 4 so why an int?
int Pd0;
int Pd1;     //same for these, will be boolean, right?


void setup(){
  
pinMode(7, INPUT); /* Front IR sensor inputs will get 0 or 1, read by Pd0-1 variables*/
pinMode(8, INPUT);  // opponent detection in Sumo mode only

/* the SumoShield has five green leds that we can use to display things in our 
   program; we need to set the LED pins as outputs for them to turn on*/

pinMode(2, OUTPUT);                      /* Pins 2-6 = green leds */
pinMode(3, OUTPUT); 
pinMode(5, OUTPUT);
pinMode(4, OUTPUT); 
pinMode(6, OUTPUT); 
    
/* the sumovore has two motors that also require output pins */

pinMode(9, OUTPUT);              // enable for L motor ? these are PWM capable      
pinMode(10, OUTPUT);             // enable for R motor ?
pinMode(12, OUTPUT);             // L motor HIGH = forward, LOW =reverse
pinMode(13, OUTPUT);             //  this 1 pin controls R motor


}

/*
 *  Main (program entry point)
 *
 */

void loop()
{
    
/* make sure all LEDs are off until we're ready */

    digitalWrite(2, LOW);
    digitalWrite(3, LOW);	              /* green LEDs off */
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);


/* make sure motors are off until we're ready. */

	digitalWrite(9, LOW);                  /* right motor disable */
	digitalWrite(10, LOW);	              /* left motor disable */

    /* read the line sensors to decide whether we should behave like a
       sumo wrestler or like a line chaser */

    linesensor_left  = analogRead(A0);  //~34 on white ~700+ on black  threshold 128
    linesensor_right = analogRead(A4);  //~34 on white ~700+ on black

    if ( linesensor_left  < kWhiteThreshold && linesensor_right < kWhiteThreshold  )
    {
        /* both line sensors see white */

        LINEFOLLOWER();
      
    }
    else
    {
        /* one or both line sensors see black */

        SUMO();
    }

    /* this point of the program is never reached, but we satisfy the
       compiler's warnings by saying we'd return zero */

    return;
}


/**********************************************************************
 *  SUMO
 *
 *  Find and push our opponent out of the ring!
 *
 */

void SUMO()
{
  
   /* start with a five second countdown */

    digitalWrite(2, HIGH); 		/* turn on LED 1 */
    delay(650);    	 	           
    
    digitalWrite(3, HIGH);		/* turn on LED 2 */
    delay(650);                        /* wait 1000 milliseconds = 2nd second */
           		
    digitalWrite(4, HIGH);		/* turn on LED 3 */
    delay(650);                          /* another 1000 milliseconds = 3rd second */
          		
    digitalWrite(5, HIGH);		/* turn on LED 4 */
    delay(650);                          /* another 1000 milliseconds = 4th second */
           		
    digitalWrite(6, HIGH);		/* turn on LED 5 */
    delay(650);                         /* another 1000 milliseconds = 5th second */
         		

    /* match begins! turn off all the leds */

    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);			        /* all LEDs off */
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);

    /* turn on both motors going full forward */

    digitalWrite(9, HIGH);    				/* left  motor enable  */
    digitalWrite(10, HIGH);     			/* right motor enable  */
    digitalWrite(12, HIGH);    				/* left  motor forward */
    digitalWrite(13, HIGH); 				/* right motor forward */

    /* loop here forever, chasing our opponent out of the ring */

    while(1)
    {
      /* read the line sensors to see if we've hit the ring edge */

         linesensor_left  = analogRead(A0);
         linesensor_right = analogRead(A4);  // these are outer sensors L and R

        if ( linesensor_right  < kWhiteThreshold )
        {
            /* ring edge seen on right side; reverse both motors to prevent
               sliding over the edge */

            digitalWrite(12, LOW);   //why not stop this left motor by disabling
            digitalWrite(13, LOW);   // and just reverse this R one briefly then reverse both
            delay(300); 

            /* now turn around fast by making the right motor go forward again */

            digitalWrite(13, HIGH);
            delay(400);
        }

        if ( linesensor_left  < kWhiteThreshold )
        {
            /* ring edge seen on left side; reverse both motors to prevent
               sliding over the edge */

            digitalWrite(12, LOW);    
            digitalWrite(13, LOW); // use same idea, back first with L, then both, then turn
            delay(300);

            /* now turn around fast by reversing the left motor  to forward */

            digitalWrite(12, HIGH);
            delay(400);
        }

        /* read the forward-looking sensors looking for our opponent, but no looking is programmed */
        
        Pd0 = digitalRead(7);
        Pd1 = digitalRead(8);
        
        irsensors = ((Pd0 * Pd1) + Pd0 + (Pd1 * 2));  // clever way to generate 4 values from 2 variations
        //irsensors = (Pd0 & Pd1); /* just Pd0, Pd1  would just give you 0 or 1 */

        switch ( irsensors )
        {
            case 0:     //  both = 0
            {
                /* both sensors see something; full steam ahead! */

                digitalWrite(2, HIGH);        /* turn on LED 1 */
                digitalWrite(6, HIGH);       /* turn on LED 5 */

                digitalWrite(9, HIGH);      /* left  motor enable  */
                digitalWrite(10, HIGH);     /* right motor enable  */
                digitalWrite(12, HIGH);     /* left  motor forward */
                digitalWrite(13, HIGH);     /* right motor forward */
                delay(50);
                break;
            }

            case 1:
            {
                /* opponent in the right sensor Pd0=1, Pd1=0 */

                digitalWrite(2, LOW);      /* turn off LED 1 */
                digitalWrite(6, HIGH);     /* turn on  LED 5 */

                digitalWrite(10, LOW);     /* right motor disable */
                
                digitalWrite(9, HIGH);    /* left motor enable  */
                digitalWrite(12, HIGH);   /* left motor forward */
                delay(50);
                break;
            }

            case 2:
            {
                /* opponent in the left sensor Pd0=0 Pd1=1 , turn to L */

                digitalWrite(2, HIGH);       /* turn on  LED 1 */
                digitalWrite(6, LOW);       /* turn off LED 5 */

                digitalWrite(9, LOW);      /* left  motor disable */

                digitalWrite(10, HIGH);     /* right motor enable  */
                digitalWrite(13, HIGH);     /* right motor forward */
                delay(50);
                break;
            }

            case 4:
            {
                // neither sensor sees anything both = 1
                     // shouldn't he be turning slowly to search?

                digitalWrite(2, LOW);      // turn off LED 1
                digitalWrite(6, LOW);      // turn off LED 5

                digitalWrite(9, HIGH);     // left  motor enable  
                digitalWrite(10, HIGH);    // right motor enable  
                digitalWrite(12, HIGH);    // left  motor forward 
                digitalWrite(13, HIGH);    // right motor forward
                delay(50);  
                break;
            }
           
        }
        
        /* loop back to check sensors again */
    }
    /* NEVER REACHED */
}


/*
 *  LINEFOLLOWER
 *
 *  Chase the black line as fast as we can!
 *
 */

void LINEFOLLOWER()
{
    

/* set up Timer1 (pins 9 and 10) to provide PWM output to adjust speed using enable pins */

  
    /* loop here forever, following the black line */

    for ( ;; )
    {
        

        /* read the current sensor values */
        
        linesensor_left     = analogRead(A0);  // don't need to specify as inputs
        linesensor_midleft  = analogRead(A1);
        linesensor_center   = analogRead(A2);
        linesensor_midright = analogRead(A3);
        linesensor_right    = analogRead(A4);

        /* check the outer right sensor */

        if ( linesensor_right < kWhiteThreshold )
        {
            /* seeing white on this sensor, OK no change */

            digitalWrite(6, HIGH); /* turn on LED 5 */
        }
        else
        {
            /* seeing black on this sensor -- pointing very leftward or crossing intersection */
              // should check if L sensor also black before reversing R motor
            digitalWrite(6, LOW); /* turn off LED 5 */
            digitalWrite(13, LOW); /* left motor reverse -- no, 13 is R motor*/
            delayMicroseconds(1300);
        }

        /* check the middle-right sensor */

        if ( linesensor_midright < kWhiteThreshold )
        {
            /* seeing white on this sensor */

            digitalWrite(5, HIGH); /* turn on LED 4 */
        }
        else
        {
            /* seeing black on this sensor  -- slightly pointing L ?*/

            digitalWrite(5, LOW); /* turn off LED 4 */
            analogWrite(10, 127);      // gives 50% pulse width to R enable pin, half speed?
            delayMicroseconds(1300);
        }

        /* check the center sensor */

        if ( linesensor_center < kWhiteThreshold )
        {
            /* seeing white on this sensor  -- something very wrong here */

            digitalWrite(4, HIGH); /* turn on LED 3 */
        }
        else
        {
            /* seeing black on this sensor , right on line */

            digitalWrite(4, LOW); /* turn off LED 3 */

            /* both motors forward at max speed */

            digitalWrite(12, HIGH); // Left motor on
            digitalWrite(13, HIGH);  // R motor on
            analogWrite(10, 255);   // R motor full pulse width
            analogWrite(9, 255);   // 100% pulse width to both motors enable pins
            delayMicroseconds(1300);
        }

        /* check the left-middle sensor */

        if ( linesensor_midleft < kWhiteThreshold )
        {
            /* seeing white on this sensor  -  OK don't do anything */

            digitalWrite(3, HIGH); /* turn on LED 2 */
        }
        else
        {
            /* seeing black on this sensor -- slightly pointing R ?*/

            digitalWrite(3, LOW); /* turn off LED 2 */
            analogWrite(9, 127);            /* slow down left side, half voltage to L motor */
            delayMicroseconds(1300);
        }

        /* check the outer left sensor */

        if ( linesensor_left < kWhiteThreshold )
        {
            /* seeing white on this sensor  OK*/

            digitalWrite(2, HIGH);  /* turn on LED 1 */
        }
        else
        {
            /* seeing black on this sensor, might be very R pointing or just crossing intersection? */

            digitalWrite(2, LOW);     /* turn off LED 1 */
            digitalWrite(12, LOW);   /* left motor reverse */
            delayMicroseconds(1300);
        }

        /* loop back to check sensors again */
    }
    /* NEVER REACHED */
}

/************************************ END ***************************************/

