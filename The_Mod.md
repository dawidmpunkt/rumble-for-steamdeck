# The Mod

This mod was and will be revised several times. See the outdated folder for more old files. 
As of now, the rumble signal is captured in software and routed to the rumble motor drivers via I2C.
This way, the rumble signal may be separated from the haptic signal. Also the rumble drivers can be configured/turned on/off without the need to open the Deck.  

### Capturing the rumble signal
software is working. needs to be documented here
The rumble signal is captured by sniffing the USB communication of the Steam Deck Controller (connected internally via USB to the Steam Deck Mainboard).
Sniffing was done with usbmon (https://docs.kernel.org/usb/usbmon.html).
A program (to be documented) was written to monitor the communcation for triggers (rumble commands).
On triggers (rumble commands) a signal is sent to the rumble motor drivers via I2C.

Information on rumble signals via USB is documented in the Steam Controller Singer repository: 
<br /> ttps://gitlab.com/Pilatomic/SteamControllerSinger and https://github.com/Roboron3042/SteamControllerSinger

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

![Alt text](pictures/Rumblestick1.jpg)
<br />Figure of a Rumblestick (Motor integrated into the Joystick cap).

![Alt text](pictures/Rumblestick2.jpg)
<br />Figure of a Rumblestick (Motor integrated into the Joystick cap). The orange wire is connected to the shell of the motor to allow capacitative touch function of the stick (capacitative switch).

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
