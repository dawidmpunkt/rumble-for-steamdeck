# Rationale

The haptic response in Valve's Steam Deck provides a great effect while using the trackpads.
While the effect feels quite strong, as fingers are touching the touchpads, it is otherwise barely noticeable.
Game rumble is supported by the Steam Deck's haptic response system, but it seems to be tailored entirely for haptic response of the touch pads. Vibrations produces by game rumble are significantly weaker (see Figure 1 and 2) than by haptic response.

![Alt text](pictures/haptic-feedback_12dB.jpg?raw=true "Oscilloscope reading of the haptic feedback signal")
<br />Figure 1: Oscilloscope reading of the AC haptic feedback signal, which is created by the microcontroller on the daughterboard. Haptic intensity was set to +12 dB in SteamOS

![Alt text](pictures/Metal-Gear-Rising_12dB.jpg?raw=true "Oscilloscope reading of rumble signal while playing Metal Gear Rising ")
<br />Figure 2: Oscilloscope reading of the AC rumble signal, while playing Metal Gear Rising: Revengeance. Haptic intensity was set to +12 dB in SteamOS

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
