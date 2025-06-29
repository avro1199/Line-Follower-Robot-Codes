void line_follow() {
  while (1) {

    reading();

    if (sum == 0) {
      if (turn != 's')
      {
        delay(20);
        (turn == 'l') ? motor(-turn_speed, turn_speed) : motor(turn_speed, -turn_speed);
        while (s[2] == 0 && s[3] == 0) reading();
        turn = 's';
        cross = 's';
      }
      else if (sum == 's')
      {
        // motor(100, 100);
        m1 = millis();
        while (sum == 0) {
          reading();
          if (millis() - m1 > 100) {
            motor(-turn_speed, turn_speed);
            while (s[2] == 0 && s[3] == 0) reading();
            // motor(100, 100);
          }
        }
      }
    }

    else if (sum == 1 || sum == 2)
    {
      if (cross != 's') {
        if (cross == 'r') {
          motor(turn_speed, -turn_speed);
          while (s[1] + s[2] + s[3]) reading();
          while (s[2] == 0 && s[3] == 0) reading();
        }
        else if (cross == 'l') {
          motor(-turn_speed, turn_speed);
          while (s[2] + s[3] + s[4]) reading();
          while (s[2] == 0 && s[3] == 0) reading();
        }
      }

      else if (sensor == 0b001100) motor(10 * spl, 10 * spr);  //straight line

      //left portion
      else if (sensor == 0b001000) motor(9 * spl, 10 * spr);
      else if (sensor == 0b011000) motor(7 * spl, 10 * spr);
      else if (sensor == 0b010000) motor(5 * spl, 10 * spr);
      else if (sensor == 0b110000) motor(3 * spl, 10 * spr);
      else if (sensor == 0b100000) motor(0 * spl, 10 * spr);

      //right portion
      else if (sensor == 0b000100) motor(10 * spl, 9 * spr);
      else if (sensor == 0b000110) motor(10 * spl, 7 * spr);
      else if (sensor == 0b000010) motor(10 * spl, 5 * spr);
      else if (sensor == 0b000011) motor(10 * spl, 3 * spr);
      else if (sensor == 0b000001) motor(10 * spl, 0 * spr);
    }

    else if (sum >= 3 && sum <= 5) {
      if (s[0] == 1 && s[5] == 0 && s[2] + s[3] > 0) {
        turn = 'r';
        cross = 'r';  //right cross for right rule
      }
      if (s[0] == 0 && s[5] == 1 && s[2] + s[3] > 0) {
        turn = 'l';
        cross = 's';  //no left cross for right rule
      }
      m1 = millis();
    }

    else if (sum == 6) {
      turn = 'r';
      cross = 's';
      m2 = millis();
      
      while (s[0] == 1 && s[5] == 1) {
        reading();
        if (millis() - m2 > stop_timer) {
          motor(0, 0);
          while (s[0] == 1 && s[5] == 1) reading();
          break;
        }
      }
      m1 = millis();
    }
    if (millis() - m1 > 200) turn = 's';
  }
}
