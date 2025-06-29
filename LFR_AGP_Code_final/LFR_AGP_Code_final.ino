//#include <NewPing.h>
//
//NewPing sonarfront(6, 6, 20);
//
//int mid_distance = 10;

#define lmf 4
#define lmb 5
#define lms 3
#define rmf 9
#define rmb 8
#define rms 10
#define sw 12
#define head_light 11

#define spl 25
#define spr 18
#define dr 3
#define tsp 70
#define tbr 50
#define side 2
#define node_delay 40
#define stop_timer 150
#define u_turn_timer 50
#define return_delay 450
#define highest_brake 0
#define lowest_brake 0
#define brake_constant 1
#define br_delay 50

int strt = lowest_brake * brake_constant;
int peak = highest_brake * brake_constant;
int cl = strt;

int pos = 0;
char flag = 's';
uint32_t m1, m2, m3, s1;

int s[6], sensor, sum = 0, distance;
int base[6] = {1, 2, 4, 8, 16, 32};

int mid[6] = {1015, 1015, 1015, 1015, 1015, 1015};

byte counter = 0;
byte cross = 0;

void setup() {
  pinMode(lmf , OUTPUT);
  pinMode(lmb , OUTPUT);
  pinMode(rmf , OUTPUT);
  pinMode(rmb , OUTPUT);
  pinMode(head_light, OUTPUT);
  pinMode(sw, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  int r = push();
  if (r != 0) {
    if (r == 1) line_follow();
    else if (r == 2) while (digitalRead(sw)) motor(10 * spl, 10 * spr);
    else if (r == 3) while (digitalRead(sw)) show_analog();
    else if (r == 4) while (digitalRead(sw)) show_digital();

    motor(0, 0);

    delay(300);
  }

}
