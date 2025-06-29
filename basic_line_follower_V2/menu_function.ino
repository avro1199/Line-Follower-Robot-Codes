int menu_function1() {
  int list = 5;
  int p = 0;
  int temp = 0;
  if (digitalRead(calin) == HIGH) {
    delay(100);
    while (digitalRead(calin) == HIGH) {
      delay(1);
      digitalWrite(calout, HIGH);
    }
    digitalWrite(calout, LOW);
    while (digitalRead(calin) == LOW) {
      delay(1);
      p = map(analogRead(9), 20, 1023, 1, list);
      if (temp != p) {
        temp = p;
        if (temp == 1) text_line_follow();
        else if (temp == 2) {
          display.clearDisplay();
          text("CALLIBRATE", 5, 24);
          display.display();
        }
        else if (temp == 3) {
          display.clearDisplay();
          text("ANALOG", 29, 16);
          text("READ", 41, 42);
          display.display();
        }
        else if (temp == 4) {
          display.clearDisplay();
          text("DIGITAL", 23, 16);
          text("READ", 41, 42);
          display.display();
        }
        else if (temp == 5) {
          display.clearDisplay();
          text("SONAR READ", 5, 24);
          display.display();
        }
      }
      if (digitalRead(switchin) == HIGH) {
        while (digitalRead(switchin) == HIGH) {
          delay(1);
          digitalWrite(calout, HIGH);
        }
        digitalWrite(calout, LOW);
        sust_cracker_nut();
        return 0;
      }
    }

    while (digitalRead(calin) == HIGH) {
      delay(1);
      digitalWrite(calout, HIGH);
    }
    digitalWrite(calout, LOW);
  }
  return temp;
}


int menu_function2() {
  int list = 5;
  int p = 0;
  int temp = 0;
  if (digitalRead(switchin) == HIGH) {
    while (digitalRead(switchin) == HIGH) {
      delay(1);
      digitalWrite(calout, HIGH);
    }
    digitalWrite(calout, LOW);
    while (digitalRead(switchin) == LOW) {
      delay(1);
      p = map(analogRead(9), 20, 1023, 1, list);
      if (temp != p) {
        temp = p;
        if (temp == 1) {
          display.clearDisplay();
          text("COUNTER", 21, 16);
          text("SETUP", 35, 42);
          display.display();
        }
        else if (temp == 2) {
          display.clearDisplay();
          text("BATTERY", 23, 16);
          text("CHECK", 35, 42);
          display.display();
        }
        else if (temp == 3) text_speed_adjust();
        else if (temp == 4) {
          display.clearDisplay();
          text("BRAKING", 23, 16);
          text("MECHANISM", 11, 42);
          display.display();
        }
        else if (temp == 5) {
          display.clearDisplay();
          text("BLUETOOTH", 11, 16);
          text("CONTROL", 23, 42);
          display.display();
        }
      }
      if (digitalRead(calin) == HIGH) {
        while (digitalRead(calin) == HIGH) {
          delay(1);
          digitalWrite(calout, HIGH);
        }
        digitalWrite(calout, LOW);
        sust_cracker_nut();
        return 0;
      }
    }

    while (digitalRead(switchin) == HIGH) {
      delay(1);
      digitalWrite(calout, HIGH);
    }
    digitalWrite(calout, LOW);
  }
  return temp;
}

