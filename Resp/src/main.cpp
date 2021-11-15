#include <Arduino.h>
#include <LedControl.h>
#include <LaserWar.h>

/**
 * Analog input 2 is Pin 4 for the DigiSpark
 * All buttons should be connected to the single pin with resistors
 * 
 * 10K resistor between pin 4 and GND
 * 100R resistor between increase btn and 5V
 * 220R resistor between decrease btn and 5V
 */ 
#define BTN_PIN 2

#define IR_PIN 3

/**
 * LedControl sets pinMode for these pins
 */ 
#define DIN_PIN 0
#define CLK_PIN 2
#define LOAD_PIN 1

#define INCREASE_BTN_VALUE 524
#define DECREASE_BTN_VALUE 702
#define RESP_BTN_VALUE 1022
#define DELTA 100

#define DOUBLE_HEALTH_CMD 0x8322e8 // Double health once per live
#define DOUBLE_HEALTH2_CMD 0x8310e8 // Double health each time button pressed
#define RESP_CMD 0x8305e8 // Start the game command, simple respawn
#define AMMO_CMD 0x8306e8

#define NUM_DISPLAYS 1
#define MAX_STEPS_ON_DISPLAY 23 
#define STEP 5

unsigned long currentStep = 20;

LaserWar lw;

LedControl lc=LedControl(DIN_PIN, CLK_PIN, LOAD_PIN, NUM_DISPLAYS);
unsigned long delaytime = 500;

void showHealth(int health){
  lc.clearDisplay(0);

  lc.setDigit(0, 3, health % 10, false);
  lc.setDigit(0, 2, int(health / 10) % 10, false);
  lc.setDigit(0, 1, int(health / 100) % 10, false);
  lc.setDigit(0, 0, int(health / 1000) % 10, false);
}

void writeDblN(int n){
  lc.clearDisplay(0);
  lc.setChar(0, 0, 'd', false);
  lc.setChar(0, 1, 'b', false);
  lc.setChar(0, 2, 'L', false);
  lc.setDigit(0, 3, n, false);
}

void writeAmmo(){
  lc.clearDisplay(0);
  lc.setChar(0, 0, 'A', false);
  lc.setChar(0, 1, 'A', false);
  lc.setChar(0, 2, 'A', false);
  lc.setChar(0, 3, 'A', false);
}

void setup(){
  pinMode(IR_PIN, OUTPUT);
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  showHealth(currentStep * STEP);
}

unsigned long getRespCmd(){
  return 0xa100e8 + 0x0500 * currentStep;
}

void respawn(){
  if (currentStep == MAX_STEPS_ON_DISPLAY - 3) {
    lw.send(IR_PIN, RESP_CMD);
  } else if (currentStep == MAX_STEPS_ON_DISPLAY - 2){
    lw.send(IR_PIN, DOUBLE_HEALTH_CMD);
  } else if (currentStep == MAX_STEPS_ON_DISPLAY - 1) {
    lw.send(IR_PIN, DOUBLE_HEALTH2_CMD);
  } else if (currentStep == MAX_STEPS_ON_DISPLAY) {
    lw.send(IR_PIN, AMMO_CMD);
  } else {
    lw.send(IR_PIN, getRespCmd());
  }
  delay(500);
}

void changeHp(int direction){
  if (currentStep == 1 && direction < 0) {
    currentStep = MAX_STEPS_ON_DISPLAY;
  } else if (currentStep == MAX_STEPS_ON_DISPLAY && direction > 0){
    currentStep = 1;
  } else {
    currentStep += direction;
  }

  if (currentStep == MAX_STEPS_ON_DISPLAY){
    writeAmmo();
  } else if (currentStep == MAX_STEPS_ON_DISPLAY - 1){
    writeDblN(2);
  } else if (currentStep == MAX_STEPS_ON_DISPLAY - 2) {
    writeDblN(1);
  } else {
    showHealth(currentStep * STEP);
  }

  delay(300);
}

void loop(){
  int buttonValue = analogRead(BTN_PIN);
  if (buttonValue < RESP_BTN_VALUE + DELTA && buttonValue > RESP_BTN_VALUE - DELTA) {
    respawn();
  } else if (buttonValue < INCREASE_BTN_VALUE + DELTA && buttonValue > INCREASE_BTN_VALUE - DELTA){
    changeHp(1);
  } else if (buttonValue < DECREASE_BTN_VALUE + DELTA && buttonValue > DECREASE_BTN_VALUE - DELTA) {
    changeHp(-1);
  }
}