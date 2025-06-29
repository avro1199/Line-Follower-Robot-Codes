void motor(short int a, short int b)
{
  if (a > 0)
  {
    digitalWrite(left_motor_forward, 1);
    digitalWrite(left_motor_backward, 0);
  }
  else if (a < 0)
  {
    a = -a;
    digitalWrite(left_motor_forward, 0);
    digitalWrite(left_motor_backward, 1);
  }
  else
  {
    digitalWrite(left_motor_forward, 0);
    digitalWrite(left_motor_backward, 0);
  }
  if (b > 0)
  {
    digitalWrite(right_motor_forward, 1);
    digitalWrite(right_motor_backward, 0);
  }
  else if (b < 0)
  {
    b = -b;
    digitalWrite(right_motor_forward, 0);
    digitalWrite(right_motor_backward, 1);
  }
  else
  {
    digitalWrite(right_motor_forward, 0);
    digitalWrite(right_motor_backward, 0);
  }
  analogWrite(left_motor_speed, a);
  analogWrite(right_motor_speed, b);
}

void nutral()
{
  digitalWrite(left_motor_forward, 1);
  digitalWrite(left_motor_backward, 1);
  digitalWrite(right_motor_forward, 1);
  digitalWrite(right_motor_backward, 1);

  digitalWrite(left_motor_speed, 1);
  digitalWrite(right_motor_speed, 1);

  delay(5);
}

void short_brake()
{
  digitalWrite(left_motor_forward, 1);
  digitalWrite(left_motor_backward, 1);
  digitalWrite(right_motor_forward, 1);
  digitalWrite(right_motor_backward, 1);

  digitalWrite(left_motor_speed, 1);
  digitalWrite(right_motor_speed, 1);

  delay(10);
}

void hard_brake()
{
  motor(-100, -100); //-100 -100
  delay(br);
  motor(0, 0);
}