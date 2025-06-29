# Memo

Using the Waveshare LCD module [Pico-LCD-1.3](https://www.waveshare.com/wiki/Pico-LCD-1.3).  
The microcontroller is a Raspberry Pi Pico (RP2040, no WiFi).

![](img/front.jpg)
![](img/back.jpg)

---

To play background music (BGM) and sound effects (SE), an amplifier module based on the PAM8012 was connected to the PWM output pin of the RP2040, along with a speaker.

For the syntax of the MML (Music Macro Language) used for BGM and SE, please refer to:  
👉 [MML Specification (Psgino)](https://github.com/nyannkov/Psgino/blob/main/MML.md)
