void check_inverse()
{
    if (s[0] && s[9] && sum < 10)
    {
        m2 = m1 = millis();
        motor(150, 150);
        while (s[0] && s[9] && (sum < 8) && (sum > 4))
        {
            sensor_read();
            m2 = millis();
            if (m2 - m1 >= inv_timer)
            {
                if (inv_mode == 0)
                {
                    inv_mode = 1;
                    oled.clear();
                    oled.set2X();
                    oled.setInvertMode(1);
                    text(" INVERTED ", 4, 3);
                    oled.setInvertMode(0);
                }
                else if (inv_mode == 1)
                {
                    inv_mode = 0;
                    oled.clear();
                    oled.set2X();
                    text("NORMAL MODE", 0, 3);
                }
                flag = 0;
                cross = 0;
                return;
            }
        }
        flag = rule_turn;
    }
}
