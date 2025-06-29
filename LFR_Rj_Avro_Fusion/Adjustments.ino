String show_adjust(byte a)
{
  if (a == 1)
    return "TOP SPEED     ";
  else if (a == 2)
    return "LEFT ERROR    ";
  else if (a == 3)
    return "RIGHT ERROR   ";
  else if (a == 4)
    return "STRAIGHTNER   ";
  else if (a == 5)
    return "FULL BRAKE    ";
  else if (a == 6)
    return "TURN BRAKE    ";
  else if (a == 7)
    return "TURN SPEED    ";
  else if (a == 8)
    return "NODE DELAY    ";
  else if (a == 9)
    return "U TURN TIMER  ";
  else if (a == 10)
    return "FINISH TIMER  ";
  else if (a == 11)
    return "U TURN SIDE   ";
  else if (a == 12)
    return "OBST. DELAY   ";
  else if (a == 13)
    return "OBST. DIST.   ";
  else if (a == 14)
    return "  PID         ";
  else if (a == 15)
    return "RIGHT WALL    ";
  else if (a == 16)
    return "RULE TURN     ";
  else if (a == 17)
    return "RULE CROSS    ";
  else if (a == 18)
    return "INVERSE MODE  ";
  else if (a == 19)
    return "INV. TIMER    ";
  else if (a == 20)
    return "NORM CROSS    ";
  else if (a == 21)
    return "OBJECT SIDE   ";
  else
    return "Garbage       ";
}

void adjustment_menu()
{
  byte cl = 0, b = 1, p = 8, sw, temp = 1, limit = 21;
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
        value = String(i) + ". " + show_adjust(i) + String(EEPROM.read(i + 17)) + "  ";
        if (i < 10)
          value = "0" + value;
        text(value, 2, i - b);
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
      adjustment(temp);
      cl = 0;
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
      break;
    }
  }
  sp = EEPROM.read(18);
  le = EEPROM.read(19);
  re = EEPROM.read(20);
  dt = EEPROM.read(21);
  br = EEPROM.read(22);
  tbr = EEPROM.read(23);
  tsp = EEPROM.read(24);
  node = EEPROM.read(25);
  u_turn_timer = EEPROM.read(26);
  finish_timer = EEPROM.read(27);
  spl = sp - le;
  spr = sp - re;
  side = EEPROM.read(28);
  obs_delay = EEPROM.read(29);
  obs_dis = EEPROM.read(30);
  l_wall_dis = EEPROM.read(31);
  r_wall_dis = EEPROM.read(32);
  rule_turn = EEPROM.read(33);
  rule_cross = EEPROM.read(34);
  inv_mode = EEPROM.read(35);
  inv_timer = EEPROM.read(36);
  normal_cross = EEPROM.read(37);
  obs_side = EEPROM.read(38);

  counter = EEPROM.read(39);
  home_screen();
}

void adjustment(byte a)
{
  byte cl = 248, sw, temp = EEPROM.read(17 + a), limit = 255;
  oled.set2X();
  oled.clear();
  if (a == 1)
    limit = 25;
  else if (a == 2 || a == 3)
    limit = 10;
  else if (a == 4)
    limit = 30;
  else if (a == 11 || a == 16 || a == 17)
    limit = 2;
  else if (a == 13 || a == 15)
    limit = 30;
  else if (a == 14)
    limit = 9;
  else if (a == 18)
    limit = 1;
  else if (a == 20)
    limit = 3;
  else if (a == 21)
    limit = 1;
  else if (a == 22)
    limit = 50;
  while (1)
  {
    if (cl != temp)
    {
      cl = temp;
      value = "SET: " + String(temp) + "    ";
      text(value, 04, 3);
    }
    sw = rswitch();
    if (sw == 1 && temp <= limit)
    {
      ((a > 4 && a < 11) || a == 12) ? temp += 10 : temp++;
      if (temp > limit)
        temp = 0;
    }
    else if (sw == 2)
    {
      EEPROM.write(17 + a, temp);
      delay(10);
      break;
    }
    sw = lswitch();
    if (sw == 1 /*&& temp > 0*/)
    {
      ((a > 4 && a < 11) || a == 12) ? temp -= 5 : temp--;
      if (temp > limit)
        temp = limit;
    }
    else if (sw == 2)
      break;
  }
  oled.clear();
  oled.set1X();
}
