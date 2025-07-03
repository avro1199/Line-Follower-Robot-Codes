// All Libraries
#include "Arduino.h"
#include <Wire.h>
// #include <EEPROM.h>
#include <at24c256.h>
#include <NewPing.h>
// #include <CD74HC4067.h>
#include <light_CD74HC4067.h>
#include "SSD1306AsciiWire.h"

// Comment this line for low memory boards(Arduino Uno, Nano, Blue Pill, etc.)
#define BAR // High memory usage but better analog display

#ifdef BAR
#include <Adafruit_SH110X.h>
#include <Adafruit_GFX.h>
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &Wire);
#endif

#define PID_FREQ 10000 // Hz
#define NUM_SENSORS 14 // Number of sensors in the array

// Define Pins
#define lmf PA6
#define lmb PB5
#define rmf PB7
#define rmb PB6
#define led PB12
#define btn_l PA1
#define btn_u PA2
#define btn_m PA3
#define btn_r PC14
#define btn_d PC15
#define MUX_S0 PA7
#define MUX_S1 PB4
#define MUX_S2 PB0
#define MUX_S3 PB1


// Objects
SSD1306AsciiWire oled;
AT24C256 EEPROM(AT24C_ADDRESS_0);
// CD74HC4067 16-Channel Multiplexer
CD74HC4067 mux(MUX_S0, MUX_S1, MUX_S2, MUX_S3);
// NewPing sonar objects for ultrasonic sensors
NewPing sonar_f(PB3, PB3, 80);
NewPing sonar_r(PB13, PB13, 50);
NewPing sonar_l(PA15, PA15, 50);

// Global Vars
String value;

// all adjustment variables
bool side;
float k_p, k_d, k_i;
byte sp, tsp, le, re;
byte obs_dis, wall_dis;
bool en_pid, inv_mode, obs_side, u_turn_side;
byte rule_turn, rule_cross, normal_cross, counter, path[70];
byte brk, tbrk, node_delay, inv_timer, u_turn_timer, finish_timer, obs_delay;
int st, pos; // st, node_delay not needed in pid // pos not needed in pid

// all pid variables
byte base_speed;
int pid, last_pid;
u_int64_t c_t, p_t = 0;
u_int32_t pid_timer = 1000000L / PID_FREQ;
float line_pos, error, last_error, integral, derivative;

// all global helper variables
byte spl, spr;
byte flag, cross;
byte f_dist, ir_dist, l_dist, r_dist;
unsigned long m1, m2, m3;

// variables for sensor data and calibration
uint16_t maximum[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
uint16_t minimum[] = {4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 
                      4095, 4095, 4095, 4095};
uint16_t thresh[] = {3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 
                     3000, 3000, 3000, 3000}; // Threshold values for sensors
uint16_t sensor, s[NUM_SENSORS], base[] = {8192, 4096, 2048, 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1};
uint8_t sum;

void setup()
{
    pinMode(PA5, INPUT); // Analog Input from Sensor Array
    pinMode(lmf, OUTPUT);
    pinMode(lmb, OUTPUT);
    pinMode(rmf, OUTPUT);
    pinMode(rmb, OUTPUT);
    pinMode(led, OUTPUT); // Status LED
    pinMode(btn_d, INPUT_PULLUP);
    pinMode(btn_u, INPUT_PULLUP);
    pinMode(btn_l, INPUT_PULLUP);
    pinMode(btn_r, INPUT_PULLUP);
    pinMode(btn_m, INPUT_PULLUP);
    // MUX select pins as outputs
    pinMode(MUX_S0, OUTPUT);
    pinMode(MUX_S1, OUTPUT);
    pinMode(MUX_S2, OUTPUT);
    pinMode(MUX_S3, OUTPUT);
    //I2C pins
    pinMode(PB8, OUTPUT); // SCL
    pinMode(PB9, OUTPUT); // SDA

    analogReadResolution(12); // 12-bit ADC(0-4095)
    analogWriteResolution(8); // 8-bit PWM(0-255)
    analogWriteFrequency(20000); // 20kHz PWM

    Serial.begin(115200);
    delay(1000);

    Wire.setSCL(PB8);
    Wire.setSDA(PB9);
    Wire.setClock(400000L);
    Wire.begin();

    oled.begin(&SH1106_128x64, 0x3C);
    delay(100);

    // Everything Off
    motor(0, 0);
    digitalWrite(led, 0);
    home_screen();

    for (byte i = 0; i < NUM_SENSORS; i++)
    {
        thresh[i] = readUShort(50 + i * 2);
        maximum[i] = readUShort(90 + i * 2);
        minimum[i] = readUShort(130 + i * 2);
    }

    for (byte i = 0; i < 70; i++)
    {
        path[i] = EEPROM.read(i + 200);
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

    delay(100);
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
                    home_screen();
                    break;
                }
                x = btn_read(btn_r);
                if (x > 0)
                {
                    line_follow();
                    home_screen();
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
                    home_screen();
                    break;
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
            wall_follow();
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
                    //Checking motor PWM
                    while(digitalRead(btn_read(btn_m)))
                    {
                        motor(0, 0);
                        delay(1000);
                        motor(100, 100);
                        delay(1000);
                        motor(200, 200);
                        delay(1000);
                        motor(255, 255);
                        delay(1000);
                        motor(0, 0);
                        delay(1000);
                        motor(-100, -100);
                        delay(1000);
                        motor(-200, -200);
                        delay(1000);
                        motor(-255, -255);
                        delay(1000);
                    }
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