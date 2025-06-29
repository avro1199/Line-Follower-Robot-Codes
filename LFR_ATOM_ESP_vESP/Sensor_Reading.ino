void sensor_read()
{
    sum = 0;
    sensor = 0;

    for (int i = 0; i < 8; i++)
    {
        mux.channel(i);
        delayMicroseconds(2);
        s[i] = analogRead(36);
        s[i] > thresh[i] ? s[i] = 0 ^ inv_mode : s[i] = 1 ^ inv_mode;

        sensor += s[i] * base[i];
        sum += s[i];
    }
}

void sonar_read()
{
    f_dist = sonar_f.ping_cm();
    l_dist = sonar_l.ping_cm();
    r_dist = sonar_r.ping_cm();
}

void show_digital()
{
    oled.set2X();
    while (digitalRead(btn_m))
    {
        sensor_read();
        sonar_read();

        oled.clear();
        oled.print(" ");
        oled.println(sensor, BIN);
        oled.print("     ");
        oled.println(sum);
        value = String(l_dist) + "  " + String(f_dist) + "  " + String(r_dist) + "  ";
        text(value, 5, 5);
        Serial.print(sensor, BIN);
        Serial.print(" -> ");
        Serial.print(sum);
        Serial.println();
        delay(50);
    }
    while (!digitalRead(btn_m))
        ;
    oled.set1X();
}

void show_analog()
{
    while (digitalRead(btn_m))
    {
        for (int i = 0; i < 8; i++)
        {
            mux.channel(i);
            delayMicroseconds(2);
            s[i] = analogRead(36);
        }
        for (int i = 0; i < 8; i++)
        {
            Serial.print(s[i]);
            Serial.print("  ");
        }
        Serial.println();
        delay(10);
        print_to_oled(s);
    }
    while (!digitalRead(btn_m))
        ;
}

void callibration()
{
    byte i = 0;
    oled.clear();
    oled.set2X();
    text("PROCESSING", 4, 3);
    for (i = 0; i < 8; i++)
    {
        maximum[i] = 0;
        minimum[i] = 4095;
    }

    motor(-170, 170);

    for (short int c = 0; c < 3500; c++)
    {
        for (i = 0; i < 8; i++)
        {
            mux.channel(i);
            delayMicroseconds(2);
            s[i] = analogRead(36);
            maximum[i] = max(maximum[i], s[i]);
            minimum[i] = min(minimum[i], s[i]);
        }
        delay(2);
    }

    motor(0, 0);

    for (i = 0; i < 8; i++)
    {
        thresh[i] = (maximum[i] - minimum[i]) * 0.5 + minimum[i];

        EEPROM.writeUShort(35 + i * 2, thresh[i]);
        delay(10);
        EEPROM.writeUShort(55 + i * 2, maximum[i]);
        delay(10);
        EEPROM.writeUShort(75 + i * 2, minimum[i]);
        delay(10);
    }
    EEPROM.commit();

    oled.clear();
    text("   DONE   ", 04, 03);
    delay(300);

    print_max_min_avg();

    home_screen();
}

// prints 10 values to the attached oled display
void print_to_oled(uint16_t data[])
{
    oled.set1X();
    oled.clear();
    for (int i = 2; i >= 0; i--)
    {
        oled.setCursor(0, 2 * i);
        oled.print(data[2 - i]);
    }
    for (int i = 3; i < 6; i++)
    {
        oled.print(" ");
        if (i == 3 || i == 5)
        {
            oled.print(" ");
        }
        oled.print(data[i]);
    }
    for (int i = 6; i < 8; i++)
    {
        oled.setCursor(102, 2 * (i - 5));
        oled.print(data[i]);
    }
}

// prints the current sensors max min and threshold values
void print_max_min_avg()
{
    print_to_oled(maximum);
    oled.set2X();
    text("MAX", 45, 4);
    while (digitalRead(btn_m) && digitalRead(btn_r))
        ;
    while (!digitalRead(btn_m) || !digitalRead(btn_r))
        ;

    print_to_oled(minimum);
    oled.set2X();
    text("MIN", 45, 4);
    while (digitalRead(btn_m) && digitalRead(btn_r))
        ;
    while (!digitalRead(btn_m) || !digitalRead(btn_r))
        ;

    print_to_oled(thresh);
    oled.set2X();
    text("AVG", 45, 4);
    while (digitalRead(btn_m) && digitalRead(btn_r))
        ;
    while (!digitalRead(btn_m) || !digitalRead(btn_r))
        ;
}