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

## Rationale:
Moved [here](RATIONALE.md)

### Aim

This project aims to utilize additional haptic motors to provide stronger rumble sensation while playing games on the steam deck. Instead of just disconnecting the touchpad motors and connecting different motors to the daughter board, this project aims to retain the functionality of the touch pads. Just connecting additional motors in parallel or in series with the existing touchpad motors would limit the output power of both motors. In contrast, hijacking the vibration signal and feeding them into a buffered motor drivers does not interfere with the function of the touch pads and poses not risk in overloading the driver on the daughter board by connecting a smaller load in form of a motor in parallel. 

## The mod

Moved [here](The_Mod.md)

## Software
To keep organization easier, I created a separate repository for for the software.
https://github.com/dawidmpunkt/RumbleDeck

The main component is a sniffer program (uses usbmon/wireshark), that monitors usb communication between SteamOS and the Steam Deck`s controller board. If a rumble signal is registered, the program sends a trigger signal to the RumbleBoard via I2C.
Other components are:
- A python script to initailize the DRV2605 driver(s) and to activate/deactivate the sniffer program. More features are to come.
- A Decky Loader Plugin to comfortably access the software.

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

