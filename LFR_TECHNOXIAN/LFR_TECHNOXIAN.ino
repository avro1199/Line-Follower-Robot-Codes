#include "SSD1306AsciiWire.h"
#include <EEPROM.h>

SSD1306AsciiWire oled;
String value;

#define left_motor_forward 7
#define left_motor_backward 8
#define right_motor_forward 3
#define right_motor_backward 4
#define left_motor_speed 6
#define right_motor_speed 5
#define light 13
#define swl 11
#define swr 12

// #define side 1 // also act as y_turn

// for line follow
byte sp, spl, spr, le, re, tsp;
byte dt, br, tbr, inv_timer, u_turn_timer, finish_timer, obs_delay;
byte flag, node, cross, side, normal_cross, rule_turn, rule_cross , counter, path[50];
byte obs_dis, l_wall_dis, r_wall_dis, left_dis, front_dis, right_dis;
short int pos;
unsigned long m1, m2, m3;

bool inv_mode, obs_side;
short int s[6];                                         // this is for storing analog value of each sensor
short int threshold[] = {600, 600, 600, 600, 600, 600}; // this is the mid value for each sensor. use this array if you want to adjust midoint of each sensor individually
short int base[6] = {1, 2, 4, 8, 16, 32};               // this is for binary to decimal conversion
short int sensor;                                       // this is to store final value after binary conversion
byte sum = 0;                                           // this is to store the count of detected sensor
short int minimum[] = {1023, 1023, 1023, 1023, 1023, 1023};
short int maximum[] = {0, 0, 0, 0, 0, 0};
// DRMC hack

void setup()
{
  Wire.begin();
  Wire.setClock(400000L);
  oled.begin(&Adafruit128x64, 0x3C);
  oled.setInvertMode(0);
  home_screen();
  pinMode(left_motor_forward, OUTPUT);
  pinMode(left_motor_backward, OUTPUT);
  pinMode(left_motor_speed, OUTPUT);
  pinMode(right_motor_forward, OUTPUT);
  pinMode(right_motor_backward, OUTPUT);
  pinMode(right_motor_speed, OUTPUT);
  pinMode(light, OUTPUT);
  pinMode(swl, INPUT_PULLUP);
  pinMode(swr, INPUT_PULLUP);

  for (byte i = 0; i < 6; i++)
  {
    threshold[i] = EEPROM.read(i) * 5;
    maximum[i] = EEPROM.read(i + 6) * 5;
    minimum[i] = EEPROM.read(i + 12) * 5;
  }
  sp = EEPROM.read(18);
  le = EEPROM.read(19);
  re = EEPROM.read(20);
  dt = EEPROM.read(21);
  br = EEPROM.read(22);
  tbr = EEPROM.read(23);
  tsp = EEPROM.read(24);
  node = EEPROM.read(25);
  u_turn_timer = EEPROM.read(26);
  finish_timer = EEPROM.read(27);
  spl = sp - le;
  spr = sp - re;
  side = EEPROM.read(28);
  obs_delay = EEPROM.read(29);
  obs_dis = EEPROM.read(30);
  l_wall_dis = EEPROM.read(31);
  r_wall_dis = EEPROM.read(32);
  rule_turn = EEPROM.read(33);
  rule_cross = EEPROM.read(34);
  inv_mode = EEPROM.read(35);
  inv_timer = EEPROM.read(36);
  normal_cross = EEPROM.read(37);
  obs_side = EEPROM.read(38);

  counter = EEPROM.read(39);
  for (byte i = 0; i < 49; i++)
    path[i] = EEPROM.read(i + 40);
  Serial.begin(115200);
}

void loop()
{
  byte x = lswitch();
  if (x == 2)
    path_reset();
  else if (x == 1)
    path_select();
  x = rswitch();
  if (x == 1)
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
        x = rswitch();
        if (x == 1 || x == 2)
        {
          line_follow();
          break;
        }
        x = lswitch();
        if (x == 1 || x == 2)
        {
          goto P;
        }
      }
    }
    else if (r == 2)
    {
    f:
      counter = EEPROM.read(39);
      oled.clear();
      oled.set2X();
      text("PathFollow", 4, 1);
      oled.set1X();
      value = "Counter : " + String(counter);
      text(value, 30, 5);
      text(" Counter       Start ", 0, 7);
      while (1)
      {
        x = rswitch();
        if (x == 1 || x == 2)
        {
          fixed_path();
          break;
        }
        x = lswitch();
        if (x == 1)
        {
          adjustment(22);
          goto f;
        }
        else if (x == 2)
        {
          goto P;
        }
      }
    }
    else if (r == 3)
      callibration();
    else if (r == 4)
    {
      oled.clear();
      oled.set2X();
      text("Sensor Check", 0, 2);
      oled.set1X();
      text(" Analog      Digital ", 0, 7);
      while (1)
      {
        x = rswitch();
        if (x == 1)
        {
          sensordisp(); // digital value
          goto P;
        }
        else if (x == 2)
        {
          goto P;
        }
        x = lswitch();
        if (x == 1)
        {
          analogReadDisp(); // analog value
          goto P;
        }
        else if (x == 2)
        {
          goto P;
        }
      }
    }

    else if (r == 5)
    {
      // wall_follow();
      goto P;
    }
    else if (r == 6)
    {
      oled.clear();
      oled.set2X();
      text(" STARTING", 04, 3);
      delay(1000);
      oled.clear();
      text("  RUNNING ", 04, 3);
      motor(10 * spl, 10 * spr);
      while (digitalRead(swl) == HIGH && digitalRead(swr) == HIGH)
        ;
      while (digitalRead(swl) == LOW || digitalRead(swr) == LOW)
        ;
      motor(0, 0);
      goto P;
    }
    else if (r == 7)
    {
      oled.clear();
      oled.set2X();
      text("MAZE SOLVE", 4, 1);
      text(" 1      2 ", 1, 5);
      while (1)
      {
        x = rswitch();
        if (x == 1)
        {
          // maze_solve(); // 2
          oled.clear();
          oled.set2X();
          text("UNAVAILABLE", 0, 2);
          delay(500);
          goto P;
        }
        else if (x == 2)
        {
          goto P;
        }
        x = lswitch();
        if (x == 1)
        {
          // maze_solve(); // 1
          oled.clear();
          oled.set2X();
          text("UNAVAILABLE", 0, 2);
          delay(500);
          goto P;
        }
        else if (x == 2)
        {
          goto P;
        }
      }
    }
    else if (r == 8)
    {
      oled.clear();
      oled.set2X();
      text("MAZE SCAN", 5, 1);
      text(" 1      2 ", 1, 5);
      while (1)
      {
        x = rswitch();
        if (x == 1)
        {
          // maze_scan(); // 2
          oled.clear();
          oled.set2X();
          text("UNAVAILABLE", 0, 2);
          delay(500);
          goto P;
        }
        else if (x == 2)
        {
          goto P;
        }
        x = lswitch();
        if (x == 1)
        {
          // maze_scan(); // 1
          oled.clear();
          oled.set2X();
          text("UNAVAILABLE", 0, 2);
          delay(500);
          goto P;
        }
        else if (x == 2)
        {
          goto P;
        }
      }
    }
  }
  else if (x == 2)
    adjustment_menu();
}
