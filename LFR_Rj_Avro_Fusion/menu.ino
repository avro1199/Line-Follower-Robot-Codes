byte menu()
{
  byte cl = 0, b = 1, p = 8, sw, temp = 1, limit = 8;
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
        value = String(i, 10) + ". " + showmenu(i);
        text(value, 0, i - b);
      }
    }
    sw = rswitch();
    if (sw == 1)
    {
      temp++;
      if (temp > limit)
      {
        temp = 1;
        b = temp;
        p = b + 7;
      }
      else if (temp > p)
      {
        p = temp;
        b = p - 7;
      }
    }
    else if (sw == 2)
    {
      oled.setInvertMode(0);
      return temp;
    }
    sw = lswitch();
    if (sw == 1)
    {
      temp--;
      if (temp < 1)
      {
        temp = limit;
        p = temp;
        b = p - 7;
      }
      else if (temp < b)
      {
        b = temp;
        p = b + 7;
      }
    }
    else if (sw == 2)
    {
      oled.setInvertMode(0);
      home_screen();
      return 0;
    }
  }
}

String showmenu(byte a)
{
  if (a == 1)
    return "LINE FOLLOWER 1 ";
  else if (a == 2)
    return "LINE FOLLOWER 2 ";
  else if (a == 3)
    return "CALLIBRATION    ";
  else if (a == 4)
    return "SENSOR READING  ";
  else if (a == 5)
    return "WALL CHECK      ";
  else if (a == 6)
    return "MOTOR CHECK     ";
  else if (a == 7)
    return "MAZE SOLVE      ";
  else if (a == 8)
    return "MAZE SCAN       ";
  else
    return "NOTHING HERE!   ";
}
