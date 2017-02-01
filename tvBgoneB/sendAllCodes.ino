// this gets the successive codes from list, sends to IRLED flasher

void sendAllCodes() 
{
  bool endingEarly = false; // will be set true if you press TRIGGER button during code-sending 

  // determine region from REGIONSWITCH: 1 = NA, 0 = EU (defined in main.h)
  if (digitalRead(REGIONSWITCH)) {
    region = NA;
    num_codes = num_NAcodes;  // vals come from main.h ?
  }
  else {
    region = EU;
    num_codes = num_EUcodes;
  }

  // for every POWER code in our list
  for (uint8_t i=0 ; i<num_codes; i++) 
  {
    PGM_P data_ptr;

    // print out the code # we are about to transmit
    DEBUGP(putstring("\n\r\n\rCode #: ");
    putnum_ud(i));

    // point to next POWER code, from the right database
    if (region == NA) {
      data_ptr = (PGM_P)pgm_read_word(NApowerCodes+i);
    }
    else {
      data_ptr = (PGM_P)pgm_read_word(EUpowerCodes+i);
    }

    // print out the address in ROM memory we're reading
    DEBUGP(putstring("\n\rAddr: ");
    putnum_uh((uint16_t)data_ptr));

    // Read the carrier frequency from the first byte of code structure
    const uint8_t freq = pgm_read_byte(data_ptr++);
    // set OCR for Timer1 to output this POWER code's carrier frequency
    OCR2A = freq;
    OCR2B = freq / 3; // 33% duty cycle

    // Print out the frequency of the carrier and the PWM settings
    DEBUGP(putstring("\n\rOCR1: ");
    putnum_ud(freq);
    );
    DEBUGP(uint16_t x = (freq+1) * 2;
    putstring("\n\rFreq: ");
    putnum_ud(F_CPU/x);
    );

    // Get the number of pairs, the second byte from the code struct
    const uint8_t numpairs = pgm_read_byte(data_ptr++);
    DEBUGP(putstring("\n\rOn/off pairs: ");
    putnum_ud(numpairs));

    // Get the number of bits we use to index into the timer table
    // This is the third byte of the structure
    const uint8_t bitcompression = pgm_read_byte(data_ptr++);
    DEBUGP(putstring("\n\rCompression: ");
    putnum_ud(bitcompression);
    putstring("\n\r"));

    // Get pointer (address in memory) to pulse-times table
    // The address is 16-bits (2 byte, 1 word)
    PGM_P time_ptr = (PGM_P)pgm_read_word(data_ptr);
    data_ptr += 2;
    code_ptr = (PGM_P)pgm_read_word(data_ptr);


    //DEVELOPMENTAL TESTING: 
//#if 0
//    // print out all of the pulse pairs
//    for (uint8_t k=0; k<numpairs; k++) {
//      uint8_t ti;
//      ti = (read_bits(bitcompression)) * 4;
//      // read the onTime and offTime from the program memory
//      ontime = pgm_read_word(time_ptr+ti);
//      offtime = pgm_read_word(time_ptr+ti+2);
//      DEBUGP(putstring("\n\rti = ");
//      putnum_ud(ti>>2);
//      putstring("\tPair = ");
//      putnum_ud(ontime));
//      DEBUGP(putstring("\t");
//      putnum_ud(offtime));
//    }
//    continue;
//#endif

    // Transmit all codeElements for this POWER code
    // (a codeElement is an onTime and an offTime)
    // transmitting onTime means pulsing the IR emitters at the carrier
    // frequency for the length of time specified in onTime
    // transmitting offTime means no output from the IR emitters for the
    // length of time specified in offTime

      // For EACH pair in this code....
    cli();   // not sure what, maybe system fx
    for (uint8_t k=0; k<numpairs; k++) 
    {
      uint16_t ti;

      // Read the next 'n' bits as indicated by the compression variable
      // Then multiply by 4 because there are 2 timing numbers per pair
      // and each timing number is one word long, so 4 bytes total!
      ti = (read_bits(bitcompression)) * 4;

      // read the onTime and offTime from the program memory
      ontime = pgm_read_word(time_ptr+ti);  // read word 1 -> ontime
      offtime = pgm_read_word(time_ptr+ti+2);  // read word 2 -> offtime
      // transmit this codeElement (ontime and offtime)
      xmitCodeElement(ontime, offtime, (freq!=0));
    }
    sei();  // not sure what, maybe system fx

    // Flush remaining bits, so that next code starts
    // with a fresh set of 8 bits.
    bitsleft_r = 0; //  global, cleared here, reset in read_bits on each call didn't work

    // visible indication that a code has been sent to IRLED
    quickflashLED();

    // delay 205 milliseconds before transmitting next POWER code
    delay_ten_us(20500);

    // if user depresses TRIGGER button, stop transmission early 
    if (digitalRead(TRIGGER) == BUTTON_PRESSED) 
    {
      endingEarly = true;
      delay_ten_us(50000); // 500ms delay 
      quickflashLEDx(4); // this should delay > 1 sec. so next delays not needed?
      //pause for ~1.3 sec to give the user time to release the button so that the code sequence won't immediately start again.
      //      delay_ten_us(MAX_WAIT_TIME); // wait 655.350ms
      //      delay_ten_us(MAX_WAIT_TIME); // wait 655.350ms
      break; //exit the POWER code "for" loop
    } // end if
  } // end :  for every POWER code loop

  if (endingEarly == false)
  {
    //pause for ~1.3 sec, then flash the visible LED 8 times when all codes sent
    delay_ten_us(MAX_WAIT_TIME); // wait 655.350ms
    delay_ten_us(MAX_WAIT_TIME); // wait 655.350ms
    quickflashLEDx(8);
  }
} //end of sendAllCodes
