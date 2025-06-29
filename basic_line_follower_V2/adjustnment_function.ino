void counter_adjust() {
  int p;
  while (digitalRead(switchin) == LOW) {
    p = map(analogRead(9), 50, 1023, 0, 9);
    display.clearDisplay();
    String value = String(p, 10);
    text("COUNTER:", 10, 24);
    text(value, 110, 24);
    display.display();
    if (digitalRead(calin) == HIGH) {
      display.clearDisplay();
      text("BYE!!!", 29, 24);
      display.display();
      return;
    }
  }
  while (digitalRead(switchin) == HIGH) digitalWrite(calout, HIGH);
  digitalWrite(calout, LOW);

  EEPROM.write(10, p);
  delay(10);
  counter = EEPROM.read(10);

  display.clearDisplay();
  text("DONE!!!", 23, 24);
  display.display();
  return;
}


void speed_adjust() {
  int p;
  while (digitalRead(switchin) == LOW) {
    p = map(analogRead(9), 50, 1023, 10, 25);
    display.clearDisplay();
    String value = String(p, 10);
    text("LEFT :", 12, 16);
    text(value, 94, 16);
    display.drawRect(3, 41, 123, 13, WHITE);
    for (int i = 4; i <= (p - 10) * 8 + 4; i++)
      for (int j = 42; j <= 52 ; j++) {
        display.drawLine(4, j, i, j, WHITE);
      }
    display.display();
    if (digitalRead(calin) == HIGH) {
      display.clearDisplay();
      text("BYE!!!", 29, 24);
      display.display();
      return;
    }
  }
  EEPROM.write(6, p);
  delay(10);
  spl = EEPROM.read(6);

  while (digitalRead(switchin) == HIGH) digitalWrite(calout, HIGH);
  digitalWrite(calout, LOW);

  while (digitalRead(switchin) == LOW) {
    p = map(analogRead(9), 50, 1023, 10, 25);
    display.clearDisplay();
    String value = String(p, 10);
    text("RIGHT :", 7, 16);
    text(value, 94, 16);
    display.drawRect(3, 41, 123, 13, WHITE);
    for (int i = 4; i <= (p - 10) * 8 + 4; i++)
      for (int j = 42; j <= 52 ; j++) {
        display.drawLine(4, j, i, j, WHITE);
      }
    display.display();
    if (digitalRead(calin) == HIGH) {
      display.clearDisplay();
      text("OKAY!!!", 23, 24);
      display.display();
      return;
    }
  }

  while (digitalRead(switchin) == HIGH) digitalWrite(calout, HIGH);
  digitalWrite(calout, LOW);

  display.clearDisplay();
  text("DONE!!!", 23, 24);
  display.display();

  EEPROM.write(7, p);
  delay(10);
  spr = EEPROM.read(7);
}



void servo_adjust() {
  while (digitalRead(switchin) == LOW) {
    gpos = map(analogRead(9), 50, 1023, 65, 140);
    grab.write(gpos);
    display.clearDisplay();
    text("GRAB:", 17, 24);
    String value = String(gpos, 10);
    text(value, 75, 24);
    display.display();
    if (digitalRead(calin) == HIGH) {
      display.clearDisplay();
      text("BYE!!!", 23, 24);
      display.display();
      return;
    }
  }

  EEPROM.write(9, gpos);
  delay(10);

  while (digitalRead(switchin) == HIGH) digitalWrite(calout, HIGH);
  digitalWrite(calout, LOW);

  while (digitalRead(switchin) == LOW) {
    hpos = map(analogRead(9), 50, 1023, 10, 100);
    handle.write(hpos);
    display.clearDisplay();
    text("HANDLE:", 5, 24);
    String value = String(hpos, 10);
    text(value, 87, 24);
    display.display();
    if (digitalRead(calin) == HIGH) {
      display.clearDisplay();
      text("OKAY!!!", 29, 24);
      display.display();
      return;
    }
  }
  EEPROM.write(8, hpos);
  delay(10);

  while (digitalRead(switchin) == HIGH) digitalWrite(calout, HIGH);
  digitalWrite(calout, LOW);

  display.clearDisplay();
  text("DONE!!!", 23, 24);
  display.display();
}
