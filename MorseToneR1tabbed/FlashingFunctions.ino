
void flashSequence(char* sequence)  
/* means seq. is a pointer of type char, i.e. an address block that holds, in this
 case, a string literal (char array). Or should it be flashSequence(char *sequence)? 
 actually both mean same thing ?
*/
{
  int i = 0;   
  //read thru 'char array'(?) until NULL reached
  while (sequence[i] != NULL)
  {                      //each i sends a dot or dash to flasher/toner
    flashDotOrDash(sequence[i]);
    i++;
  }
  delay(dotDelay*3);    // gap between letters
}

void flashDotOrDash(char dotOrDash)
{
  digitalWrite(ledPin, HIGH);
  tone(tonePin, 2300);   // don't seem to need pin config
  if (dotOrDash == '.')
  {
    delay(dotDelay);           
  }
  else // must be a dash [so comma or any other char becomes a dash]
  {
    delay(dotDelay * 3);           
  }
  digitalWrite(ledPin, LOW);
  noTone(tonePin);  
  delay(dotDelay); // gap between flashes
}

