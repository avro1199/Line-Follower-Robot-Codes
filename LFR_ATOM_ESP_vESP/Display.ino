void text(String t, byte x, byte y)
{
  oled.setCursor(x, y);
  oled.print(t);
}

void home_screen()
{
  oled.set2X();
  oled.clear();
  oled.setFont(Arial_bold_14);
  text("ATOM", 22, 0);
  oled.setFont(Iain5x7);
  oled.set1X();
  text("esp", 103, 0);
  text("Developed by ", 5, 3);
  oled.setFont(lcd5x7);
  text("\"Team-Void\"", 60, 3);
  oled.setFont(font8x8);
  text("-.._SUST_..-", 15, 4);
  oled.setFont(Stang5x7);
  text(" Path   Menu   Optns ", 1, 7);
  oled.set2X();
}