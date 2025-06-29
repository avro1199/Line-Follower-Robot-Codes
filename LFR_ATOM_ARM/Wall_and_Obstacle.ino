void wall_follow()
{
    oled.clear();
    text("WallFollow", 04, 3);
    brake();
    delay(10);
    // hard_brake();
    digitalWrite(led, 1);

    bool crx = 1; //have to put it on display function

    motor(60, 60); //unnecessary but for tuning
    
    while (sum == 0)
    {
        sensor_read();
        sonar_read();

        // if (!(l_dist + r_dist))
        //     return;

        if (f_dist < obs_dis && f_dist != 0)
        {
            (obs_side) ? motor(100, 0) : motor(0, 100);
            while (f_dist != 0)
            {
                f_dist = sonar_f.ping_cm();
                delay(2);
            }
            while (!(l_dist && r_dist))
            {
                sonar_read();
                delay(2);
            }
            delay(obs_delay);
        }

        if (l_dist && r_dist)
        {
            if ((l_dist - r_dist) > 2)
                motor(60, 120);
            else if ((r_dist - l_dist) > 2)
                motor(120, 60);
            else
                motor(100, 100);
        }
        else if (l_dist && !r_dist)
        {
            if(crx)
            {
                motor(120, -10);
                while (!(l_dist && r_dist))
                {
                    sonar_read();
                    delay(2);
                }
                
            }
            if (l_dist < wall_dis)
                motor(120, 60);
            else if (l_dist > wall_dis)
                motor(60, 120);
            else
                motor(100, 100);
        }
        else if (!l_dist && r_dist)
        {
            if(crx)
            {
                motor(-10, 120);
                while (!(l_dist && r_dist))
                {
                    sonar_read();
                    delay(2);
                }
                
            }
            if (r_dist < wall_dis)
                motor(60, 120);
            else if (r_dist > wall_dis)
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
    digitalWrite(led, 0);
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