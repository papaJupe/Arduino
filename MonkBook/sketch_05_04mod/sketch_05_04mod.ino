// sketch 5-04 modified to better explain pointer syntax

char* message = "Hello";  
    // message is pointer to char array; no braces needed for pointer; spacing is optional
    //can be char * message or char*message or char *message
    
void setup()
{
  Serial.begin(9600);
}

void loop()
{                            // with above declaration of type,
  Serial.println(message);  // if you put pointer here, it just prints 1st letter
  delay(1000);
}

/*
char message[] = {"Hello"};  
    // braces optional for string array declaration
    // works with (message) as var below, but if you say char * message[] =
    // then you need (*message) below
void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Serial.println(message);   //if pointer array declared, then you need pointer here
  delay(1000);              // neither coding prints quotation marks of word !
}
*/
