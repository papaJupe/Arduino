/* Monk's sketch 5-05 morse reader, modified to play tone thru pin X;
 enter char string into serial monitor, plays morse equivalent by tone and led,
 delay 4 sec before each new char string plays; then added
 more code to make it play random strings if switch tells it to;
 Rand1 made an array using for, then played that in another for loop;
 this is simpler and adds ser. mon. readout of each 7 letter word;
 consider adding pot control of speed, serial readout of char/min
 */
 
#define toneOut 8 
#define ledPin  13
#define dotDelay 150    // all speed params derive from this value, could make adjustable

char* letters[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",    // A-I
  ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",  // J-R
  "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."          // S-Z
};

char* numbers[] = {
  "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."};

void setup()                 
{   
  pinMode(A0, INPUT);  //makes no difference if here or not, gets floating voltage
  pinMode(15, INPUT);  // same as A1, will be high unless switch pulls it low
  digitalWrite(15, HIGH);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600); 
}

void loop()                    
{
  char ch;                // to hold indiv char to play from serial input
  static boolean var=0;  // just set it once when beginning
                         //otherwise if/while condx will delay each char
  if (!Serial.available())  // nothing in the buffer
       {    
       var=0;      // ser.avail will be > 0 while characters can be read from buffer;
                   // this resets var to 0 when buffer empties again
       }
  if (Serial.available())   //same as (Ser.avail() > 0), value = # of bytes being held in buffer
  
 {                         // if/while condition pauses 4 sec
        if (var == 0)      // after new serial data comes
        {                  // before starting output
        delay(4000);
        var = 1;
        }
      ch = Serial.read();   //get char one by one from buffer, will be ascii value of char
  
      if (ch >= 'a' && ch <= 'z')           // 'z' is ascii value of z
      {    
        flashSequence(letters[ch - 'a']);  // so this gives index of ch in letter array
      }
      else if (ch >= 'A' && ch <= 'Z')
      {
        flashSequence(letters[ch - 'A']);
      }
      else if (ch >= '0' && ch <= '9')
      {
        flashSequence(numbers[ch - '0']);
      }
      else if (ch == ' ')
      {
        delay(dotDelay*4);  // gap between words  
      }
  }  //end if Ser.avail
    
  if (digitalRead(15) == LOW)   // switch is closed to activate random word generation
  { 
    int floteV = analogRead(0);    // should range 0 to 1024
    randomSeed(floteV);     //each 'for' loop below will get new seed
//    Serial.print("seed is: "), Serial.println(floteV); //used to debug
  
    for (byte i=0; i<7; i++)   // make 7 character words of random #s
    {                        // random must be inside 'for' block to give you a sequence ?
 
       char ch = random(30);  // put random # 0-29 in ch
       
        if (ch <= 25)  // one of the 26 letters
        {    
          flashSequence(letters[ch]);
          Serial.write(ch + 'A'), Serial.write(32);  // print the char + space
        }            // to print capital letter A-Z must add ascii value of A to ch value
                      // ser.print just prints the decimal # instead of its char value
                      // not sure why since ch is char type; maybe adding 'A' makes it an int?
                      
        else if (ch >= 26  && ch <= 29)  //  4 slots for designating numbers
        {
          byte randY = random(0,256); //random(0,9) gave mostly 5
          randY = (randY % 10);   //want a random digit 0-9, hard getting it
          // Serial.print("randY is: "), Serial.println(randY); //for debugging
          flashSequence(numbers[randY]);           
        Serial.print(randY), Serial.write(32);  // print the # + space; Ser.print(32) prints 32
        }  // end else                        //randY is a byte so .print shows its numeric value
    }  // end 'for' word generation               
       delay(dotDelay*4);  // gap between 7 letter words
       Serial.println();
 } // end if switch on, random generation
}   // end loop
  
void flashSequence(char* sequence)  //not sure if * means seq. is a pointer to array element?
  {
    int i = 0;   //read thru char array until NULL reached
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
   tone(toneOut, 2300);   // don't seem to need pin config
  if (dotOrDash == '.')
  {
    delay(dotDelay);           
  }
  else // must be a dash
  {
    delay(dotDelay * 3);           
  }
  digitalWrite(ledPin, LOW);
  noTone(toneOut);  
  delay(dotDelay); // gap between flashes
}



