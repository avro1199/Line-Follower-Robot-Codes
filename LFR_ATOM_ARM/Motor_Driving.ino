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

//TB6612FNG Breaking
void brake()
{
    analogWrite(lmf, 255);
    analogWrite(lmb, 255);
    analogWrite(rmf, 255);
    analogWrite(rmb, 255);

    delay(10);
}

void hard_brake()
{
    motor(-100, -100);
    delay(100);
    // delay(brk); // br
    motor(0, 0);
}