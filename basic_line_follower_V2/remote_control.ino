void remote_control() {
//   while (digitalRead(calin) == LOW && digitalRead(switchin) == LOW) {
//     if (Serial.available() > 0) {
//       char data =  Serial.read();
//       if (data == 'F')      mos(10 * spr, 10 * spl);
//       else if (data == 'B') motorSpeedB(10 * spr, 10 * spl);
//       else if (data == 'L') motorSpeedL(6 * spr, 6 * spl);
//       else if (data == 'R') motorSpeedR(6 * spr, 6 * spl);
//       else if (data == 'G') mos(10 * spr, 0);
//       else if (data == 'I') mos(0, 10 * spl);
//       else if (data == 'H') motorSpeedB(10 * spr, 0);
//       else if (data == 'J') motorSpeedB(0, 10 * spl);
//       else if (data == 'W') digitalWrite(calout, HIGH);
//       else if (data == 'w') digitalWrite(calout, LOW);
//       else if (data == 'U') digitalWrite(d30, HIGH);
//       else if (data == 'u') digitalWrite(d30, LOW);
//       else if (data == 'S') motorSpeedS();
//       else if (data == '0') grab.write(140);
//       else if (data == '1') grab.write(132);
//       else if (data == '2') grab.write(124);
//       else if (data == '3') grab.write(116);
//       else if (data == '4') grab.write(108);
//       else if (data == '5') grab.write(100);
//       else if (data == '6') grab.write(92);
//       else if (data == '7') grab.write(84);
//       else if (data == '8') grab.write(76);
//       else if (data == '9') grab.write(68);
//       else if (data == 'a') handle.write(100);
//       else if (data == 'b') handle.write(85);
//       else if (data == 'c') handle.write(70);
//       else if (data == 'd') handle.write(60);
//       else if (data == 'e') handle.write(50);
//       else if (data == 'f') handle.write(40);
//       else if (data == 'g') handle.write(30);
//       else if (data == 'h') handle.write(20);
//       else if (data == 'i') handle.write(10);
//       else if (data == 'j') handle.write(0);
//     }
// //    int p = map(analogRead(8), 740, 1023, 0, 100);
// //    if (p <= 20) {
// //      motorSpeedS();
// //      while (digitalRead(switchin) == LOW) text_battery_low();
// //      while (digitalRead(switchin) == HIGH) digitalWrite(calout, HIGH);
// //      digitalWrite(calout, LOW);
// //      break;
// //    }
//   }
}

