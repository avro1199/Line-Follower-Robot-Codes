#include "SSD1306AsciiWire.h"
#include <CD74HC4067.h>
#include <NewPing.h>
#include <EEPROM.h>

#define fan_speed 230

// motor driver pins
#define lmf 19
#define lmb 18
#define rmf 16
#define rmb 17

// button pins
#define btn_r 25
#define btn_l 32
#define btn_d 33
#define btn_u 34
#define btn_m 35

#define coreless 26
#define light 2

//             s0  s1  s2  s3
CD74HC4067 mux(12, 13, 27, 14);
NewPing sonar_f(23, 23, 100);
NewPing sonar_l(4, 4, 100);
NewPing sonar_r(5, 5, 100);
SSD1306AsciiWire oled;
String value;

// all adjustment variables
bool side;
float k_p, k_d, k_i;
byte sp, tsp, le, re, pos;
byte obs_dis, wall_dis;
bool en_pid, inv_mode, obs_side, u_turn_side;
byte rule_turn, rule_cross, normal_cross, counter, path[70];
byte st, brk, tbrk, node_delay, inv_timer, u_turn_timer, finish_timer, obs_delay;

// all global helper variables
byte spl, spr;
byte flag, cross;
byte f_dist, ir_dist, l_dist, r_dist;
unsigned long m1, m2, m3;

// variables for sensor data and calibration
uint16_t maximum[] = {0, 0, 0, 0, 0, 0, 0, 0};
uint16_t minimum[] = {4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095};
uint16_t thresh[] = {3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000};
uint16_t sensor, s[8], base[] = {128, 64, 32, 16, 8, 4, 2, 1};
uint8_t sum;

