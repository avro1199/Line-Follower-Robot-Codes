//void sonar_reading(){
//  if(millis()-s1 > 30){
//    distance = sonarfront.ping_cm();
//    s1=millis();
//  } 
//}
//
//void show_sonar(){
//  distance = sonarfront.ping_cm();
//  Serial.println(distance);
//}

//void wall_follow(){
//  while(sum == 0){
//    reading(); 
//    distance = sonar.ping_cm();
//    if(distance - mid_distance > 2 && distance !=0) motor(120,0);  
//    if(mid_distance - distance > 2 && distance !=0) motor(0,120);
//    else motor(120,120); 
//  }
//}
