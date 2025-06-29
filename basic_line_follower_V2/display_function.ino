void text(String t, int x, int y)
{
  display.setCursor(x, y);
  display.print(t);
}

void sust_cracker_nut()
{
  display.setTextSize(2);
  display.clearDisplay();
  text("SUST", 43, 2);
  text("CRACKER", 23, 24);
  text("NUT", 47, 46);
  for (int i = 0; i <= 15; i += 3)
  {
    display.drawLine(i + 21, 0, i, 32, WHITE);
    display.drawLine(i, 32, i + 25, 64, WHITE);
    display.drawLine(i + 94, 0, i + 112, 32, WHITE);
    display.drawLine(i + 112, 32, i + 94, 64, WHITE);
  }
  display.display();
}

void text_battery_check()
{
  display.clearDisplay();
  //  int p = analogRead(8);
  int p = map(analogRead(8), 740, 1023, 0, 100);
  if (p < 0)
    p = 0;
  String value = String(p, 10);
  text("BATTERY", 23, 16);
  if (p >= 100)
  {
    text(value, 43, 42);
    text("%", 80, 42);
  }
  else
  {
    text(value, 47, 42);
    text("%", 73, 42);
  }
  //  text(value, 43, 24);
  display.display();
}

void text_battery_low()
{
  int p = map(analogRead(8), 740, 1023, 0, 100);
  if (p < 0)
    p = 0;
  display.clearDisplay();
  String value = String(p, 10);
  text("BATTERY", 23, 2);
  text("LOW", 43, 24);
  text(value, 43, 46);
  text("%", 80, 42);
  display.display();
}

void analog_reading_display()
{
  int s[] = {0, 0, 0, 0, 0, 0};
  display.clearDisplay();
  for (int i = 0; i < 6; i++)
  {
    s[i] = analogRead(i + 10);
  }
  for (int i = 0; i < 3; i++)
  {
    String value = String(s[i], 10);
    text(value, 72, 46 - (i * 22));
  }
  for (int i = 3; i < 6; i++)
  {
    String value = String(s[i], 10);
    text(value, 12, (i * 22) - 64);
  }
  display.display();
}

void digital_reading_display()
{
  display.clearDisplay();
  String value;
  check();
  for (int i = 5; i >= 0; i--)
  {
    value = String(sensor[i], 2);
    text(value, 115 - (i * 23), 2);
  }
  value = String(sum, 10);
  text("sum = ", 0, 24);
  text(value, 70, 24);
  value = String(bin, 10);
  text("bin = ", 0, 46);
  text(value, 70, 46);
  display.display();
}

void text_line_follow()
{
  display.clearDisplay();
  text("LineFollow", 4, 16);
  text("Counter:", 10, 42);
  String value = String(counter, 10);
  text(value, 104, 42);
  display.display();
}

void text_wall_follow()
{
  display.clearDisplay();
  text("WallFollow", 4, 16);
  text("Counter:", 10, 42);
  String value = String(counter, 10);
  text(value, 104, 42);
  display.display();
}

void text_speed_adjust()
{
  display.clearDisplay();
  text("SPEED", 35, 2);
  text("ADJUST", 29, 24);
  text("L:   R:  ", 11, 46);
  int p = EEPROM.read(6);
  String value = String(p, 10);
  text(value, 33, 46);
  p = EEPROM.read(7);
  value = String(p, 10);
  text(value, 93, 46);
  display.display();
}

void sonar_reading_display()
{
  sl = sonarl.ping_cm();
  sf = sonarf.ping_cm();
  sr = sonarr.ping_cm();
  display.clearDisplay();
  String value;
  text("L", 12, 16);
  text("F", 59, 16);
  text("R", 106, 16);
  value = String(sl, 10);
  text(value, 5, 44);
  value = String(sf, 10);
  text(value, 52, 44);
  value = String(sr, 10);
  text(value, 99, 44);
  display.display();
}

void text_callibrating()
{
  int i, t = 0;
  display.clearDisplay();
  display.setTextSize(1);
  text("CALLIBRATING", 20, 24);
  display.display();
  for (i = 95; i <= 105; i += 5)
  {
    while (digitalRead(calin) == LOW)
    {
      t++;
      delay(1);
      if (t == 500)
      {
        t = 0;
        break;
      }
    }
    if (digitalRead(calin) == HIGH)
      break;
    text(".", i, 24);
    display.display();
  }
  if (i >= 110)
    cal();

  display.clearDisplay();
  // for showing the thresould value
  for (int i = 0; i < 6; i++)
  {
    String value = String(minimum[i], 10);
    if (i >= 0 && i <= 2)
    {
      text(value, 32 + (i * 32), 0);
    }
    if (i >= 3 && i <= 5)
    {
      text(value, (i * 32) - 64, 10);
    }
  }

  for (int i = 0; i < 6; i++)
  {
    String value = String(trash[i], 10);
    if (i >= 0 && i <= 2)
    {
      text(value, 32 + (i * 32), 23);
    }
    if (i >= 3 && i <= 5)
    {
      text(value, (i * 32) - 64, 33);
    }
  }

  for (int i = 0; i < 6; i++)
  {
    String value = String(maximum[i], 10);
    if (i >= 0 && i <= 2)
    {
      text(value, 32 + (i * 32), 46);
    }
    if (i >= 3 && i <= 5)
    {
      text(value, (i * 32) - 64, 56);
    }
  }
  text("MIN", 8, 5);
  text("MID", 8, 28);
  text("MAX", 8, 51);
  display.drawLine(0, 20, 128, 20, WHITE);
  display.drawLine(0, 43, 128, 43, WHITE);
  display.display();
  delay(500);
  while (digitalRead(calin) == LOW)
    ;
}
