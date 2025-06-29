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
  for (byte i = 0; i < 70; i++)
  {
    EEPROM.write(i + 100, 0);
    text("=", i * 5 / 2, 1);
    text("=", i * 5 / 2, 6);
    path[i] = 0;
    delay(20);
  }
  EEPROM.commit();
  oled.clear();
  oled.set2X();
  text("   DONE   ", 04, 03);
  delay(300);
  home_screen();
}

void path_select()
{
  byte cl = 0, b = 1, p = 8, sw, temp = 1, limit = 70;
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
    sw = btn_read(btn_d);
    if (sw == 1 && temp < limit)
      temp++;
    else if (sw == 2 && temp < limit - 8)
      temp += 8;
    if (temp > p)
    {
      p = temp;
      b = p - 7;
    }
    sw = btn_read(btn_u);
    if (sw == 1 && temp > 1)
      temp--;
    else if (sw == 2 && temp > 8)
      temp -= 8;
    if (temp < b)
    {
      b = temp;
      p = b + 7;
    }
    sw = btn_read(btn_m);
    if (sw == 1)
    {
      change_path(temp);
      cl = 0;
    }
    else if (sw == 2)
      break;
  }
  oled.setInvertMode(0);
  home_screen();
}

void change_path(int a)
{
  byte cl = 100, sw, temp = EEPROM.read(99 + a), limit = 3;
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
    sw = btn_read(btn_r);
    if (sw > 0 && temp <= limit)
    {
      temp++;
      if (temp > limit)
        temp = 0;
    }

    sw = btn_read(btn_l);
    if (sw > 0 && temp >= 0)
    {
      if(temp == 0) temp = limit+1;
      temp--;
    }

    sw = btn_read(btn_m);
    if (sw == 1)
    {
      EEPROM.write(99 + a, temp);
      delay(10);
      EEPROM.commit();
      path[a - 1] = temp;
      break;
    }
    else if (sw == 2)
      break;
  }
  oled.clear();
  oled.set1X();
}
