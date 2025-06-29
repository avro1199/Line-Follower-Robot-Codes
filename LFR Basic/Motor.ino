void motor(int ls, int rs)
{
  if(ls >= 0)
  {
    digitalWrite(lmf, 1);
    digitalWrite(lmb, 0);
    analogWrite(lms, ls);
  }
  else
  {
    ls *= -1;
    digitalWrite(lmf, 0);
    digitalWrite(lmb, 1);
    analogWrite(lms, ls);
  }

  if(rs >= 0)
  {
    digitalWrite(rmf, 1);
    digitalWrite(rmb, 0);
    analogWrite(rms, ls);
  }
  else
  {
    ls *= -1;
    digitalWrite(rmf, 0);
    digitalWrite(rmb, 1);
    analogWrite(rms, rs);
  }
}