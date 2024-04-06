# samples/platformer

## Modules in use

| Module name | Description | Notes |
|---|---|---|
| Raspberry Pi Pico | Microcontroller ||
| MSP2807 | LCD module ||
| YMZ294 | Sound Source IC (SSG) | Since the PSG emulator can be integrated into the project, it is possible to output PSG audio without using YMZ294. |
| Mini 2-Axis Analog Thumbstick | Adafruit, Analog Joystick ||

## Pin assignments

For more information on Raspberry Pi Pico pin assignments, please refer to the respective source code below.

 - LCD module:
   [display_ili9341_rp2040.c](https://github.com/nyannkov/mgc/blob/main/devices/mgc/display/ili9341/rp2040/display_ili9341_rp2040.c)

 - Gamepad:
   [gamepad_rp2040.c](https://github.com/nyannkov/mgc/blob/main/devices/mgc/gamepad/rp2040/gamepad_rp2040.c)
   
   Tip: Tact switches are used for KEY0 and KEY1 on the gamepad, with a pull-up on the Raspberry Pi Pico side so that the input pin is connected to GND when the switch is ON.

 - Sound source IC:
   [sound_ymz294_rp2040.cpp](https://github.com/nyannkov/mgc/blob/main/devices/mgc/sound/psg/ymz294/rp2040/sound_ymz294_rp2040.cpp)
   
   **Note (1):** The YMZ294 is a 5V IC; it can be controlled by directly connecting the output pins of the Raspberry Pi Pico to the input pins of the YMZ294, but this may damage the Raspberry Pi Pico. Therefore, please consider using a suitable voltage level shifter if necessary.

   **Note (2):** Due to the difficulty in obtaining the YMZ294, we have implemented emulation of PSG output using emu2149 (https://github.com/digital-sound-antiques/emu2149.git). 
In the case of emulation, there is an increase in processing load, but there are advantages such as reducing the GPIO to just one PWM output pin and resolving the 3.3V-5V compatibility issue mentioned in NOTE (1). Therefore, if there are no particular preferences, it may be better to use this emulator.
When using the emulator, please change the value of MGC_SOUND_PSG_TYPE from ymz294 to emulator in the CMakeLists.txt file located in the same directory as main.cpp as shown below:
   ```cmake
    # sound driver
    #set(MGC_SOUND_PSG_TYPE ymz294)
    set(MGC_SOUND_PSG_TYPE emulator)
   ```
   The source code of this emulator is [here](https://github.com/nyannkov/mgc/tree/main/devices/mgc/sound/psg/emulator).
   
## Notes

This sample is incomplete. Features will be added, modified, and removed from the library itself as more content is added to the sample in the future.
