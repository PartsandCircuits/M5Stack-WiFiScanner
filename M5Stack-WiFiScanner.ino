// Credit to Dimi for the code  -  see:   http://forum.m5stack.com/topic/58/lesson-3-wi-fi-scanner
// 

#include <M5Stack.h>
#include "WiFi.h"
#include <String.h>

int n;
int ssidLength = 10;
int thisPage = 0;
const int pageSize = 8;
bool on = false;
bool leftLocked = false;
bool rightLocked = false;

void setup()
{
  M5.begin();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  M5.Lcd.setBrightness(100);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(45, 110);
  M5.Lcd.printf("Wi-Fi scanner");
  DrawMenu();
}

void LCD_Clear() {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);
}

void DrawMenu(){
  M5.Lcd.setTextSize(3);
  M5.Lcd.setTextColor(BLUE);
  
  if (on == true)
  {
    M5.Lcd.setCursor(110, 215);
    M5.Lcd.printf("RESCAN");
    if (thisPage != 0)
    {
      M5.Lcd.setCursor(60, 215);
      M5.Lcd.printf("<");
      leftLocked = false;
    }
    else
    {
      leftLocked = true;
    }
    if (thisPage < ((n - 1) / pageSize))
    {
      M5.Lcd.setCursor(250, 215);
      M5.Lcd.printf(">");
      rightLocked = false;
    }
    else
    {
      rightLocked = true;
    }
  }
  else
  {
    M5.Lcd.setCursor(128, 215);
    M5.Lcd.printf("SCAN");
  }
}

void Show(int nav = 0) // -1 top, 1 bottom
{
  if (nav == -1)
  {
    if ((on == true) && (leftLocked == false))
    {
      thisPage--;
      if (thisPage < 0) thisPage = 0;
      Show();
    }
  }
  else if (nav == 1)
  {
    if ((on == true) && (rightLocked == false))
    {
      if ((thisPage) <= (n / pageSize)) thisPage++;
      Show();
    }
  }
  else
  {
    LCD_Clear();
    M5.Lcd.setCursor(100, 2);
    M5.Lcd.print("TOTAL: ");
    M5.Lcd.print(n);
    M5.Lcd.setCursor(0, 30);
    for (int i = (thisPage * pageSize); i < ((thisPage * pageSize) + pageSize); i++)
    {
      if (i >= n) break;
      M5.Lcd.print(i + 1);
      String ssid = (WiFi.SSID(i).length() > ssidLength) ? (WiFi.SSID(i).substring(0, ssidLength) + "...") : WiFi.SSID(i);
      M5.Lcd.print(") " + ssid + " (" + WiFi.RSSI(i) + ");\n");
    }
    DrawMenu();
  }
}

void Search() {
  on = true;
  LCD_Clear();
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(50, 90);
  M5.Lcd.printf("Please, wait.");
  M5.Lcd.setCursor(50, 120);
  M5.Lcd.printf("Searching...");
  n = WiFi.scanNetworks();
  Show();
}

void loop()
{
  if (M5.BtnA.wasPressed()) Show(-1);
  if (M5.BtnB.wasPressed()) Search();
  if (M5.BtnC.wasPressed()) Show(1);
  M5.update();
}
