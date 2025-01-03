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

## Information on the components used:

- ATTiny 85
  <br />https://www.microchip.com/en-us/product/attiny85

- DRV2605L - Haptic Motor Driver
  <br />https://www.ti.com/product/de-de/DRV2605L
  <br />The DRV2605L is a haptic motor driver that supports both linear resonance actuators (LRA) and eccentric rotating mass motors (ERM). Adafruit provides DRV2605L breakout-boards, that can easily be used for experimenting:
<br />https://learn.adafruit.com/adafruit-drv2605-haptic-controller-breakout/overview

- Alps Haptic Reactor - AFT14 (Linear Resonance Actuator - LRA)
  <br />https://tech.alpsalpine.com/e/products/category/haptic/sub/01/series/aft14/
  <br />(most likely the LRA that is used on JoyCons on the Nintendo Switch) 

- Valve Steam Deck LCD:
  <br />https://www.steamdeck.com

## Rationale:

The haptic response in Valve's Steam Deck provides a great effect while using the trackpads.
While the effect feels quite strong, as fingers are touching the touchpads, it is otherwise barely noticeable.
Game rumble is supported by the Steam Deck's haptic response system, but it seems to be tailored entirely for haptic response of the touch pads. Vibrations produces by game rumble are significantly weaker (see Figure 1 and 2) than by haptic response.

![Alt text](pictures/haptic-feedback_12dB.jpg?raw=true "Oscilloscope reading of the haptic feedback signal")
<br />Figure 1: Oscilloscope reading of the AC haptic feedback signal, which is created by the microcontroller on the daughterboard. Haptic intensity was set to +12 dB in SteamOS

![Alt text](pictures/Metal-Gear-Rising_12dB.jpg?raw=true "Oscilloscope reading of rumble signal while playing Metal Gear Rising ")
<br />Figure 2: Oscilloscope reading of the AC rumble signal, while playing Metal Gear Rising: Revengeance. Haptic intensity was set to +12 dB in SteamOS

### Aim

This project aims to utilize additional haptic motors to provide stronger rumble sensation while playing games on the steam deck. Instead of just disconnecting the touchpad motors and connecting different motors to the daughter board, this project aims to retain the functionality of the touch pads. Just connecting additional motors in parallel or in series with the existing touchpad motors would limit the output power of both motors. In contrast, hijacking the vibration signal and feeding them into a buffered motor drivers does not interfere with the function of the touch pads and poses not risk in overloading the driver on the daughter board by connecting a smaller load in form of a motor in parallel. 

### Why not increase rumble intensity in software?
<br />Currently, the setting for the intensity of game rumble is directly tied to the intensity of haptics in SteamOS.
By changing the intensity of haptics in the SteamOS settings, you change the intensity of rumble at the same time.
Even if the intensity of haptics is set to maximum, the rumble signal, that is being produced by the microprocessor on the right motherboard, is still waaaay smaller in amplitude, than the haptics signal.
There is an open issue since May 2023 on the SteamOS github page on separating the settings of rumble and haptics.
<br />https://github.com/ValveSoftware/SteamOS/issues/1003
<br />Until now nothing happened.
If this could be solved in software, this would save a lot of work. Unfortunately I am not good in coding, so I can only solve this in hardware.

### Why not simply attatch another LRA directly to the daughterboard

You could disconnect the LRA of the touchpad and attatch another LRA, as it is done here:
<br />https://www.reddit.com/r/SteamDeckModded/comments/14zse0s/steam_deck_vibrationmotor_upgrade/
<br />But then you are left without the haptic feedback on the touchpad(s).

(Audio) amplifiers are tailored to deliver power efficiently at specific loads (impedance of the LRA in this case). If you wire another LRA in parallel you reduce the impedance of the load (by half, if both LRAs have an impedance of 8 Ohms) and could possibly overload the audio amplifier (and possibly damage it) or cause it to malfunction, because it detects a lower load. 
If you connect LRAs in series, you reduce the voltage on the LRA and thus reduce output significantly.

If we want to retain the haptic response of the touchpads, we need another option.

### How is vibration created on the steam deck.

The vibration signal for the left and right touchpad is produced by the ARM microprocessor on the daughter board (see Fig. 3).

