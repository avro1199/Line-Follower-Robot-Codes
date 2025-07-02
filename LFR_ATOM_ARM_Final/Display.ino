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
  text("arm", 103, 0);
  text("Developed by ", 5, 3);
  oled.setFont(lcd5x7);
  text("\"Team-Void\"", 60, 3);
  oled.setFont(font8x8);
  text("-.._SUST_..-", 15, 4);
  oled.setFont(Stang5x7);
  text(" Path   Menu   Optns ", 1, 7);
  oled.set2X();
}

#ifdef BAR
// A custom function to display the sensor data as bars
void show_as_bars(uint16_t *data, uint8_t size, uint16_t min, uint16_t max, boolean invert)
{
  display.clearDisplay();
  display.setRotation(90);

  for (int i = 0; i < size; i++)
  {
    if (!invert)
    {
      display.fillRect(114 - ((i * 12)), 0, 10, map(constrain(data[i], min, max), min, max, 0, 64), 1);
    }
    else
    {
      display.fillRect(114 - ((i * 12)), 0, 10, map(constrain(data[i], min, max), max, min, 0, 64), 1);
    }
  }
  display.display();
  display.setRotation(0);
}
#endif