
/*
 This fx uses hard coded AVR chip timers --> on D3 <-- must use
 This function is the 'workhorse' of transmitting IR codes.
 Given the on and off times, it turns the PWM output on and off
 to generate one 'pair' from a long code. Each code has ~50 pairs!
 [params renamed to clarify they are local to this fx] 
 */
void xmitCodeElement(uint16_t ontyme, uint16_t offtyme, uint8_t PWM_code)
{
  TCNT2 = 0;
  if(PWM_code) {
    // pinMode(IRLED, OUTPUT);  // why put in fx, already in setup ?
    // Fast PWM, setting top limit, divide by 8
    // Output to pin 3
    TCCR2A = _BV(COM2A0) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
    TCCR2B = _BV(WGM22) | _BV(CS21);
  }
  else 
  {
    // However some codes don't use PWM in which case we just turn on the 
    // IRLED for the on period.
    digitalWrite(IRLED, HIGH);
  }

  // Now we wait, allowing the PWM hardware to pulse out the carrier
  // frequency for the specified 'on' time
  delay_ten_us(ontyme);

  // Now we have to turn it off, so first disable the PWM output
  TCCR2A = 0;
  TCCR2B = 0;
  // And make sure that the IR LED is off (since the PWM timer may have
  // stopped while the LED was on!)
  digitalWrite(IRLED, LOW);

  // Now we wait for the specified 'off' time
  delay_ten_us(offtyme);
}  // end xmitCodeElement
