#include <NewPing.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define lcd_addr 0x3C

#define lmf 7
#define lmb 8
#define lms 6
#define rmf 3
#define rmb 4
#define rms 5
#define sw 12
#define head_light 13

#define spl 15
#define spr 15
#define dr 3
#define tsp 80
#define tbr 10
#define node_delay 2
#define stop_timer 150
#define u_turn_timer 100
#define return_delay 450
#define highest_brake 0
#define lowest_brake 0
#define brake_constant 1
#define br_delay 10

NewPing sonarleft(2, 2, 20);
NewPing sonarfront(9, 9, 20);
Adafruit_SSD1306 lcd(128, 64, &Wire, -1);

int mid_distance = 7;
int front_distance = 10;

int strt = lowest_brake * brake_constant;
int peak = highest_brake * brake_constant;
int cl = strt;

int pos = 0;
char flag = 's';
uint32_t m1, m2, m3, s1;

int s[6], sensor, sum = 0, distance_l, distance_f, distance_r;
int base[6] = {1, 2, 4, 8, 16, 32};

int mid[6] = {600, 600, 600, 600, 600, 600};

byte counter = 0;
byte cross = 0;
byte side = 2;

void setup()
{
  pinMode(lmf, OUTPUT);
  pinMode(lmb, OUTPUT);
  pinMode(rmf, OUTPUT);
  pinMode(rmb, OUTPUT);
  pinMode(head_light, OUTPUT);
  pinMode(sw, INPUT_PULLUP);
  Serial.begin(115200);
  lcd.begin(SSD1306_SWITCHCAPVCC, lcd_addr);

  delay(200);

  lcd.setTextColor(SSD1306_WHITE);
  main_menu();
}

void loop()
{
  int r = push();
  if (r != 0)
  {
    if (r == 1)
    {
      lcd.clearDisplay();
      lcd.setTextSize(2);
      lcd.setCursor(40, 10);
      lcd.print("Line");
      lcd.setCursor(30, 40);
      lcd.print("Follow");
      lcd.display();
      line_follow();
      main_menu();
    }
    else if (r == 2)
    {
      lcd.clearDisplay();
      lcd.setTextSize(2);
      lcd.setCursor(35, 10);
      lcd.print("Motor");
      lcd.setCursor(18, 40);
      lcd.print("Checking");
      lcd.display();
      while (digitalRead(sw))
        motor(10 * spl, 10 * spr);
      delay(200);
      main_menu();
    }
    else if (r == 3)
    {
      lcd.clearDisplay();
      lcd.setTextSize(2);
      lcd.setCursor(26, 10);
      lcd.print("Analog");
      lcd.setCursor(20, 40);
      lcd.print("Readings");
      lcd.display();
      delay(500);
      lcd.setTextSize(1);
      while (digitalRead(sw))
        show_analog();
      delay(200);
      main_menu();
    }
    else if (r == 4)
    {
      lcd.clearDisplay();
      lcd.setTextSize(2);
      lcd.setCursor(25, 10);
      lcd.print("Digital");
      lcd.setCursor(20, 40);
      lcd.print("Readings");
      lcd.display();
      delay(500);
      lcd.setTextSize(2);
      while (digitalRead(sw))
        show_digital();
      delay(200);
      main_menu();
    }
    else if (r == 5)
    {
      while (digitalRead(sw))
        show_sonar();
      delay(200);
    }
    else if (r == 6)
    {
      while (digitalRead(sw))
        wall_follow();
      delay(200);
    }

    motor(0, 0);

    delay(100);
  }
}