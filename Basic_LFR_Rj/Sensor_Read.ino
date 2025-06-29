void reading() {

  sensor = 0;
  sum = 0;

  for (int i = 0; i < 6; i++) {
    s[i] = analogRead(14+i);
    (s[i] > mid[i]) ? s[i] = 1 : s[i] = 0;
    sensor += s[i] * base[i];
    sum += s[i];
  }
}

void show_digital() {
  reading();
  for (int i = 5; i >= 0; i--) {
    Serial.print(s[i]);
    Serial.print(" ");
  }
  Serial.println(sensor, BIN);
}

void show_analog() {
  for (int i = 5; i >= 0; i--) {
    s[i] = analogRead(i);
    Serial.print(s[i]);
    Serial.print(" ");
  }
  Serial.println();
}
