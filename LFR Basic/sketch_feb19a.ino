#define lms 5
#define rms 6
#define lmf 3
#define lmb 4
#define rmf 10
#define rmb 8

//all speed
#define sp 8
#define tsp 120

int s[6];
int base[] = {32, 16, 8, 4, 2, 1};
int thresh[] = {200, 200, 200, 200, 200, 200};
int sensor;
byte sum;

//control
byte flag = 0, cross = 0;

void setup() {
  // put your setup code here, to run once:
  for(int i = 14; i < 20; i++)
  {
    pinMode(i, INPUT);
  }
  pinMode(lms, OUTPUT);
  pinMode(lmf, OUTPUT);
  pinMode(lmb, OUTPUT);
  pinMode(rms, OUTPUT);
  pinMode(rmf, OUTPUT);
  pinMode(rmb, OUTPUT);

  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:

  analog_print();
  // digital_print();
  // line_follow();

  //Motor Check
  // motor(100, 100);
  // delay(2000);
  // motor(-100, 100);
  // delay(2000);

  // motor(-100, -100);
  // delay(1000);

}
