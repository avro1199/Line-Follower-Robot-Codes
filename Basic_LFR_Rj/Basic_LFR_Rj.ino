#define lms 5
#define lmf 3
#define lmb 4

#define rms 6
#define rmf 10
#define rmb 8

#define spl 18
#define spr 18
#define turn_speed 120

#define sw 12  //Button
#define head_light 13

#define stop_timer 150

char turn = 's', cross = 's';
uint32_t m1, m2, s1;

int s[6], sensor, sum, distance;
int base[6] = { 1, 2, 4, 8, 16, 32 };
int mid[6] = { 100, 100, 100, 100, 100, 100 }; //change according to the analog reading
void setup() {
  pinMode(lmf, OUTPUT);
  pinMode(lmb, OUTPUT);
  pinMode(rmf, OUTPUT);
  pinMode(rmb, OUTPUT);
  pinMode(head_light, OUTPUT);

  pinMode(sw, INPUT_PULLUP);

  Serial.begin(115200);
}

void loop() {
  int r = button_read();
  if (r != 0) {
    if (r == 1) line_follow();                   // single press to line follow
    else if (r == 2) motor(10 * spl, 10 * spr);  // double press to motor check for straight
    else if (r == 3)
      while (1) show_analog();  // triple press to show analog sensor readings
    else if (r == 4)
      while (1) show_digital();  // 4 press to show digital sensor readings
  }
}