void setup()
{
    pinMode(btn_r, INPUT_PULLUP);
    pinMode(btn_l, INPUT_PULLUP);
    pinMode(btn_d, INPUT_PULLUP);
    pinMode(btn_u, INPUT_PULLUP);
    pinMode(btn_m, INPUT_PULLUP);
    pinMode(light, OUTPUT);

    Serial.begin(115200);
    Wire.setClock(400000L);
    Wire.begin();
    oled.begin(&SH1106_128x64, 0x3C);
    EEPROM.begin(170);

    ledcAttach(lmf, 1000, 8);
    ledcAttach(lmb, 1000, 8);
    ledcAttach(rmf, 1000, 8);
    ledcAttach(rmb, 1000, 8);
    ledcAttach(coreless, 10000, 8);
    ledcWrite(coreless, 0);

    motor(0, 0);
    home_screen();

    for (byte i = 0; i < 8; i++)
    {
        thresh[i] = EEPROM.readUShort(35 + i * 2);
        maximum[i] = EEPROM.readUShort(55 + i * 2);
        minimum[i] = EEPROM.readUShort(75 + i * 2);
    }

    for (byte i = 0; i < 70; i++)
    {
        path[i] = EEPROM.read(i + 100);
    }

    sp = EEPROM.read(0);
    rule_turn = EEPROM.read(1);
    rule_cross = EEPROM.read(2);
    normal_cross = EEPROM.read(3);
    u_turn_side = EEPROM.read(4);
    u_turn_timer = EEPROM.read(5);
    finish_timer = EEPROM.read(6);
    node_delay = EEPROM.read(7);
    tsp = EEPROM.read(8);
    tbrk = EEPROM.read(9);
    brk = EEPROM.read(10);
    le = EEPROM.read(11);
    re = EEPROM.read(12);
    st = EEPROM.read(13);
    wall_dis = EEPROM.read(14);
    inv_mode = EEPROM.read(15);
    obs_side = EEPROM.read(16);
    obs_dis = EEPROM.read(17);
    obs_delay = EEPROM.read(18);
    inv_timer = EEPROM.read(19);
    k_p = EEPROM.read(20);
    k_d = EEPROM.read(21);
    k_i = EEPROM.read(22);
    en_pid = EEPROM.read(23);


    counter = EEPROM.read(30);

    spl = sp - le;
    spr = sp - re;
}
void loop()
{
    int x = btn_read(btn_l);
    if (x == 1)
    {
        path_select();
    }
    else if (x == 2)
    {
        path_reset();
    }

    x = btn_read(btn_m);
    if (x > 0)
    {
    P:
        byte r = menu();
        if (r == 1)
        {
            oled.clear();
            oled.set2X();
            text("LineFollow", 4, 1);
            oled.set1X();
            text(" Cancel        Start ", 0, 7);
            while (1)
            {
                x = btn_read(btn_m);
                if (x > 0)
                {
                    line_follow();
                    break;
                }
                x = btn_read(btn_r);
                if (x > 0)
                {
                    line_follow();
                    break;
                }
                x = btn_read(btn_l);
                if (x > 0)
                {
                    goto P;
                }
            }
        }
        else if (r == 2)
        {
        f:
            counter = EEPROM.read(30);
            oled.clear();
            oled.set2X();
            text("PathFollow", 4, 1);
            oled.set1X();
            value = "Counter : " + String(counter);
            text(value, 30, 5);
            text(" Cancel  Start  Cntr ", 0, 7);
            while (1)
            {
                x = btn_read(btn_m);
                if (x > 0)
                {
                    path_follow();
                }
                x = btn_read(btn_r);
                if (x > 0)
                {
                    text("Counter Adjst ::", 1, 5);
                    adjustment(31);
                    delay(300);
                    goto f;
                }
                x = btn_read(btn_l);
                if (x > 0)
                {
                    goto P;
                }
            }
        }
        else if (r == 3)
        {
            oled.clear();
            oled.set2X();
            text("Calibration", 0, 1);
            oled.set1X();
            text(" Cancel        Start ", 0, 7);
            while (1)
            {
                x = btn_read(btn_m);
                if (x > 0)
                {
                    delay(500);
                    callibration();
                    break;
                }
                x = btn_read(btn_r);
                if (x > 0)
                {
                    delay(500);
                    callibration();
                    break;
                }
                x = btn_read(btn_l);
                if (x > 0)
                {
                    goto P;
                }
            }
        }
        else if (r == 4)
        {
            oled.clear();
            oled.set2X();
            text("Sensr Read", 0, 2);
            oled.set1X();
            text(" Analog      Digital ", 0, 7);
            while (1)
            {
                x = btn_read(btn_r);
                if (x > 0)
                {
                    show_digital(); // digital value
                    goto P;
                }
                x = btn_read(btn_l);
                if (x > 0)
                {
                    show_analog(); // analog value
                    goto P;
                }
                x = btn_read(btn_m);
                if (x > 0)
                {
                    goto P;
                }
            }
        }

        else if (r == 5)
        {
            oled.clear();
            oled.set2X();
            text(" STARTING", 04, 3);
            delay(1000);
            oled.clear();
            text("  RUNNING ", 04, 3);
            motor(10 * spl, 10 * spr);
            while (digitalRead(btn_m))
                ;
            while (!digitalRead(btn_m))
                ;
            motor(0, 0);
            goto P;
        }
        else if (r == 6)
        {
            // wall_follow();
            text("Wall Follow !", 0, 3);
            delay(500);
            goto P;
        }
        else if (r == 7)
        {
            oled.clear();
            oled.set2X();
            text("MAZE  SCAN", 4, 1);
            text(" 1      2 ", 1, 5);
            while (1)
            {
                x = btn_read(btn_r);
                if (x > 0)
                {
                    // maze_solve(); // 2
                    oled.clear();
                    oled.set2X();
                    text("UNAVAILABLE", 0, 2);
                    delay(500);
                    goto P;
                }

                x = btn_read(btn_l);
                if (x > 0)
                {
                    // maze_solve(); // 1
                    oled.clear();
                    oled.set2X();
                    text("UNAVAILABLE", 0, 2);
                    delay(500);
                    goto P;
                }
                x = btn_read(btn_m);
                if (x > 0)
                {
                    goto P;
                }
            }
        }
        else if (r == 8)
        {
            oled.clear();
            oled.set2X();
            text("MAZE SOLVE", 5, 1);
            text(" 1      2 ", 1, 5);
            while (1)
            {
                x = btn_read(btn_r);
                if (x > 0)
                {
                    // maze_scan(); // 2
                    oled.clear();
                    oled.set2X();
                    text("UNAVAILABLE", 0, 2);
                    delay(500);
                    goto P;
                }
                x = btn_read(btn_l);
                if (x > 0)
                {
                    // maze_scan(); // 1
                    oled.clear();
                    oled.set2X();
                    text("UNAVAILABLE", 0, 2);
                    delay(500);
                    goto P;
                }
                x = btn_read(btn_m);
                if (x > 0)
                {
                    goto P;
                }
            }
        }
    }

    x = btn_read(btn_r);
    if (x == 1)
    {
        adjustment_menu();
    }
    else if (x == 2)
    {
        print_max_min_avg();
        home_screen();
    }
}