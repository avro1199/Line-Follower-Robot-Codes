void motor(int lms, int rms)
{
    if (lms >= 0)
    {
        analogWrite(lmf, lms);
        analogWrite(lmb, 0);
    }
    else
    {
        lms *= -1;
        analogWrite(lmf, 0);
        analogWrite(lmb, lms);
    }

    if (rms >= 0)
    {
        analogWrite(rmf, rms);
        analogWrite(rmb, 0);
    }
    else
    {
        rms *= -1;
        analogWrite(rmf, 0);
        analogWrite(rmb, rms);
    }
}

void brake()
{
    analogWrite(lmf, 255);
    analogWrite(lmb, 255);
    analogWrite(rmf, 255);
    analogWrite(rmb, 255);

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
    analogWrite(coreless, speed);
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