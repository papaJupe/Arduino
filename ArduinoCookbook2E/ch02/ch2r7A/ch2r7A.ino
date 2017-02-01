/*
 * SplitSplit sketch
 * split a comma-separated string
 */

const int  MAX_STRING_LEN = 20; // set this to the largest string 
                                // you'll process
char stringList[] = "Peter,Paul,Mary";  // an example string

char stringBuffer[MAX_STRING_LEN+1]; // static buffer for computation & output

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  char *str;
  char *p;
  strncpy(stringBuffer, stringList, MAX_STRING_LEN); // copy source string
  Serial.println(stringBuffer);                      // show the source string
  for( str = strtok_r(stringBuffer, ",", &p);        // split using comma
       str;                                          // loop while str is 
                                                     // not null
       str = strtok_r(NULL, ",", &p)                 // get subsequent tokens
     )
  {
    Serial.println(str);
    if(strcmp(str, "Paul") == 0)
      Serial.println("found Paul");
  }
    delay(5000);
}
      
    
