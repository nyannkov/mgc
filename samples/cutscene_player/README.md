# samples/platformer

## Modules in use

| Module name | Description | Notes |
|---|---|---|
| Raspberry Pi Pico | Microcontroller ||
| MSP2807 | LCD module ||

## Pin assignments

For more information on Raspberry Pi Pico pin assignments, please refer to the respective source code below.

 - LCD module:
   [display_ili9341_rp2040.c](https://github.com/nyannkov/mgc/blob/main/devices/mgc/display/ili9341/rp2040/display_ili9341_rp2040.c)

 - Gamepad:
   [gamepad_rp2040.c](https://github.com/nyannkov/mgc/blob/main/devices/mgc/gamepad/rp2040/gamepad_rp2040.c)
   
   Tip: Tact switches are used for KEY0 and KEY1 on the gamepad, with a pull-up on the Raspberry Pi Pico side so that the input pin is connected to GND when the switch is ON.

 - PSG Sound:
   [sound_psg_emulator_rp2040.cpp](https://github.com/nyannkov/mgc/blob/main/devices/mgc/sound/psg/emulator/rp2040/sound_psg_emulator_rp2040.cpp)
   
## Notes

This sample is incomplete. Features will be added, modified, and removed from the library itself as more content is added to the sample in the future.
