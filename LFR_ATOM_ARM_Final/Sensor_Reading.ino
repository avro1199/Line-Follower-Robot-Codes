void sensor_read()
{
    sum = 0;
    sensor = 0;

    for (int i = 0; i < NUM_SENSORS; i++)
    {
        mux.channel(i);
        s[i] = analogRead(PA5);
        s[i] > thresh[i] ? s[i] = 0 ^ inv_mode : s[i] = 1 ^ inv_mode;
    }
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        sensor += s[i] * base[i];
        sum += s[i];
    }
    c_t = micros();
}

void sonar_read()
{
    f_dist = sonar_f.ping_cm();
    l_dist = sonar_l.ping_cm();
    r_dist = sonar_r.ping_cm();
}

void show_digital()
{
    oled.clear();
    while (digitalRead(btn_m))
    {
        sensor_read();
        sonar_read();

        oled.set1X();
        // oled.print(" ");
        oled.setCursor(20, 0);
        oled.print((sensor >> 13) & 1);
        oled.print((sensor >> 12) & 1);
        oled.print((sensor >> 11) & 1);
        oled.print((sensor >> 10) & 1);
        oled.print((sensor >> 9) & 1);
        oled.print((sensor >> 8) & 1);
        oled.print((sensor >> 7) & 1);
        oled.print((sensor >> 6) & 1);
        oled.print((sensor >> 5) & 1);
        oled.print((sensor >> 4) & 1);
        oled.print((sensor >> 3) & 1);
        oled.print((sensor >> 2) & 1);
        oled.print((sensor >> 1) & 1);
        oled.print(sensor & 1);

        oled.set2X();
        oled.setCursor(55, 2);
        oled.print(sum);
        oled.print("  ");
        // value = String(l_dist) + "  " + String(f_dist) + "  " + String(r_dist) + "  ";
        text(String(l_dist) + " ", 5, 5);
        text(String(f_dist) + " ", 50, 5);
        text(String(r_dist) + " ", 95, 5);
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
    oled.clear();

#ifdef BAR
    bool bar = 0;
    oled.set2X();
    text("Bar    Raw", 4, 3);
    while (1)
    {
        if (!digitalRead(btn_l))
        {
            bar = 1;
            oled.clear();
            display.begin(0x3C);
            break;
        }
        else if (!digitalRead(btn_r))
        {
            bar = 0;
            oled.clear();
            break;
        }
    }
#endif

    while (digitalRead(btn_m))
    {
        for (int i = 0; i < NUM_SENSORS; i++)
        {
            mux.channel(i);
            s[i] = analogRead(PA5);
        }
        for (int i = 0; i < NUM_SENSORS; i++)
        {
            Serial.print(s[i]);
            Serial.print("  ");
        }
        Serial.println();

#ifdef BAR
        if (bar)
        {
            show_as_bars(s, NUM_SENSORS, 0, 4095, 1);
        }
        else
        {
            print_to_oled(s);
        }
#else
        print_to_oled(s);
#endif

        delay(10);
    }
    while (!digitalRead(btn_m))
        ;
#ifdef BAR
    if (bar)
    {
        display.clearDisplay();
        oled.begin(&SH1106_128x64, 0x3C);
    }
#endif

    delay(100);
}

void callibration()
{
    byte i = 0;
    oled.clear();
    oled.set2X();
    text("PROCESSING", 4, 3);
    for (i = 0; i < NUM_SENSORS; i++)
    {
        maximum[i] = 0;
        minimum[i] = 4095;
    }

    motor(-170, 170);

    for (short int c = 0; c < 2000; c++)
    {
        for (i = 0; i < NUM_SENSORS; i++)
        {
            mux.channel(i);
            s[i] = analogRead(PA5);
            maximum[i] = max(maximum[i], s[i]);
            minimum[i] = min(minimum[i], s[i]);
        }
        delay(2);
    }

    motor(0, 0);

    for (i = 0; i < NUM_SENSORS; i++)
    {
        thresh[i] = (maximum[i] - minimum[i]) * 0.5 + minimum[i];

        writeUShort(50 + i * 2, thresh[i]);
        delay(1);
        writeUShort(90 + i * 2, maximum[i]);
        delay(1);
        writeUShort(130 + i * 2, minimum[i]);
        delay(1);
    }
    // EEPROM.commit();

    oled.clear();
    text("   DONE   ", 4, 3);
    delay(300);

    print_max_min_avg();

    home_screen();
}

// prints values[NUM_SENSORS] to the attached oled display
void print_to_oled(uint16_t data[])
{
    oled.set1X();
    // oled.clear();
    for (int i = 5; i >= 0; i--)
    {
        oled.setCursor(0, i);
        oled.print(data[5 - i]);
        oled.print("    ");
    }
    for (int i = 6; i < 8; i++)
    {
        oled.setCursor(34 * (i - 5), 0);
        oled.print(data[i]);
        oled.print("    ");
    }
    for (int i = 8; i < 14; i++)
    {
        oled.setCursor(102, (i - 8));
        oled.print(data[i]);
        oled.print("    ");
    }
}

// prints the current sensors max min and threshold values
void print_max_min_avg()
{
Max:
    oled.clear();
    print_to_oled(maximum);
    oled.set2X();
    text("MAX", 45, 4);
    while (digitalRead(btn_m) && digitalRead(btn_r))
        ;
    while (!digitalRead(btn_m) || !digitalRead(btn_r))
        ;

Min:
    oled.clear();
    print_to_oled(minimum);
    oled.set2X();
    text("MIN", 45, 4);
    while (digitalRead(btn_m) && digitalRead(btn_r))
    {
        if (!digitalRead(btn_l))
            goto Max;
    }
    while (!digitalRead(btn_m) || !digitalRead(btn_r))
        ;

    oled.clear();
    print_to_oled(thresh);
    oled.set2X();
    text("AVG", 45, 4);
    while (digitalRead(btn_m) && digitalRead(btn_r))
    {
        if (!digitalRead(btn_l))
            goto Min;
    }
    while (!digitalRead(btn_m) || !digitalRead(btn_r))
        ;
}