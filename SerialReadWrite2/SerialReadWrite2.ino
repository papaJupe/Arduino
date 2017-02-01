int highestinput=6;
int nbAIs=2;
int i;
char out[100],in[100],tempstr[100];


void setup() {
  Serial.begin(9600);
  Serial.setTimeout(500);
  for (i=2;i<14;i++){
    if (i<=highestinput){
      pinMode(i, INPUT);
    }
    else{
      pinMode(i, OUTPUT);
    }
  }
}

void loop(){
  out[0]=0;
  strcat(out,"buttons");
  for (i=2;i<=highestinput;i++){
    if (digitalRead(i)==HIGH){
      strcat(out,"1");
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
          digitalWrite(i-2+highestinput,LOW);
        }
        else{
          digitalWrite(i-2+highestinput,HIGH);
        }
      }
    }
  }
  delay(50);  

}

