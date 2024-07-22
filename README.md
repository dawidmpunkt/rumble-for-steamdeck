# rumble-for-steamdeck

This is a project (work in progress) to modify the rumble feature on Valves Steam Deck.

### Disclaimer:
This project includes modifications of your hardware (Steam Deck, ATtiny, DRV2605, etc.) and soldering of thin wires and small pads. 
Any attempt to carry out the modification described here is at your own risk.
Please be careful as to not damage your hardware and in consequence yourself or others.
Do not attempt this mod, if you are not competent in soldering and working with electronics.
The Steam Deck contains a Lithium-Polymer battery cell which can be very dangerous if handled improbable (e.g. short circuit).
Always unplug your battery and check for shorts before reconnecting the battery.
I don't take any responsibility or liability for any damages to you, your hardware or anything else.

With that said: good luck and happy tinkering.

## Information on the componentes used:

- ATTiny (15 through 85)
  https://www.microchip.com/en-us/product/attiny85

- DRV2605L - Haptic Motor Driver
  https://www.ti.com/product/de-de/DRV2605L
  The DRV2605L is a haptic motor driver that supports both linear resonance actuators (LRA) and eccentric rotating mass motors (ERM). Adafruit provides DRV2605L breakout-boards, that can easily be used for experimenting:
https://learn.adafruit.com/adafruit-drv2605-haptic-controller-breakout/overview

- Alps Haptic Reactor - AFT14 (Linear Resonance Actuator - LRA)
  https://tech.alpsalpine.com/e/products/category/haptic/sub/01/series/aft14/
  (most likely the LRA that is used on JoyCons on the Nintendo Switch) 

- Valve Steam Deck LCD:
  https://www.steamdeck.com

## Rationale:

The haptic response in Valve's Steam Deck provides a great effect while using the trackpads.
While the effect feels quite strong, as fingers are touching the touchpads, it is otherwise barely noticeable.
Game rumble is supported by the Steam Deck's haptic response system, but it seems to be tailored entirely for haptic response of the touch pads. Vibrations produces by game rumble are significantly weaker (see Figure 1 and 2) than by haptic response.

![Alt text](pictures/haptic-feedback_12dB.jpg?raw=true "Oscilloscope reading of the haptic feedback signal")
Figure 1: Oscilloscope reading of the AC haptic feedback signal, which is created by the microcontroller on the daughterboard. Haptic intensity was set to +12 dB in SteamOS

![Alt text](pictures/Metal-Gear-Rising_12dB.jpg?raw=true "Oscilloscope reading of rumble signal while playing Metal Gear Rising ")
Figure 2: Oscilloscope reading of the AC rumble signal, while playing Metal Gear Rising: Revengeance. Haptic intensity was set to +12 dB in SteamOS

## Aim

This project aims to utilize additional haptic motors to provide stronger rumble sensation while playing games on the steam deck. Instead of just disconnecting the touchpad motors and connecting different motors to the daughter board, this project aims to retain the functionality of the touch pads. Just connecting additional motors in parallel or in series with the existing touchpad motors would limit the output power of both motors. In contrast, hijacking the vibration signal and feeding them into a buffered motor drivers does not interfere with the function of the touch pads and poses not risk in overloading the driver on the daughter board by connecting a smaller load in form of a motor in parallel. 

### Hijacking the vibration signal

The vibration signal for the left and right touchpad is produced by the ARM microcontroller on the daughter board (see Fig. 3).

![Alt text](pictures/daughterboard_vibration_traces.jpg?raw=true "location of the traces with the vibration signal")
Figure 3: Location of the traces, which provide the vibration signal for the Left (L) and Right (R) touch-pad

For each (the left and right) channel, the microcontroller produces a PWM signal which is transformed into a 160 Hz AC-signal by passing through a low-pass filter (RC-circuit). The PWM signal consists of a fixed 200 kHz carrier frequency with a duty cycle of 50% onto which the vibration signal is integrated (similar to class-D modulation) (see Fig. 4). 0% duty cycle correspond to peak minimum of the ac signal and 100% duty cycle correspond to peak maximum of the wave.

![Alt text](pictures/vibration-PWM-carrier-wave.jpg?raw=true "PWM carrier frequency and vibraton signal")
Figure 4: 200 kHz carrier frequency and vibration signal

## The mod

In the mod, the signal is hijacked and fed into two DRV2605L Haptic Motor Drivers (see Figure 5). A 1 µF capacitor is used to couple the AC-signal from the Steam Deck to the DRV2605L. Each DRV2605L then drives a motor (in this case a LRA - AFT14), mounted on the back-shell of the Steam Deck. The DRV2605L needs to be initialized each time it is powered. An ATTiny is used for this. To be able to toggle the rumble mod on resp. off, the signal of the Steam Deck's map and menu buttons is being hijacked and fed into the ATTiny. When the ATTiny registers, that both menu and map buttons are pressed simultaneously, the EN-pin of on the DRV2605L is being toggled on/off. 

![Alt text](pictures/DRV2605L_wiring.jpg?raw=true "Schematic")
Figure 5: Schematic diagram of wiring of the devices, that are used in the mod.

### Configuring the ATTiny

To initialize the DRV2605L, a ATTiny85 (cheapest one on Aliexpress) is used. The ATTiny is programmed with ArduinoIDE. The "Adafruit DRV2605 Library" is used for programming.
https://github.com/adafruit/Adafruit_DRV2605_Library

The DRV2605L was configured in Audio-to-Vibe mode in the first successful tests.

## Issues
- Rumble intensity is weaker than the intensity of the haptic feedback.
  While playing Metal Gear Rising Revengence and the Majoras Mask Recompilation, I noticed that the intensity of the game rumble is a lot weaker than haptic feedback. In SteamOS options, there is only the option to adjust the overall intensity of the haptic feedback. adjusting haptic intensity also adjusts the intensity of game rumble. But game rumble is still a lot weaker than haptic feedback.
  This issue was already documented on the SteamOS github: https://github.com/ValveSoftware/SteamOS/issues/1003
  It would be great if game rumble could be adjusted independently of haptic intensity.
- I noticed, that game rumble on the Steam Deck is only produced by the left trackpad.

## ToDo
- [ ] Update schematic (pullup resistors on i2c lines, coupling capacitors, second enable pin, pullup/-down on enable pins)
- [ ] Locate power sources for the circuit (Hook up ATTiny to daughterboard and DRV2605L directly to Battery?)
- [ ] Fix errors on the picture of the daughterboard (position of the red marker for the right signal is not 100% correct: points to ground)
- [ ] Calibrate DRV2605L with the LRA
- [ ] Configure DRV2605L in audio mode
- [ ] Filter out haptic feedback, pass-through for game-rumble signal
- [ ] Test different actuators (ALPS AFR14A901B, Vibronics VJP12, INEED MOTOR)
- [ ] Design 3D-printed actuator mount

