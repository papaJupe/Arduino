int i;
char out[100], in[100], tempstr[100];

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(500);
  for (i = 2; i < 18; i++) {
    pinMode(i, INPUT_PULLUP);
  }
}

void loop() {
  out[0] = 0;
  strcat(out, "buttons");
  for (i = 2; i < 18; i++) {
    if (digitalRead(i) == HIGH) {
      strcat(out, "0");
    }
    else {
      strcat(out, "1");
    }
  }
  Serial.println(out);
  delay(50);
}


