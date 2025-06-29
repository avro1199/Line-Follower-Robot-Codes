void line_follow()
{
    oled.clear();
    oled.set2X();
    inv_mode = EEPROM.read(15);
    sp = EEPROM.read(0);
    spl = sp - le;
    spr = sp - re;

    Serial.print(sp);
    Serial.print(" <=> ");
    Serial.print(spl);
    Serial.print(" <=> ");
    Serial.print(spr);
    Serial.println();

    text("LineFollow", 04, 3);
    delay(100);

    while (digitalRead(btn_m) && digitalRead(btn_l) && digitalRead(btn_r))
    {
    point:
        sensor_read();
        check_inverse();

        if (sum == 0)
        {
            sonar_read();
            if (l_dist && r_dist)
            {
                wall_follow();
                goto point;
            }

            if (flag != 0)
            {
                fan(fan_speed * en_pid);
                delay(node_delay);
                brake();
                hard_brake();
                (flag == 1) ? motor(-tsp, tsp) : motor(tsp, -tsp);
                while (!s[3] && !s[4])
                    sensor_read();
                brake();
                (flag == 1) ? motor(tsp, -tsp) : motor(-tsp, tsp);
                delay(tbrk);
                fan(0);
                pos = 0;
                flag = 0;
                cross = 0;
            }
            else if (pos < 5 && pos > -5)
            {
                m1 = millis();
                while (sum == 0)
                {
                    sensor_read();
                    if (millis() - m1 > u_turn_timer)
                    {
                        digitalWrite(light, 1);
                        brake();
                        hard_brake();
                        (side == 1) ? motor(-tsp, tsp) : motor(tsp, -tsp);
                        while (!s[3] && !s[4])
                            sensor_read();
                        brake();
                        (side == 1) ? motor(tsp, -tsp) : motor(-tsp, tsp);
                        delay(tbrk);
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
                fan(fan_speed * en_pid);
                digitalWrite(light, 1);
                delay(node_delay);
                brake();
                hard_brake();
                (cross == 1) ? motor(-tsp, tsp) : motor(tsp, -tsp);
                while (s[1] || s[2] || s[3] || s[4] || s[5] || s[6])
                    sensor_read();
                while (!s[3] && !s[4])
                    sensor_read();
                brake();
                (cross == 1) ? motor(tsp, -tsp) : motor(-tsp, tsp);
                delay(tbrk);
                fan(0);
                digitalWrite(light, 0);
                pos = 0;
                flag = 0;
                cross = 0;
            }
            else if (sensor == 0b00011000)
            {
                if (pos != 0)
                {
                    (pos > 0) ? motor(-10 * spl, 10 * spr) : motor(10 * spl, -10 * spr);
                    (pos > 0) ? delay(pos * st) : delay(-pos * st);
                    pos = 0;
                }
                motor(10 * spl, 10 * spr);
            }
            else if (sensor == 0b00001000)
                motor(10 * spl, 8 * spr);
            else if (sensor == 0b00001100)
            {
                motor(10 * spl, 7 * spr);
            }
            else if (sensor == 0b00000100)
            {
                if (pos < 1)
                    pos = 1;
                motor(10 * spl, 5 * spr);
            }
            else if (sensor == 0b00000110)
            {
                if (pos < 2)
                    pos = 2;
                motor(10 * spl, 1 * spr);
            }
            else if (sensor == 0b00000010)
            {
                if (pos < 3)
                    pos = 3;
                motor(10 * spl, -2 * spr);
            }
            else if (sensor == 0b00000011)
            {
                if (pos < 4)
                    pos = 4;
                motor(10 * spl, -4 * spr);
                flag = 2; // only if bot can't hold on the line
            }
            else if (sensor == 0b00000001)
            {
                if (pos < 5)
                    pos = 5;
                motor(10 * spl, -7 * spr);
                flag = 2; // only if bot can't hold on the line
            }

            // left side portion
            else if (sensor == 0b00010000)
                motor(8 * spl, 10 * spr);
            else if (sensor == 0b00110000)
            {
                motor(7 * spl, 10 * spr);
            }
            else if (sensor == 0b00100000)
            {
                if (pos > -1)
                    pos = -1;
                motor(5 * spl, 10 * spr);
            }
            else if (sensor == 0b01100000)
            {
                if (pos > -2)
                    pos = -2;
                motor(1 * spl, 10 * spr);
            }
            else if (sensor == 0b01000000)
            {
                if (pos > -3)
                    pos = -3;
                motor(2 * spl, 10 * spr);
            }
            else if (sensor == 0b11000000)
            {
                if (pos > -4)
                    pos = -4;
                motor(-4 * spl, 10 * spr);
                flag = 1; // only if bot can't hold on the line
            }
            else if (sensor == 0b10000000)
            {
                if (pos > -5)
                    pos = -5;
                motor(-7 * spl, 10 * spr);
                flag = 1; // only if bot can't hold on the line
            }
        }

        else if (sum > 2 && sum < 8)
        {
            if (s[0] && !s[7] && (s[2] || s[3] || s[4] || s[5]))
            {
                digitalWrite(light, 1);
                flag = 1;
                if (normal_cross == 1 || normal_cross == 3)
                    cross = 1; // set it according to the arena
                else
                    cross = 0;
            }
            else if (!s[0] && s[7] && (s[2] || s[3] || s[4] || s[5]))
            {
                digitalWrite(light, 1);
                flag = 2;
                if (normal_cross == 2 || normal_cross == 3)
                    cross = 2; // set it according to the arena
                else
                    cross = 0;
            }
            m3 = millis();
        }

        else if (sum == 8)
        {
            m1 = millis();
            digitalWrite(light, 1);
            flag = rule_turn;
            cross = rule_cross;
            normal_cross = 3;
            
            while (sum == 8)
            {
                sensor_read();
                m2 = millis();
                if (m2 - m1 > finish_timer)
                {
                f:
                    brake();
                    hard_brake();
                    digitalWrite(light, 1);
                    oled.clear();
                    oled.set2X();
                    text("!!FINISH!!", 4, 1);
                    while (digitalRead(btn_m) && digitalRead(btn_l) && digitalRead(btn_r))
                        sensor_read();
                    oled.clear();
                    oled.set2X();
                    text("LineFollow", 04, 3);
                    digitalWrite(light, 0);
                    if (digitalRead(btn_m) && digitalRead(btn_l) && digitalRead(btn_r))
                    {
                        delay(1000);
                        goto point;
                    }

                    break;
                }
            }
            delay(node_delay);
            m3 = millis();
        }
        m2 = millis();
        if (m2 - m3 > 200) // 200 old
        {
            flag = 0;
            cross = 0;
            digitalWrite(light, 0);
        }
    }
    motor(0, 0);
    home_screen();
    digitalWrite(light, 0);
    cross = 0;
    flag = 0;
    while (digitalRead(btn_m) && digitalRead(btn_l) && digitalRead(btn_r))
        ;
}