![Alt text](pictures/daughterboard_vibration_traces.jpg?raw=true "location of the traces with the vibration signal")
<br />Figure 3: Location of the traces, which provide the vibration signal for the Left (L) and Right (R) touch-pad

For each (the left and right) channel, the microcontroller produces a PWM signal which is transformed into a 160 Hz AC-signal by passing through a low-pass filter (RC-circuit). The PWM signal consists of a fixed 200 kHz carrier frequency with a duty cycle of 50% onto which the vibration signal is integrated (similar to class-D modulation) (see Fig. 4). 0% duty cycle correspond to peak minimum of the ac signal and 100% duty cycle correspond to peak maximum of the wave.

![Alt text](pictures/vibration-PWM-carrier-wave.jpg?raw=true "PWM carrier frequency and vibraton signal")
<br />Figure 4: 200 kHz carrier frequency and vibration signal

Each (left and right) AC-signal then runs through what appears to be an AD51652 Class-D audio amplifier (see Figure 5 and Figure 6) on each of the daughter boards. From the audio amplifier, the positive and negative signal (both biased to 2.5 V) then travels through some beefy inductors to the positive (red) and negative (black) input of the Steam Deck's LRA (which acts like a speaker). 

There was a discussion on the Steam Deck Discord, that this is an incorrect/inefficient way to drive LRAs, as they are meant to be driven by PWM. Possibly, the developers did not put much work into it as this solution was working well enough for haptic response.

![Alt text](pictures/AD51652-signal.jpg?raw=true "Travel path of AC-signal on left daughterboard. AD51652")
<br />Figure 5: Travel path of the AC-signal on the left daughterboard to the LRA via an AD51652 audio amplifier.

![Alt text](pictures/AD51652-close-up.jpg?raw=true "Close-Up of the AD51652")
<br />Figure 6: Close-Up of the AD51652.

## The mod

In the mod, the signal is hijacked and fed into two DRV2605L Haptic Motor Drivers (see Figure 7). A 1 µF capacitor is used to couple the AC-signal from the Steam Deck to the DRV2605L. Each DRV2605L then drives a motor (in this case a LRA - AFT14), mounted on the back-shell of the Steam Deck (see figure 8 for an example). The DRV2605L needs to be initialized each time it is powered. An ATTiny is used for this. To be able to toggle the rumble mod on resp. off, the signal of the Steam Deck's map and menu buttons is being hijacked and fed into the ATTiny. When the ATTiny registers, that both menu and map buttons are pressed simultaneously, the EN-pin of on the DRV2605L is being toggled on/off. 

![Alt text](pictures/Schematic-V0.4.jpg?raw=true "Schematic")
<br />Figure 7: Schematic diagram of wiring of the devices, that are used in the mod.

![Alt text](pictures/Actuator_mount.jpg?raw=true "Actuator mounts")
<br />Figure 8: 3D printed Actuator Mounts for ERM (left) and LRA (right).

### Configuring the ATTiny

To initialize the DRV2605L, a ATTiny85 (cheapest one on Aliexpress) is used. The ATTiny is programmed with ArduinoIDE. The "Adafruit DRV2605 Library" is used for programming.
<br />https://github.com/adafruit/Adafruit_DRV2605_Library

To program the ATtiny I used an Arduino Uno as Programmer (Arduino as ISP).See this tutorial: 
<br />https://www.instructables.com/Program-an-ATtiny-with-Arduino/

If you use Arduino IDE (like me) you can upload sketches to the ATtiny via "Sketch -> Upload Using Programmer". 

There are issues with Wire.h (used by the adafruit library) and ATtiny. See issue discussed here: 
<br />https://forum.arduino.cc/t/attiny85-to-control-haptic-motor-driver-adafruit-drv2605l/587215

You will need to use a different ATtiny core. I used this one: 
<br />https://github.com/SpenceKonde/ATTinyCore

Steps in the tutorials above: 
- Program the Arduino as programmer
- burn bootloader on ATtiny
- upload sketch to ATtiny.

### Configuring the DRV2605L Haptics Driver

