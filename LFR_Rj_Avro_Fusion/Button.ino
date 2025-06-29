int lswitch() {
  m1 = m2 = millis();
  while (digitalRead(swl) == LOW) {
    m2 = millis();
    if (m2 - m1 > 350) digitalWrite(light, 1);
  }
  digitalWrite(light, 0);
  if (m2 - m1 > 15 && m2 - m1 < 350) return 1;
  else if (m2 - m1 > 350) return 2;
  else return 0;
}

int rswitch() {
  m1 = m2 = millis();
  while (digitalRead(swr) == LOW) {
    m2 = millis();
    if (m2 - m1 > 350) digitalWrite(light, 1);
  }
  digitalWrite(light, 0);
  if (m2 - m1 > 15 && m2 - m1 < 350) return 1;
  else if (m2 - m1 > 350) return 2;
  else return 0;
}
