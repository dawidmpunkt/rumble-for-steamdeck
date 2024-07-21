# rumble-for-steamdeck

This is a project (work in progress) to modify the rumble feature on Valves Steam Deck.

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

The haptic response in Valve's Steam Deck provides a great effect, while using the trackpads.
While the effect feels quite strong, while fingers are touching the touchpads, it is otherwise barely noticeable.
Game rumble is supported by the Steam Deck's haptic response system, but it seems to be tailored entirely for haptic response of the touch pads. Vibrations produces by game rumble are significantly weaker [see Figure 1 and 2] than by haptic response.

This project aims to utilize additional haptic motors to provide stronger rumble sensation while playing games on the steam deck. The vibration signal for the left and right touchpad is produced by the ARM microcontroller on the daughter board.
In the mod, the signal is hijacked and fed into two DRV2605L Haptic Motor Drivers. A 1 µF capacitor is used to couple the AC-signal from the Steam Deck to the DRV2605L. Each DRV2605L then drives a motor (in this case a LRA - AFT14), mounted on the back-shell of the Steam Deck. The DRV2605L needs to be initialized each time it is powered. An ATTiny is used for this. To be able to toggle the rumble mod on resp. off, the signal of the Steam Deck's map and menu buttons is being hijacked and fed into the ATTiny. When the ATTiny registers, that both menu and map buttons are pressed simultaneously, the EN-pin of on the DRV2605L is being toggled on/off.

## Issues
- Rumble intensity is weaker than the intensity of the haptic feedback.
  While playing Metal Gear Rising Revengence and the Majoras Mask Recompilation, I noticed that the intensity of the game rumble is a lot weaker than haptic feedback. In SteamOS options, there is only the option to adjust the overall intensity of the haptic feedback. adjusting haptic intensity also adjusts the intensity of game rumble. But game rumble is still a lot weaker than haptic feedback.
  This issue was already documented on the SteamOS github: https://github.com/ValveSoftware/SteamOS/issues/1003
  It would be great if game rumble could be adjusted independently of haptic intensity.
- I noticed, that game rumble on the Steam Deck is only produced by the left trackpad.
## ToDo
- [ ] Test different actuators (ALPS AFR14A901B, Vibronics VJP12, INEED MOTOR)
- [ ] Design 3D-printed actuator mount

