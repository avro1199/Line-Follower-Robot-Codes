void sonar_reading(){
  if(millis()-s1 > 30){
    distance_l = sonarleft.ping_cm();
    delay(2);
    distance_f = sonarfront.ping_cm();
    s1=millis();
  } 
}

void show_sonar(){
  lcd.clearDisplay();
  distance_l = sonarleft.ping_cm();
  distance_f = sonarfront.ping_cm();
  lcd.setCursor(20, 10);
  lcd.print(distance_l);
  lcd.setCursor(50, 10);
  lcd.print(distance_f);
  lcd.display();
  Serial.print(distance_l);
  Serial.print("  < >  ");
  Serial.print(distance_f);
  Serial.println();
  delay(20);
}

void wall_follow(){
  while(1/*sum == 0*/){
    //reading(); 
    sonar_reading();
    if(distance_f < front_distance && distance_f != 0){
      braking();
      while(distance_f != 0){
        motor(100, -120);
        sonar_reading();
      }
    }
    if((distance_l > mid_distance) && distance_l !=0) motor(60,120);  
    else if((distance_l < mid_distance) && distance_l !=0) motor(120,60);
    else motor(100,100);
    delay(1);
  }
}
