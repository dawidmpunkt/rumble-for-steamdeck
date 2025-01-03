# rumble-for-steamdeck

This is a project (work in progress) to modify the rumble feature on Valves Steam Deck.

### Disclaimer:
This project includes modifications of your hardware (Steam Deck, DRV2605, etc.) and soldering of thin wires and small pads. 
Any attempt to carry out the modification described here is at your own risk.
Please be careful as to not damage your hardware and in consequence yourself or others.
Do not attempt this mod, if you are not competent in soldering and working with electronics.
The Steam Deck contains a Lithium-Polymer battery cell which can be very dangerous if handled improbable (e.g. short circuit).
Always unplug your battery and check for shorts before reconnecting the battery.
I don't take any responsibility or liability for any damages to you, your hardware or anything else.

With that said: good luck and happy tinkering.

## Information on the components used:

- DRV2605L - Haptic Motor Driver
  <br />https://www.ti.com/product/de-de/DRV2605L
  <br />The DRV2605L is a haptic motor driver that supports both linear resonance actuators (LRA) and eccentric rotating mass motors (ERM). Adafruit provides DRV2605L breakout-boards, that can easily be used for experimenting:
<br />https://learn.adafruit.com/adafruit-drv2605-haptic-controller-breakout/overview

- Alps Haptic Reactor - AFT14 (Linear Resonance Actuator - LRA)
  <br />https://tech.alpsalpine.com/e/products/category/haptic/sub/01/series/aft14/
  <br />(most likely the LRA that is used on JoyCons on the Nintendo Switch) 

- Valve Steam Deck LCD (+refreshed Version) and OLED:
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
If this could be solved in software, this would save a lot of work. Unfortunately I am not good in coding and there is almost no documentation available, so I can only solve this (mostly) in hardware.

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

This mod was revised several times. See the outdated folder for more old files. 
The rumble signal is captured in software and routed to the rumble motor drivers via I2C.
This way, the rumble signal may be separated from the haptic signal. Also the rumble drivers can be configured/turned on/off without the need to open the Deck.  

### Capturing the rumble signal
software is working. needs to be documented here
The rumble signal is captured by sniffing the USB communication of the Steam Deck Controller (connected internally via USB to the Steam Deck Mainboard).
Sniffing was done with usbmon (https://docs.kernel.org/usb/usbmon.html).
A program (to be documented) was written to monitor the communcation for triggers (rumble commands).
On triggers (rumble commands) a signal is sent to the rumble motor drivers via I2C.

Information on rumble signals via USB is documented in the Steam Controller Singer repository: 
<br /> ttps://gitlab.com/Pilatomic/SteamControllerSinger and https://github.com/Roboron3042/SteamControllerSinger

### Routing via I2C

On the LCD Steam Deck (non-refreshed version, silver mainboard cover) an I2C interface is accessed by attaching a circuit to the Steam Deck's audio board.
A FPC was designed to be fit in the 8 surface connector. The FPC with the brightness sensor and the microphones can be connected directly on the rumble FPC so all signals are passed through.

The I2C interface is not available at the audio board on the refreshed LCD Steam Deck and the Steam Deck OLED. Instead it is available directly at the mainboard. A FPC will be designed to fit in the 19 pin connector on the mainboard. Hopefully valve will give us an easier option to attach external devices on future devices.

The rumble FPC contains an I2C multiplexer and two DRV2605L motor drivers. The I2C multiplexer is necessary, because the I2C address cannot be changed on the motor drivers
The motor drivers are programmed by selecting one I2C channel after the other. Afterwards both I2C channels are set active. The rumble signal can be sent to both drivers via a common broadcast address.

### Motors/Actuators

![Alt text](pictures/Schematic-V0.4.jpg?raw=true "Schematic")
<br />Figure 7: Schematic diagram of wiring of the devices, that are used in the mod.

![Alt text](pictures/Actuator_mount.jpg?raw=true "Actuator mounts")
<br />Figure 8: 3D printed Actuator Mounts for ERM (left) and LRA (right).

### RumbleSticks
Due to the large mass of the Steam Deck it is difficult to transmit sufficient vibration to the user. On the Steam Deck's touchpads this is solved by partially decoupling the trackpads from the Steam Deck by using "springs".

A similar approach was used with the RumbleSticks. A small coin vibration motor (10 mm resp. 12 mm) was integrated in the head of a joystick cap right under the spot, where the thumg is touching the joystick. By design, the joystick cap is sufficiently decoupled (in the x- and y-axis) from the rest of the device. This way only a small mass needs to be moved in order to transmit vibration. A coin vibration motor was chosen, due to the form factor and the direction of the vibration in the x- and y-axis.
The vibration motor is connected to the Rumble FPC.
A wire is soldered to the shell of the motor and connected to the board of the joystick. This way haptic touch functionality is retained (possibly needs fine-adjustment in the future).

### Configuring the DRV2605L Haptics Driver

To configure the DRV2605L Haptics Driver I used the DRV2604/5 Design Equation document (https://www.ti.com/tool/DRV2604-2605_DESIGN_TOOL), provided on the TI website.
The configuration tool (SLOR114, ZIP File containing two Excel files) can be downloaded here: https://www.ti.com/tool/download/SLOR114

### Where do we take the power from?

Caution: There have been reports of damages to the daughterboards by high current draw:
<br />https://github.com/WUBBSY/RGBDeck
<br />Also, connecting anything directly to the Steam Deck's battery can cause the Deck to refuse to start up or wake from standby (most likely issues with the charging IC due to a different load):
<br />https://github.com/WUBBSY/RGBDeck/issues/1

Power is drawn by connecting a small buck converter to the main power rail. Optional: The buck converter will turn on, when power is applied on an enable-pin.
The rumble drivers can also be toggled on/off via I2C. (see separate project: https://github.com/dawidmpunkt/SteamDeck-I2C-MUX_Buffer)

## Issues


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

