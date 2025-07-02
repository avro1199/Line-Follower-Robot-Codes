void single_line_follow()
{
    if (c_t - p_t > pid_timer)
    {
        //........... |............|............|...........|||..........|||...........|............|............|
        line_pos = (s[1] * 1.0 + s[2] * 2.0 + s[3] * 3.0 + s[4] * 4.0 + s[5] * 5.0 + s[6] * 6.0 + s[7] * 7.0 + s[8] * 8.0) / (sum);
        error = line_pos - 4.5;


        integral = integral + error;
        derivative = error - last_error;
        last_error = error;
        Serial.print(line_pos);
        Serial.print(" <=> ");
        Serial.print(sum);
        Serial.print(" <=> ");
        Serial.print(error);
        pid = error * k_p + integral * k_i + derivative * k_d;
        Serial.print(" <=> ");
        Serial.print(pid);
        Serial.print(" || D => ");
        Serial.print(derivative);
        Serial.print(" || P => ");
        Serial.print(error * k_p);
        pid = constrain(pid, -200 - base_speed, 200 - base_speed);

        int ls, rs;
        ls = constrain(base_speed + pid, -base_speed, base_speed+30);
        rs = constrain(base_speed - pid, -base_speed, base_speed+30);

        Serial.print("........(");
        Serial.print(ls);
        Serial.print(" , ");
        Serial.print(rs);
        Serial.println(")");

        motor(ls, rs);
        // Implement delay to control the frequency of PID updates
        p_t = c_t;
    }
}

void pid_reset()
{
    pid = 0;
    error = 0;
    integral = 0;
    last_error = 0;
    derivative = 0;
}