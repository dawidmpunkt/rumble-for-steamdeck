// Below is a modified version of the Adafruit_DRV2605_Library example file: https://github.com/adafruit/Adafruit_DRV2605_Library/blob/master/examples/audio/audio.ino

#include <Wire.h>
#include "Adafruit_DRV2605.h"

Adafruit_DRV2605 drv;

void setup() {
  Serial.begin(9600);
  Serial.println("DRV2605 Audio responsive test");
  drv.begin();


  drv.setMode(DRV2605_MODE_AUDIOVIBE); // Put the driver in Audio-to-Vibe mode
  //drv.setMode(DRV2605_MODE_PWMANALOG); // PWM or Analog mode

  // ac coupled input, puts in 0.9V bias
  //drv.writeRegister8(DRV2605_REG_CONTROL1, 0x20); //configuration from the example file
  drv.writeRegister8(DRV2605_REG_CONTROL1, 0x3C);  
 
  // analog input
  drv.writeRegister8(DRV2605_REG_CONTROL3, 0xA3);  

  // set minimum input threshold, default = 0x19
  drv.writeRegister8(DRV2605_REG_AUDIOLVL, 0x19);

  // set maximum input headroom, default = 0xFF
  drv.writeRegister8(DRV2605_REG_AUDIOMAX, 0xFF);
  
  //set minimum output level, default = 0x19
  drv.writeRegister8(DRV2605_REG_AUDIOOUTMIN, 0xFF); // maximum output

  // Feedback control register
  drv.writeRegister8(DRV2605_REG_FEEDBACK, 0xB6);
  //low pass filter 200 Hz
  drv.writeRegister8(DRV2605_REG_AUDIOCTRL, 0x07);
}


void loop() {
}

