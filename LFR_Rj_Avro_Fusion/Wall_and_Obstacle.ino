void wall_follow()
{
    oled.clear();
    text("WallFollow", 04, 3);
    short_brake();
    delay(10);
    // hard_brake();
    digitalWrite(light, 1);
    inv_mode = 1;
    normal_cross = 0;
    reading();

    bool crx = 0;

    Serial.println(sum);

    while (sum == (6 * inv_mode))
    {
        reading();
        sonar_reading();

        // if (!(left_dis + right_dis))
        //     return;

        if (front_dis < obs_dis && front_dis != 0)
        {
            // (obs_side) ? motor(120, -100) : motor(-100, 120);
            motor(-100, 120);
            while (front_dis != 0)
            {
                front_dis = sonar_f.ping_cm();
                delay(2);
            }
            while (!(left_dis + right_dis))
            {
                sonar_reading();
                delay(2);
            }
            delay(50);
        }

        if (left_dis && right_dis)
        {
            if ((left_dis - right_dis) > 2)
                motor(40, 120);
            else if ((right_dis - left_dis) > 2)
                motor(120, 40);
            else
                motor(100, 100);
        }
        else if (left_dis && !right_dis)
        {
            if (crx)
            {
                motor(120, 40);
                while (!(left_dis && right_dis))
                {
                    sonar_reading();
                    delay(2);
                }
            }
            if (left_dis < r_wall_dis)
                motor(120, 60);
            else if (left_dis > r_wall_dis)
                motor(60, 120);
            else
                motor(100, 100);
        }
        else if (!left_dis && right_dis)
        {
            if (crx)
            {
                motor(40, 120);
                while (!(left_dis && right_dis))
                {
                    sonar_reading();
                    delay(2);
                }
            }
            if (right_dis < r_wall_dis)
                motor(60, 120);
            else if (right_dis > r_wall_dis)
                motor(120, 60);
            else
                motor(100, 100);
        }
        else
        {
            motor(100, 100);
        }
        delay(2);
    }
    digitalWrite(light, 0);
    oled.clear();
    text("Normal Mode", 0, 3);
}

// void avoid_obstacle()
// {
//     oled.clear();
//     text(" Obstacle ", 04, 3);
//     nutral();
//     hard_brake();
//     spr = 13;
//     spl = 13;
//     digitalWrite(light, 1);

//     (obs_side) ? motor(8 * spl, -8 * spr) : motor(-8 * spl, 8 * spr);
//     delay(140);
//     nutral();
//     (obs_side) ? motor(-8 * spl, 8 * spr) : motor(8 * spl, -8 * spr);
//     delay(25);

//     motor(7 * spl, 7 * spr);
//     delay(obs_delay * 3);
//     nutral();
//     hard_brake();

//     (obs_side) ? motor(-8 * spl, 8 * spr) : motor(8 * spl, -8 * spr);
//     delay(180);
//     nutral();
//     (obs_side) ? motor(8 * spl, -8 * spr) : motor(-8 * spl, 8 * spr);
//     delay(25);

//     motor(7 * spl, 7 * spr);
//     delay(obs_delay * 2.7);
//     nutral();
//     hard_brake();

//     (obs_side) ? motor(-8 * spl, 8 * spr) : motor(8 * spl, -8 * spr);
//     delay(150);

//     // jotokkhon na majhkhaner duita black pabe totokkhon soja jaite thakbe.
//     reading();
//     while (!s[1] && !s[2])
//     {
//         reading();
//         motor(7 * spl, 7 * spr);
//     }

//     (obs_side) ? motor(8 * spl, -8 * spr) : motor(-8 * spl, 8 * spr);
//     delay(80);
//     nutral();
//     (obs_side) ? motor(-8 * spl, 8 * spr) : motor(8 * spl, -8 * spr);
//     delay(20);

//     (obs_side) ? pos = 4 : pos = -4;

//     digitalWrite(light, 0);

//     oled.clear();
//     text("Normal Mode", 0, 3);
//     sp = EEPROM.read(18);
//     le = EEPROM.read(19);
//     re = EEPROM.read(20);
//     spl = sp - le;
//     spr = sp - re;
// }