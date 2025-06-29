String show_adjust(byte a)
{
  if (a == 0)
    return "TOP SPEED     ";
  else if (a == 1)
    return "RULE TURN     ";
  else if (a == 2)
    return "RULE CROSS    ";
  else if (a == 3)
    return "NORM CROSS    ";
  else if (a == 4)
    return "U TURN SIDE   ";
  else if (a == 5)
    return "U TURN TIMER  ";
  else if (a == 6)
    return "FINISH TIMER  ";
  else if (a == 7)
    return "NODE DELAY    ";
  else if (a == 8)
    return "TURN SPEED    ";
  else if (a == 9)
    return "TURN BRAKE    ";
  else if (a == 10)
    return "FULL BRAKE    ";
  else if (a == 11)
    return "LEFT ERROR    ";
  else if (a == 12)
    return "RIGHT ERROR   ";
  else if (a == 13)
    return "STRAIGHTNER   ";
  else if (a == 14)
    return "WALL DISTANCE ";
  else if (a == 15)
    return "INVERSE MODE  ";
  else if (a == 16)
    return "OBSTACLE SIDE ";
  else if (a == 17)
    return "OBSTACLE DIST ";
  else if (a == 18)
    return "OBSTACLE DELY ";
  else if (a == 19)
    return "INV. TIMER    ";
  else if (a == 20)
    return " << K_p >>    ";
  else if (a == 21)
    return " << K_d >>    ";
  else if (a == 22)
    return " << K_i >>    ";
  else if (a == 23)
    return " >Enable PID< ";
  else
    return "Garbage       ";
}

void adjustment_menu()
{
  byte cl = 0, b = 1, p = 8, sw, temp = 1, limit = 24;
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
        value = String(i) + ". " + show_adjust(i - 1) + String(EEPROM.read(i - 1)) + "  ";
        if (i < 10)
          value = "0" + value;
        text(value, 2, i - b);
      }
    }
    sw = btn_read(btn_r);
    if (sw == 1)
    {
      temp++;
    }

    if (sw == 2)
    {
      temp += 8;
    }
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
    sw = btn_read(btn_l);
    if (sw == 1)
    {
      temp--;
    }

    if (sw == 2)
    {
      if (temp > 8)
      {
        temp -= 8;
      }
      else
      {
        temp = limit - 8 + temp;
      }
    }
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
    sw = btn_read(btn_m);
    if (sw == 1)
    {
      oled.setInvertMode(0);
      adjustment(temp);
      cl = 0;
    }
    else if (sw == 2)
    {
      oled.setInvertMode(0);
      break;
    }
  }

  sp = EEPROM.read(0);
  rule_turn = EEPROM.read(1);
  rule_cross = EEPROM.read(2);
  normal_cross = EEPROM.read(3);
  u_turn_side = EEPROM.read(4);
  u_turn_timer = EEPROM.read(5);
  finish_timer = EEPROM.read(6);
  node_delay = EEPROM.read(7);
  tsp = EEPROM.read(8);
  tbrk = EEPROM.read(9);
  brk = EEPROM.read(10);
  le = EEPROM.read(11);
  re = EEPROM.read(12);
  st = EEPROM.read(13);
  wall_dis = EEPROM.read(14);
  inv_mode = EEPROM.read(15);
  obs_side = EEPROM.read(16);
  obs_dis = EEPROM.read(17);
  obs_delay = EEPROM.read(18);
  inv_timer = EEPROM.read(19);
  k_p = EEPROM.read(20);
  k_d = EEPROM.read(21);
  k_i = EEPROM.read(22);
  en_pid = EEPROM.read(23);
  home_screen();
}

void adjustment(byte a)
{
  byte cl = 248, sw, temp = EEPROM.read(a - 1), limit = 255;
  oled.set2X();
  oled.clear();
  if (a == 1)
    limit = 25;
  else if (a == 2)
    limit = 2;
  else if (a == 3 || a == 4)
    limit = 3;
  else if (a == 5 || a == 16 || a == 17 || a == 24)
    limit = 1;

  while (1)
  {
    if (cl != temp)
    {
      cl = temp;
      value = "SET: " + String(temp) + "    ";
      text(value, 04, 3);
    }
    sw = btn_read(btn_r);
    if (sw > 0 && temp <= limit)
    {
      (a == 6 || a == 7 || a == 9 || a == 10 || a == 11) ? temp += 10 : temp++;
      // temp++;
      if (temp > limit)
        temp = 0;
    }

    sw = btn_read(btn_l);
    if (sw > 0 /*&& temp > 0*/)
    {
      (a == 6 || a == 7 || a == 9 || a == 10 || a == 11) ? temp -= 5 : temp--;
      // temp--;
      if (temp > limit)
        temp = limit;
    }

    sw = btn_read(btn_m);
    if (sw == 1)
    {
      EEPROM.write(a - 1, temp);
      delay(10);
      break;
    }
    else if (sw == 2)
      break;
  }
  oled.clear();
  oled.set1X();
}
