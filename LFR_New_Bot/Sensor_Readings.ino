void reading()
{
  sensor = 0;
  sum = 0;
  for (int i = 0; i <= 5; i++)
  {
    if (i <= 3)
    {
      s[i] = analogRead(i);
    }
    else
    {
      s[i] = analogRead(i + 2);
    }

    (s[i] < mid[i]) ? s[i] = 1 : s[i] = 0;
    sensor += s[i] * base[i];
    sum += s[i];
  }
}

void show_digital()
{
  lcd.clearDisplay();
  reading();
  for (int i = 5; i >= 0; i--)
  {
    lcd.setCursor(23*(5-i), 10);
    lcd.print(s[i]);
    Serial.print(s[i]);
    Serial.print(" ");
  }
  lcd.setCursor(28, 50);
  lcd.print(sensor, BIN);
  lcd.display();
  Serial.println(sensor, BIN);
  delay(100);
}

void show_analog()
{
  lcd.clearDisplay();
  for (int i = 5; i >= 0; i--)
  {
    if (i <= 3)
    {
      s[i] = analogRead(i);
    }
    else
    {
      s[i] = analogRead(i + 2);
    }
    lcd.setCursor(22 * (5 - i), 10);
    lcd.print(s[i]);
    Serial.print(s[i]);
    Serial.print(" ");
  }
  lcd.display();
  Serial.println();
  delay(100);
}
