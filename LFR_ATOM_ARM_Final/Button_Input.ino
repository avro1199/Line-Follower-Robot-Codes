byte btn_read(byte btn_pin)
{
    m1 = m2 = millis();
    while (digitalRead(btn_pin) == LOW)
    {
        m2 = millis();
        if (m2 - m1 > 350)
            digitalWrite(led, 1);
    }
    digitalWrite(led, 0);
    if (m2 - m1 > 15 && m2 - m1 < 350)
        return 1;
    else if (m2 - m1 > 350)
        return 2;
    else
        return 0;
}