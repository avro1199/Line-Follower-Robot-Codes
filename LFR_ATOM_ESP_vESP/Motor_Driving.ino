void motor(int lms, int rms)
{
    if (lms >= 0)
    {
        ledcWrite(lmf, lms);
        ledcWrite(lmb, 0);
    }
    else
    {
        lms *= -1;
        ledcWrite(lmf, 0);
        ledcWrite(lmb, lms);
    }

    if (rms >= 0)
    {
        ledcWrite(rmf, rms);
        ledcWrite(rmb, 0);
    }
    else
    {
        rms *= -1;
        ledcWrite(rmf, 0);
        ledcWrite(rmb, rms);
    }
}

void brake()
{
    ledcWrite(lmf, 255);
    ledcWrite(lmb, 255);
    ledcWrite(rmf, 255);
    ledcWrite(rmb, 255);

    delay(5);
}

void hard_brake()
{
    motor(-100, -100);
    delay(brk); // br
    motor(0, 0);
}

void fan(uint8_t speed)
{
    ledcWrite(coreless, speed);
}

void coreless_check()
{
    uint64_t t;
    oled.clear();
    oled.set2X();
    for (int i = 10; i <= 250; i += 20)
    {
        t = millis();
        fan(i);
        oled.clear();
        oled.print(i);
    x:
        if (!digitalRead(btn_m))
        {
            break;
        }
        if (millis() - t < 4000)
        {
            goto x;
        }
    }
    fan(0);
    oled.set1X();
}