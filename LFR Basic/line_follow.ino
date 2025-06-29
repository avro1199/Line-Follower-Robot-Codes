void line_follow() {
  while (1) {
    read_sensor();

    //out of line
    if (sum == 0) {
      if (flag) {
        if (flag == 1) {
          motor(-tsp, tsp);
        } else if (flag == 2) {
          motor(tsp, -tsp);
        }
        while ((!s[2] && !s[3])) {
          read_sensor();
        }
        flag = 0;
        cross = 0;
      }
    }
    //normal line
    else if (sum == 1 || sum == 2) {
      if (sensor == 0b001100) {
        motor(10*sp, 10*sp);
      }
      //left part
      else if (sensor == 0b001000) {
        motor(9*sp, 10*sp);
      } else if (sensor == 0b011000) {
        motor(7*sp, 10*sp);
      } else if (sensor == 0b010000) {
        motor(5*sp, 10*sp);
      } else if (sensor == 0b110000) {
        motor(2*sp, 10*sp);
      } else if (sensor == 0b100000) {
        motor(-3*sp, 10*sp);
      }
      //right part
      else if (sensor == 0b000100) {
        motor(10*sp, 9*sp);
      } else if (sensor == 0b000110) {
        motor(10*sp, 7*sp);
      } else if (sensor == 0b000010) {
        motor(10*sp, 5*sp);
        Serial.println("r_l_1");
      } else if (sensor == 0b000011) {
        motor(10*sp, 2*sp);
        Serial.println("r_l_2");
      } else if (sensor == 0b000001) {
        motor(10*sp, -3*sp);
        Serial.println("r_l_3");
      }
    }
    //turn
    else if (sum > 2 && sum < 6) {
      if (s[0] && (s[2] || s[3]) && !s[5]) {
        flag = 1;
      } else if (!s[0] && (s[2] || s[3]) && s[5]) {
        flag = 2;
      }

    }
    //all sensor
    else if (sum == 6) {
      if(s[0])
      {
        flag = 1;
      }
      else if(s[5])
      {
        flag = 2;
      }
      // motor(0, 0);
      // while (1)
      //   ;
    }
  }
}