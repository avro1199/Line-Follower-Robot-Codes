void read_sensor()
{
  sum = 0;
  sensor = 0;

  for(int i = 0; i < 6; i++)
  {
    s[i] = analogRead(i+14);
    s[i] = s[i] > thresh[i];
    sensor += s[i] * base[i];
    sum += s[i];
  }
}

void analog_print()
{
  for(int i = 0; i < 6; i++)
  {
    s[i] = analogRead(i+14);
    Serial.print(s[i]);
    Serial.print(" <-> ");
  }
  Serial.println();
}

void digital_print()
{
  read_sensor();
  Serial.print(sensor, BIN);
  Serial.print(" => ");
  Serial.println(sum);
}