void fixed_path()
{
  oled.clear();
  oled.set2X();
  inv_mode = EEPROM.read(35);
  counter = EEPROM.read(39);
  text("PathFollow", 04, 3);
  Serial.print("Counter : ");
  Serial.print(counter);
  delay(100);

  while (digitalRead(swl) == HIGH && digitalRead(swr) == HIGH)
  {
  point:
    reading();

    if (sum == 0)
    {
      if (flag != 0)
      {
        nutral();
        hard_brake();
        (flag == 1) ? motor(-tsp, tsp) : motor(tsp, -tsp);
        while (!s[2] && !s[3])
          reading();
        nutral();
        (flag == 1) ? motor(tsp, -tsp) : motor(-tsp, tsp);
        delay(tbr);
        pos = 0;
        flag = 0;
        cross = 0;
      }
      else if (pos < 4 && pos > -4)
      {
        m1 = millis();
        while (sum == 0)
        {
          reading();
          if (millis() - m1 > u_turn_timer)
          {
            digitalWrite(light, 1);
            nutral();
            hard_brake();
            (side == 1) ? motor(-tsp, tsp) : motor(tsp, -tsp);
            while (!s[2] && !s[3])
              reading();
            nutral();
            (side == 1) ? motor(tsp, -tsp) : motor(-tsp, tsp);
            delay(tbr);
            pos = 0;
            digitalWrite(light, 0);
            break;
          }
        }
      }
    }
    else if (sum == 1 || sum == 2)
    {
      if (cross != 0)
      {
        digitalWrite(light, 1);
        nutral();
        hard_brake();
        (cross == 1) ? motor(-tsp, tsp) : motor(tsp, -tsp);
        while (s[1] || s[2] || s[3] || s[4])
          reading();
        while (!s[2] && !s[3])
          reading();
        nutral();
        (cross == 1) ? motor(tsp, -tsp) : motor(-tsp, tsp);
        delay(tbr);
        digitalWrite(light, 0);
        pos = 0;
        flag = 0;
        cross = 0;
      }
      else if (sensor == 0b001100)
      {
        if (pos != 0)
        {
          (pos > 0) ? motor(-10 * spl, 10 * spr) : motor(10 * spl, -10 * spr);
          (pos > 0) ? delay(pos * dt) : delay(-pos * dt);
          pos = 0;
        }
        motor(10 * spl, 10 * spr);
      }
      else if (sensor == 0b000100)
        motor(10 * spl, 8 * spr);
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
        motor(10 * spl, 2 * spr);
      }
      else if (sensor == 0b000011)
      {
        if (pos < 3)
          pos = 3;
        motor(10 * spl, -2 * spr);
        // flag = 2; // only if bot can't hold on the line
      }
      else if (sensor == 0b000001)
      {
        if (pos < 4)
          pos = 4;
        motor(10 * spl, -4 * spr);
        flag = 2; // only if bot can't hold on the line
      }
      // left side portion
      else if (sensor == 0b001000)
        motor(8 * spl, 10 * spr);
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
        motor(2 * spl, 10 * spr);
      }
      else if (sensor == 0b110000)
      {
        if (pos > -3)
          pos = -3;
        motor(-2 * spl, 10 * spr);
        // flag = 1; // only if bot can't hold on the line
      }
      else if (sensor == 0b100000)
      {
        if (pos > -4)
          pos = -4;
        motor(-4 * spl, 10 * spr);
        flag = 1; // only if bot can't hold on the line
      }
    }

    else if (sum == 3 || sum == 4 || sum == 5)
    { // only for sharp turn logic
      if (s[5] && !s[0] && (s[3] || s[2]))
      {
        flag = 1;
        while (s[5] && !s[0])
          reading();
        if (!s[0])
        {
          delay(node);
          reading();
          if (sum)
          {
            if (path[counter] == 1)
              cross = 1;
            counter++;
            digitalWrite(light, 1);
          }
        }
      }
      else if (!s[5] && s[0] && (s[3] || s[2]))
      {
        flag = 2;
        while (!s[5] && s[0])
          reading();
        if (!s[5])
        {
          delay(node);
          reading();
          if (sum)
          {
            if (path[counter] == 2)
              cross = 2;
            counter++;
            digitalWrite(light, 1);
          }
        }
      }
      m3 = millis();
    }

    else if (sum == 6)
    {
      m1 = millis();
      while (sum == 6)
      {
        reading();
        m2 = millis();
        if (m2 - m1 > finish_timer)
        {
          nutral();
          hard_brake();
          digitalWrite(light, 1);
          oled.clear();
          oled.set2X();
          text("!!FINISH!!", 4, 1);
          while (/*sum == 6 && */ digitalRead(swl) == HIGH && digitalRead(swr) == HIGH)
            reading();
          oled.clear();
          oled.set2X();
          text("PathFollow", 04, 3);
          digitalWrite(light, 0);
          if (digitalRead(swl) == HIGH && digitalRead(swr) == HIGH)
          {
            delay(1000);
            goto point;
          }
          break;
        }
      }
      (path[counter] == 1) ? flag = 1 : ((path[counter] == 2) ? flag = 2 : flag = rule_turn);
      delay(node);
      reading();
      if (sum)
        cross = path[counter];
      counter++;
      digitalWrite(light, 1);
      m3 = millis();
    }
    m2 = millis();
    if (m2 - m3 > 200)
    {
      flag = 0;
      digitalWrite(light, 0);
    }
  }
  home_screen();
  motor(0, 0);
  digitalWrite(light, 0);
  cross = 0;
  flag = 0;
  while (digitalRead(swl) == LOW || digitalRead(swr) == LOW)
    ;
}
