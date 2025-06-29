void text(String t, byte x, byte y)
{
  oled.setCursor(x, y);
  oled.print(t);
}

void home_screen()
{
  oled.set2X();
  oled.clear();
  oled.setFont(Arial_bold_14);
  text("ATOM", 22, 0);
  oled.setFont(Iain5x7);
  oled.set1X();
  text("nano", 103, 0);
  text("Developed by ", 5, 4);
  oled.setFont(lcd5x7);
  text("\"Team-Void\"", 60, 4);
  oled.setFont(font8x8);
  text("SUST", 50, 5);
  oled.setFont(Stang5x7);
  text(" Path           Menu ", 1, 7);
  oled.set2X();
}

void analogReadDisp()
{
  byte i;
  oled.set2X();
  oled.clear();
  while (digitalRead(swl) == HIGH && digitalRead(swr) == HIGH)
  {
    for (i = 0; i < 6; i++)
      (i < 4) ? s[i] = analogRead(i) : s[i] = analogRead(i + 2);
    for (i = 0; i < 3; i++)
    {
      value = String(s[i]);
      (s[i] < 100) ? text(value + "  ", 70, 5 - i * 2) : text(value + " ", 70, 5 - i * 2);
    }
    for (i = 3; i < 6; i++)
    {
      value = String(s[i], 10);
      (s[i] < 100) ? text(value + "  ", 10, (i - 3) * 2 + 1) : text(value + " ", 10, (i - 3) * 2 + 1);
    }
  }
  while (digitalRead(swl) == LOW || digitalRead(swr) == LOW)
    ;
}

void sensordisp()
{
  byte i;
  oled.set2X();
  oled.clear();
  while (digitalRead(swl) == HIGH && digitalRead(swr) == HIGH)
  {
    reading();
    sonar_reading();

    for (short int i = 5; i >= 0; i--)
    {
      Serial.print(String(s[i], 10) + " ");
      value = String(s[i], 2);
      text(value, 115 - (i * 23), 2);
    }
    Serial.print(" << ");
    Serial.print(sensor, BIN);
    Serial.print(" >> ");
    value = String(left_dis) + "  " + String(front_dis) + "  " + String(right_dis) + "  ";
    text(value, 4, 5);
    Serial.println(value);
  }
  while (digitalRead(swl) == LOW || digitalRead(swr) == LOW)
    ;
}

void callibration()
{
  byte i;
  oled.clear();
  oled.set2X();
  text(" STARTING ", 4, 3);
  m1 = m2 = millis();
  oled.set1X();
  text("    Press Any Key    ", 0, 6);
  text("to Cancel Calibration", 0, 7);
  while (digitalRead(swl) == HIGH && digitalRead(swr) == HIGH)
  {
    m2 = millis();
    if (m2 - m1 > 1000)
    {
      cal();
      break;
    }
  }
  while (digitalRead(swl) == LOW || digitalRead(swr) == LOW)
    digitalWrite(light, 1);
  digitalWrite(light, 0);
  // for showing the thresould value
  oled.clear();
  oled.set1X();
  for (byte i = 0; i < 6; i++)
  {
    value = String(minimum[i], 10);
    if (i >= 0 && i <= 2)
      text(value, 32 + (i * 32), 0);
    if (i >= 3 && i <= 5)
      text(value, (i * 32) - 64, 1);
  }
  for (byte i = 0; i < 6; i++)
  {
    value = String(threshold[i], 10);
    if (i >= 0 && i <= 2)
      text(value, 32 + (i * 32), 2);
    if (i >= 3 && i <= 5)
      text(value, (i * 32) - 64, 3);
  }
  for (byte i = 0; i < 6; i++)
  {
    value = String(maximum[i], 10);
    if (i >= 0 && i <= 2)
      text(value, 32 + (i * 32), 4);
    if (i >= 3 && i <= 5)
      text(value, (i * 32) - 64, 5);
  }
  text("MIN", 8, 0);
  text("MID", 8, 2);
  text("MAX", 8, 4);

  while (digitalRead(swl) == HIGH && digitalRead(swr) == HIGH)
    ;
  home_screen();
  while (digitalRead(swl) == LOW || digitalRead(swr) == LOW)
    ;
}