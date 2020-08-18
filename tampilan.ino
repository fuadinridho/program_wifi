void awalmulai()
{
  
}

void tampil()
{
  display.clearDisplay();
display.display();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0, 0);

display.println("Weather station");
display.print("Suhu :");
display.print(suhu);
display.print(" ");
display.print((char)247);
display.print("C");

display.println();
display.print("Kelembaban: ");
display.print(kelembaban);
display.print(" %");
display.println();

display.display();
display.startscrollright(0x00, 0x00);
  delay(4000);

display.clearDisplay();
display.display();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0, 0);

display.println("Weather station");
display.print("Cahaya :");
display.print(lux);
display.print(" ");
display.print("lx");

display.println();
display.print("Tekanan: ");
display.print(pressure);
display.print(" atm");
display.println();

display.display();
display.startscrollright(0x00, 0x00);
  delay(4000);
}
