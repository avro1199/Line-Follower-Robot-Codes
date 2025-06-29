int push() {
  int count = 0;
p:  int t = 0;
  if (!digitalRead(sw)) {
    digitalWrite(head_light, HIGH);
    while (!digitalRead(sw)) {
      delay(1); t++;
    }
    digitalWrite(head_light, LOW);
    if (t > 15) {
      count++;
      t = 0;
      while(digitalRead(sw)){
        delay(1); t++;
        if(t>1000) break;
      }
      if(t<1000) goto p;
    }
  }
  return count;
}
