#include <Wire.h>
#include "Adafruit_DRV2605.h"

Adafruit_DRV2605 drv;

// pin definitons
const int map_buttonPin = 3;  // the pin that the pushbutton is attached to
const int menu_buttonPin = 4;  // the pin that the second pushbutton is attached to
const int DRV_EN = 1;    // the pin that the DRV2605 is attached to

void setup() {
  drv.begin();

  // initialize the button pin as a input:
  pinMode(map_buttonPin, INPUT);
  pinMode(menu_buttonPin, INPUT);
  // sets the Enable pin as output
  pinMode(DRV_EN, OUTPUT);
  // enable driver for initialization
  digitalWrite(DRV_EN, HIGH);

  drv.setMode(DRV2605_MODE_AUDIOVIBE);
  //drv.setMode(DRV2605_MODE_PWMANALOG);

  // ac coupled input, puts in 0.9V bias
  //drv.writeRegister8(DRV2605_REG_CONTROL1, 0x20); 
  drv.writeRegister8(DRV2605_REG_CONTROL1, 0x3C);  
 
  // analog input
  drv.writeRegister8(DRV2605_REG_CONTROL3, 0xA3);  

  // set minimum input threshold, default = 0x19
  drv.writeRegister8(DRV2605_REG_AUDIOLVL, 0x19);

  // set maximum input headroom, default = 0xFF
  drv.writeRegister8(DRV2605_REG_AUDIOMAX, 0xFF);
  
  //set minimum output level, default = 0x19
  drv.writeRegister8(DRV2605_REG_AUDIOOUTMIN, 0xFF); 

  // Feedback control register
  drv.writeRegister8(DRV2605_REG_FEEDBACK, 0xB6);
  //low pass filter 200 Hz
  drv.writeRegister8(DRV2605_REG_AUDIOCTRL, 0x07);
  //disable driver
  digitalWrite(DRV_EN, HIGH);
}


void loop() {
// read the pushbutton input pins
// if the state of both buttons is high, toggle DRV2605 EN output
if (digitalRead(map_buttonPin) == HIGH && digitalRead(menu_buttonPin) == HIGH) {
 digitalWrite(DRV_EN, !digitalRead(DRV_EN));
  // Delay to avoid bouncing
  delay(750);
}
}
