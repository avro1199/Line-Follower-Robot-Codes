String show_path(byte a)
{
  if (a == 0)
    return " STRAIGHT ";
  else if (a == 1)
    return "LEFT      ";
  else if (a == 2)
    return "     RIGHT";
  else if (a == 3)
    return "  BRIDGE  ";
  else
    return "GARBAGE   ";
}

void path_reset()
{
  oled.clear();
  oled.set2X();
  text("PATH RESET", 04, 03);
  oled.set1X();
  for (byte i = 0; i <= 49; i++)
  {
    EEPROM.write(i + 40, 0);
    text("=", i * 5 / 2, 1);
    text("=", i * 5 / 2, 6);
    delay(20);
    path[i] = 0;
  }
  oled.clear();
  oled.set2X();
  text("   DONE   ", 04, 03);
  delay(300);
  home_screen();
}

void path_select()
{
  byte cl = 0, b = 1, p = 8, sw, temp = 1, limit = 50;
  oled.set1X();
  oled.clear();
  while (1)
  {
    if (cl != temp)
    {
      cl = temp;
      for (byte i = b; i <= p && i <= limit; i++)
      {
        (i == cl) ? oled.setInvertMode(1) : oled.setInvertMode(0);
        value = String(i, 10) + ". " + show_path(path[i - 1]) + "          ";
        (i < 10) ? value = '0' + value : value;
        text(value, 0, i - b);
      }
    }
    sw = rswitch();
    if (sw == 1 && temp < limit)
    {
      temp++;
      if (temp > p)
      {
        p = temp;
        b = p - 7;
      }
    }
    else if (sw == 2)
    {
      change_path(temp);
      cl = 0;
    }
    sw = lswitch();
    if (sw == 1 && temp > 1)
    {
      temp--;
      if (temp < b)
      {
        b = temp;
        p = b + 7;
      }
    }
    else if (sw == 2)
      break;
  }
  oled.setInvertMode(0);
  home_screen();
}

void change_path(int a)
{
  byte cl = 100, sw, temp = EEPROM.read(39 + a), limit = 3;
  oled.set2X();
  oled.clear();
  oled.setInvertMode(0);
  while (1)
  {
    if (cl != temp)
    {
      cl = temp;
      value = show_path(temp);
      text(value, 04, 3);
    }
    sw = rswitch();
    if (sw == 1 && temp < limit)
      temp++;
    else if (sw == 2)
    {
      EEPROM.write(39 + a, temp);
      delay(10);
      path[a - 1] = temp;
      break;
    }
    sw = lswitch();
    if (sw == 1 && temp > 0)
      temp--;
    else if (sw == 2)
      break;
  }
  oled.clear();
  oled.set1X();
}
