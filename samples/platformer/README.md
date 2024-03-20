# samples/platformer

## Modules used

| Module name | Description |
|-----------------------------------|-----------------------------------|
| Raspberry Pi Pico | Microcontroller |
MSP2807 | LCD module | YMZ294 | Sound source IC(SSG)
| YMZ294 | Sound Source IC (SSG)
| Mini 2-Axis Analog Thumbstick | Adafruit, Analog Joystick |

## Pin assignments

For more information on Raspberry Pi Pico pin assignments, please refer to the respective source code below.

 - LCD module:.
   [display_ili9341_rp2040.c](https://github.com/nyannkov/mgc/blob/main/devices/mgc/display/ili9341/rp2040/display_ili9341_rp2040.c)

 - Gamepad:.
   [gamepad_rp2040.c](https://github.com/nyannkov/mgc/blob/main/devices/mgc/gamepad/rp2040/gamepad_rp2040.c)
   
   Tip: Tact switches are used for KEY0 and KEY1 on the gamepad, with a pull-up on the Raspberry Pi Pico side so that the input pin is connected to GND when the switch is ON.

 - Sound source IC:.
   [sound_ymz294_rp2040.cpp](https://github.com/nyannkov/mgc/blob/main/devices/mgc/sound/psg/ymz294/rp2040/sound_ymz294_rp2040.cpp)
   
   Tip: If the sound source is not required, it can be omitted. Note: The YMZ294 is a 5V IC; it can be controlled by directly connecting the output pins of the Raspberry Pi Pico to the input pins of the YMZ294, but this may damage the Raspberry Pi Pico. Therefore, please consider using a suitable voltage level shifter if necessary.

## Notes.

This sample is incomplete. Features will be added, modified, and removed from the library itself as more content is added to the sample in the future.
