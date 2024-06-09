# samples/cutscene_player

When you start this sample, a short animation will play along with music from the PSG sound source.
The PSG sound source uses an emulator. The emulator's output is produced through PWM.
When connecting to a speaker, please attach appropriate capacitors and resistors as needed.

## Modules in use

| Module name | Description | Notes |
|---|---|---|
| Raspberry Pi Pico | Microcontroller ||
| MSP2807 | LCD module ||

## Pin assignments

For more information on Raspberry Pi Pico pin assignments, please refer to the respective source code below.

 - LCD module:
   [display_ili9341_rp2040.c](https://github.com/nyannkov/mgc/blob/main/devices/mgc/display/ili9341/rp2040/display_ili9341_rp2040.c)

 - PSG Sound:
   [sound_psg_emulator_rp2040.cpp](https://github.com/nyannkov/mgc/blob/main/devices/mgc/sound/psg/emulator/rp2040/sound_psg_emulator_rp2040.cpp)
   
