void line_follow()
{
  while (digitalRead(sw))
  { // infinite loop
  a:
    reading();
    if (sum == 0)
    {
      if (flag != 's')
      {
        delay(node_delay);
        braking();
        (flag == 'l') ? motor(-tsp, tsp - 30) : motor(tsp, -tsp);
        while (s[2] == 0 && s[3] == 0)
          reading();
        (flag == 'l') ? motor(tsp, -tsp) : motor(-tsp, tsp);
        delay(tbr);
        pos = 0;
        flag = 's';
        cross = 0; // when you are done turning robot, make the flag to its normal state so that robot wont turn on its own when it finds no line without detecting 90degree logic
      }

      else if (pos < 4 && pos > -4)
      {
        m3 = millis();
        while (sum == 0)
        {
          reading();
          m2 = millis();
          if (m2 - m3 > u_turn_timer)
          {
            braking();
            digitalWrite(head_light, HIGH);
            (side == 1) ? motor(-tsp, tsp) : motor(tsp, -tsp);
            while (s[2] == 0 && s[3] == 0)
              reading();
            (side == 1) ? motor(tsp, -tsp) : motor(-tsp, tsp);
            delay(tbr);
            pos = 0;
            digitalWrite(head_light, LOW);
            break;
          }
        }
      }
    }
    else if (sum == 1 || sum == 2)
    { // only for straight line
      if (cross != 0)
      {
        braking();
        (cross == 1) ? motor(-tsp, tsp) : motor(tsp, -tsp);
        while (sum > 0)
          reading();
        while (s[2] == 0 && s[3] == 0)
          reading();
        (cross == 1) ? motor(tsp, -tsp) : motor(-tsp, tsp);
        delay(tbr);
        pos = 0;
        flag = 's';
        cross = 0;
      }

      else if (sensor == 0b001100)
      {
        if (pos != 0)
        {
          (pos > 0) ? motor(-10 * spl, 10 * spr) : motor(10 * spl, -10 * spr);
          (pos > 0) ? delay(pos * dr) : delay(-pos * dr);
          pos = 0; // this logic is for those whose bot is wabbling during high speed run. this logic will forcefully return bot to its midpoint. don't give more than 30ms delay here!
        }
        motor(10 * spl, 10 * spr);
        if (cl < peak)
          cl++;
      }
      // right side portion
      else if (sensor == 0b000100)
      {
        motor(10 * spl, 8 * spr);
        if (cl < peak)
          cl++;
      }
      else if (sensor == 0b000110)
      {
        if (pos < 1)
          pos = 1;
        motor(10 * spl, 5 * spr);
      }
      else if (sensor == 0b000010)
      {
        if (pos < 2)
          pos = 2;
        motor(10 * spl, -1 * spr);
      }
      else if (sensor == 0b000011)
      {
        if (pos < 3)
          pos = 3;
        motor(10 * spl, -6 * spr);
        flag = 'r';
      }
      else if (sensor == 0b000001)
      {
        if (pos < 4)
          pos = 4;
        motor(10 * spl, -10 * spr);
        if (cl > strt)
          cl--;
        flag = 'r';
      }
      // left side portion
      else if (sensor == 0b001000)
      {
        motor(8 * spl, 10 * spr);
        if (cl < peak)
          cl++;
      }
      else if (sensor == 0b011000)
      {
        if (pos > -1)
          pos = -1;
        motor(5 * spl, 10 * spr);
      }
      else if (sensor == 0b010000)
      {
        if (pos > -2)
          pos = -2;
        motor(-1 * spl, 10 * spr);
      }
      else if (sensor == 0b110000)
      {
        if (pos > -3)
          pos = -3;
        motor(-6 * spl, 10 * spr);
        flag = 'l';
      }
      else if (sensor == 0b100000)
      {
        if (pos > -4)
          pos = -4;
        motor(-10 * spl, 10 * spr);
        if (cl > strt)
          cl--;
        flag = 'l';
      }
    }

    else if (sum == 3 || sum == 4 || sum == 5)
    { // only for sharp turn logic
      motor(10 * spl, 10 * spr);
      if (s[0] == 0 && s[5] == 1 && s[2] + s[3] + s[4] > 0)
      {

        flag = 'l';
      }
      else if (s[0] == 1 && s[5] == 0 && s[1] + s[2] + s[3] > 0)
      {
        flag = 'r';
      }
      m1 = millis();
    }

    else if (sum == 6)
    {
      m3 = millis();
      flag = 's';
      cross = 2;
      pos = 0; // turn no 1
      while (s[0] == 1 || s[5] == 1)
      {
        reading();
        m2 = millis();
        if (m2 - m3 > stop_timer)
        {
          motor(-250, -250);
          delay(cl / brake_constant);
          motor(0, 0);
          digitalWrite(head_light, HIGH);
          while (sum == 6 && digitalRead(sw))
            reading();
          digitalWrite(head_light, LOW);
          if (digitalRead(sw))
          {
            delay(1000);
            goto a;
          }
          break;
        }
      }
      m1 = millis();
    }
    m2 = millis();
    if (m2 - m1 > 200)
      flag = 's';
  }
}
