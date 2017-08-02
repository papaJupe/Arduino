int highestInput=6; // highest digital pin to use
int nbAIs=2; // number of analog inputs
int i;
char out[100],in[100],tempstr[100];


void setup() {
  Serial.begin(9600);
  Serial.setTimeout(500);
  for (i=2;i<14;i++){  // usable DI are D2-D13
    if (i<=highestInput){  // set pins D2-highest
      pinMode(i, INPUT);  // input_pullup would be better to avoid need for pulldown resistors
    }
    else{
      pinMode(i, OUTPUT);
    }
  }
}  // end setup

void loop(){
  out[0]=0;  // this just puts a numeric 0 (or null?) in 1st slot of array, doesn't clear all values
  strcat(out,"buttons"); // but all slots are rewritten below, so it works
  for (i=2;i<=highestinput;i++){
    if (digitalRead(i)==HIGH){ // assumes pressed button will pull pin high instead of low
      strcat(out,"1");  // not reliable without pulldown on each pin
    }
    else{
      strcat(out,"0");
    }
  }
  strcat(out,"AIs");
  for (i=0;i<nbAIs;i++){
    sprintf(tempstr,"%d;",analogRead(i));   
    strcat(out,tempstr);    
  }
  Serial.println(out);

  Serial.readBytes(in,Serial.available());
  if (strlen(in)>0){
    if (in[0]=='L' && in[1]=='E' && in[2]=='D'){ 
      for (i=3;i<=strlen(in);i++){
        if (in[i]=='0'){
          digitalWrite(i-2+highestInput,LOW);
        }
        else{
          digitalWrite(i-2+highestInput,HIGH);
        }
      }
    }
  }
  delay(50);  

} // end loop

