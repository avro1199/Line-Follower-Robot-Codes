void line_follow()
{
    oled.clear();
    oled.set2X();
    inv_mode = EEPROM.read(15);
    sp = EEPROM.read(0);
    spl = sp - le;
    spr = sp - re;

    text("LineFollow", 04, 3);
    base_speed = sp * 10; // for pid
    delay(100);

    while (digitalRead(btn_m) && digitalRead(btn_l) && digitalRead(btn_r))
    {
    point:
        sensor_read();
        // check_inverse();

        if (sum == 0)
        {
            motor(0, 0);
            brake();
            while (!btn_read(btn_m))
                ;
            /*
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

            // else if (pos < 5 && pos > -5)
            {
                m1 = millis();
                while (sum == 0)
                {
                    sensor_read();
                    if (millis() - m1 > u_turn_timer)
                    {
                        digitalWrite(led, 1);
                        brake();
                        hard_brake();
                        (side == 1) ? motor(-tsp, tsp) : motor(tsp, -tsp);
                        while (!s[4] && !s[5])
                            sensor_read();
                        brake();
                        (side == 1) ? motor(tsp, -tsp) : motor(-tsp, tsp);
                        delay(tbrk);
                        pos = 0;
                        digitalWrite(led, 0);
                        brake();
                        while (!btn_read(btn_m))
                            ;
                        break;
                    }
                }
            }
            */
        }
        else if (sum == 1 || sum == 2 || sum == 3)
        {
            single_line_follow();
        }
        /*
        else if (sum > 2 && sum < 8)
        {
            if (s[0] && !s[7] && (s[2] || s[3] || s[4] || s[5]))
            {
                digitalWrite(led, 1);
                flag = 1;
                if (normal_cross == 1 || normal_cross == 3)
                    cross = 1; // set it according to the arena
                else
                    cross = 0;
            }
            else if (!s[0] && s[7] && (s[2] || s[3] || s[4] || s[5]))
            {
                digitalWrite(led, 1);
                flag = 2;
                if (normal_cross == 2 || normal_cross == 3)
                    cross = 2; // set it according to the arena
                else
                    cross = 0;
            }
            m3 = millis();
        }
        */

        else if (sum == 6)
        {
            m1 = millis();
            digitalWrite(led, 1);
            flag = rule_turn;
            cross = rule_cross;
            normal_cross = 3;

            while (sum == 6)
            {
                sensor_read();
                m2 = millis();
                if (m2 - m1 > finish_timer)
                {
                f:
                    brake();
                    hard_brake();
                    digitalWrite(led, 1);
                    oled.clear();
                    oled.set2X();
                    text("!!FINISH!!", 4, 1);
                    while (digitalRead(btn_m) && digitalRead(btn_l) && digitalRead(btn_r))
                        sensor_read();
                    oled.clear();
                    oled.set2X();
                    text("LineFollow", 04, 3);
                    digitalWrite(led, 0);
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
            digitalWrite(led, 0);
        }
    }
    motor(0, 0);
    home_screen();
    digitalWrite(led, 0);
    cross = 0;
    flag = 0;
    while (digitalRead(btn_m) && digitalRead(btn_l) && digitalRead(btn_r))
        ;
}