To configure the DRV2605L Haptics Driver I used the DRV2604/5 Design Equation document (https://www.ti.com/tool/DRV2604-2605_DESIGN_TOOL), provided on the TI website.
The configuration tool (SLOR114, ZIP File containing two Excel files) can be downloaded here: https://www.ti.com/tool/download/SLOR114

~~The DRV2605L was configured in Audio-to-Vibe mode in the first successful tests.~~
The DRV2605L is configured in Audio mode now.

You can find the code [here](DRV2605_test.ino). 

### Connecting the ATtiny - Buttons

To toggle the LRA driver on/off, we will need to hijack the signal from the map and menu buttons and feed them into the ATtiny.
For the menu button we have to attatch a wire to the FPC connector on the right daughterboard (as seen from the front. it is flipped, since we are looking at it from the back, just look at the board marked "R"). The menu button is wired to the second pin from the left (or from the bottom, see figure 9). Every odd numbered pin is connected to ground. Every even numbered pin is connected to the ARM microprocessor. On the left daughterboard we need to do the same for the map button.. It is still the second button from the left (or from top this time, see figure 10). We also have the option to connect the wire to a testing point in the middle of the daughterboard (See figure 11). I couldn't find a testing point on the right daughterboard.

![Alt text](pictures/right-daughterboard-menu.jpg?raw=true "FPC connector on the right daughterboard")
<br />Figure 9: FPC connector on the right daughterboard (Menu). Connect the wire to the second pin.

![Alt text](pictures/left-daughterboard-map.jpg?raw=true "FPC connector on the left daughterboard")
<br />Figure 10: FPC connector on the left daughterboard (Map). Connect the wire to the second pin.

![Alt text](pictures/left-daughterboard-map2.jpg?raw=true "Testing point for Map button")
<br />Figure 11: It is probably easier to connect the wire (Map) to this testing point.

### Where do we take the power from?

Caution: There have been reports of damages to the daughterboards by high current draw:
<br />https://github.com/WUBBSY/RGBDeck
<br />Also, connecting anything directly to the Steam Deck's battery can cause the Deck to refuse to start up or wake from standby:
<br />https://github.com/WUBBSY/RGBDeck/issues/1

~~I plan to draw the power the ATtiny vial the right daughterboard (as seen from the front. it is flipped, since we are looking at it from the back), since it draws very little power. The risk for damaging the daughterboard by high current draw is minimal this way. Also, this way it will only be powered, if the steam deck is on and not drawing any power if the steam deck is sleeping/off.
Powering the DRV2605 is a little more tricky, since it draws more power. I am still working on this. I will try to use a step-down converter, hooked up to the Steam Deck's battery that will be triggered on, when there is power on the daughterboard.~~
Power will be drawn ~~directly from the battery~~ (not safe for a large load. Can cause issues with the charging IC. can work for just the DRV2605 and a small LRA) from the main power rail (after I find the main power rail after the load switch) by connecting a small switching-mode power supply (SMPS) to the battery. The SMPS will turn on, when power is applied via USB (via an enable-pin).
The device can also be toggled on/off via I2C. (see separate project: https://github.com/dawidmpunkt/SteamDeck-I2C-MUX_Buffer)

## Issues
- Rumble intensity is weaker than the intensity of the haptic feedback.
  While playing Metal Gear Rising Revengence and the Majoras Mask Recompilation, I noticed that the intensity of the game rumble is a lot weaker than haptic feedback. In SteamOS options, there is only the option to adjust the overall intensity of the haptic feedback. adjusting haptic intensity also adjusts the intensity of game rumble. But game rumble is still a lot weaker than haptic feedback.
  This issue was already documented on the SteamOS github: https://github.com/ValveSoftware/SteamOS/issues/1003
  It would be great if game rumble could be adjusted independently of haptic intensity.
- I noticed, that game rumble on the Steam Deck is only produced by the left trackpad.

## ToDo
- [x] Update schematic (pullup/-down on enable pins)
- [x] Locate power sources for the circuit (Hook up ATTiny to daughterboard and DRV2605L directly to Battery?) -> completely hooked up to battery via separate LDO
- [ ] Fix errors on the picture of the daughterboard (position of the red marker for the right signal is not 100% correct: points to ground)
- [x] Calibrate DRV2605L with the LRA
- [x] Configure DRV2605L in audio mode
- [ ] Work out issues with calibration of AFT14A903A LRA
- [ ] Filter out haptic feedback, pass-through for game-rumble signal
- [x] Test different actuators (ALPS AFR14A901B, Vibronics VJP12, INEED MOTOR)
- [x] Design 3D-printed actuator mount

