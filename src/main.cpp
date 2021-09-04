#include <Arduino.h>
#include <Wire.h>
#include <LaserWar.h>
#include <LiquidCrystal.h>
#include <LiquidMenu.h>
#define IR_PIN 3
#define KILL_CMD 0x8300e8
#define BLUE_CMD 0xa901e8
#define RED_CMD 0xa900e8
#define DOUBLEHEALTH_CMD 0x8322e8
#define REDSHOOT_CMD 0x1fc1e8
#define BLUESHOOT_CMD 0x1fd1e8
#define RESPAWN_CMD 0x8305e8
#define AMMO_CMD 0x830fe8
const byte LCD_RS = 8;
const byte LCD_E = 9;
const byte LCD_D4 = 4;
const byte LCD_D5 = 5;
const byte LCD_D6 = 6;
const byte LCD_D7 = 7;

#define SELECT 1
#define UP 2
#define DOWN 3

LaserWar lw;

LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

LiquidLine line1_1(1, 0, "Respawn");
LiquidLine line1_2(2, 1, "Kill");
LiquidScreen screen1(line1_1, line1_2);
LiquidLine line2_1(1, 0, "Color red");
LiquidLine line2_2(2, 1, "Color blue");
LiquidScreen screen2(line2_1, line2_2);
LiquidLine line3_1(1, 0, "Red shoot");
LiquidLine line3_2(2, 1, "Blue shoot");
LiquidScreen screen3(line3_1, line3_2);
LiquidLine line4_1(1, 0, "Double healht");
LiquidLine line4_2(2, 1, "Ammo");
LiquidScreen screen4(line4_1, line4_2);

LiquidMenu menu(lcd, screen1, screen2, screen3, screen4);

void setup()
{
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  Serial.begin(9600);
}

int detectButton()
{
  int button = analogRead(0);
  if (700 < button && button < 800)
  {
    return SELECT;
  }
  if (100 < button && button < 200)
  {
    return UP;
  }
  if (300 < button && button < 400)
  {
    return DOWN;
  }
  return 0;
}
void loop()
{
  // put your main code here, to run repeatedly:
  switch (detectButton())
  {
  case DOWN:
    menu.next_screen();
    delay(500);
    break;
  case UP:
    menu.previous_screen();
    delay(500);
    break;
  default:
    break;
  }
}

void kill()
{
  lw.send(IR_PIN, KILL_CMD);
}

void colorRed()
{
  lw.send(IR_PIN, RED_CMD);
}

void colorBlue()
{
  lw.send(IR_PIN, BLUE_CMD);
}

void shootRed()
{
  lw.send(IR_PIN, REDSHOOT_CMD);
}

void shootBlue()
{
  lw.send(IR_PIN, BLUESHOOT_CMD);
}

void respawn()
{
  lw.send(IR_PIN, RESPAWN_CMD);
}

void doubleHealth()
{
  lw.send(IR_PIN, DOUBLEHEALTH_CMD);
}

void ammo()
{
  lw.send(IR_PIN, AMMO_CMD);
}

//722 celect
//131 up
//308 down