#include <Wire.h> // there are issues with Wire.h and ATtiny. See information in the Readme
#include "Adafruit_DRV2605.h"

Adafruit_DRV2605 drv;


// pin definitons
const int map_buttonPin = 3;  // the pin that the pushbutton is attached to
const int menu_buttonPin = 4;  // the pin that the second pushbutton is attached to
const int DRV_EN = 1;    // the pin that the DRV2605 is attached to

void setup() {
  drv.begin();
  Serial.begin(9600); // debugging only 
  // initialize the button pin as a input:
  pinMode(map_buttonPin, INPUT);
  pinMode(menu_buttonPin, INPUT);
  // sets the Enable pin as output
  pinMode(DRV_EN, OUTPUT);
  // enable driver for initialization
  digitalWrite(DRV_EN, HIGH);

  // Control1 Register (Address: 0x1B, default: 0x93). Set STARTUP_BOOST. Set AC-coupled input (0.9V bias), set DRIVE_TIME to 3.125 ms (half period of 160 Hz) according to LRA datasheet
  drv.writeRegister8(DRV2605_REG_CONTROL1, 0xBB);  

  // Control2 Register (Address: 0x1C, default: 0xF5). TODO: unidirectional (closed loop only) or bidirectional mode (default), sampling time, blanking time, current dissipation time. This is beyond my knowledge
  //drv.writeRegister8(DRV2605_REG_CONTROL2, 0xF5);  

  // Rated voltage register (default=0x3E). Set to rated voltage of 3V, see DRV2605 datasheet page 24 Formula 5, resp. datasheet of actuator
  drv.writeRegister8(DRV2605_REG_RATEDV, 0x7F);
  // Overdrive clamp voltage register (default=0x8C). Set to maximum voltage of 3.3V, see DRV2605 datasheet page 24 Formula 7, resp. datasheet of actuator
  drv.writeRegister8(DRV2605_REG_CLAMPV, 0x90);

  // Calibration:
  //Set Calibration Mode
  drv.writeRegister8(DRV2605_REG_MODE, 0x07);
  //Run Calibration
  drv.writeRegister8(0x0C, 0x01);
  // Debug or initial calibration: read register after calibration
  //read Register 0x00 to check if calibration was successful
  byte x00 = drv.readRegister8(0x00);
  Serial.print("Auto-calibration compensation result register - DRV2605_REG_AUTOCALCOMP: ");  Serial.println(x00, BIN);
  //read GO Status
  byte GO = drv.readRegister8(0x0C);
  Serial.print("GO Status: ");  Serial.println(GO, BIN);
  //read Auto-calibration compensation result register 
  byte x0 = drv.readRegister8(DRV2605_REG_AUTOCALCOMP);
  Serial.print("Auto-calibration compensation result register - DRV2605_REG_AUTOCALCOMP: ");  Serial.println(x0, BIN);
  // read Auto-calibration back-EMF result register
  byte x1 = drv.readRegister8(DRV2605_REG_AUTOCALEMP);
  Serial.print("Auto-calibration back-EMF result register - DRV2605_REG_AUTOCALEMP: ");  Serial.println(x0, BIN);
 
  if (GO == 0) {

  // Control3 Register (Address: 0x1D, default: 0xA0). Set closed-loop mode and analog input
  drv.writeRegister8(DRV2605_REG_CONTROL3, 0x82);  
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
  //set Analog Input Mode
  drv.writeRegister8(DRV2605_REG_MODE, 0x03);
}

  //disable driver
  digitalWrite(DRV_EN, LOW);
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
