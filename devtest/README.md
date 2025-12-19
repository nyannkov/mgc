# Test Game

This is a small game created to evaluate the usability of mgc.
Through the development of this game, we are iteratively improving mgc by adding new features, refining existing ones, and fixing bugs.

![](img/screenshot1.png)
![](img/screenshot2.png)
![](img/image1.jpeg)

## Development Log

|Rev. | Date       | Video | Description |
|---|------------|-------| ----- |
|1 | 2025-12-19 | [#2](https://youtu.be/jxs_fwYP9mA) | Significantly redesigned the game based on Rev.0. Further development continues from this revision. |
|0 | 2025-09-14 | [#1](https://youtu.be/wK57wLaQIZE) | Implemented a simple 2D platformer.　|

## Hardware Used

The game runs on the Waveshare LCD module [Pico-LCD-1.3](https://www.waveshare.com/wiki/Pico-LCD-1.3), driven by a Raspberry Pi Pico microcontroller (RP2040, no Wi-Fi).

Background music (BGM) and sound effects (SE) are played through a PAM8012-based
amplifier module connected to a PWM output pin of the RP2040, with an external speaker.

![](img/front.jpg)
![](img/back.jpg)

## Setup Instructions (in Ubuntu 24.04)

```bash
# Install dependencies
apt update
apt install -y python3 python3-venv git wget

# Set up Python environment
python3 -m venv .venv
source .venv/bin/activate
pip install pyyaml pillow

# Set up Pico SDK (optional script)
wget https://raw.githubusercontent.com/raspberrypi/pico-setup/master/pico_setup.sh
chmod +x pico_setup.sh
./pico_setup.sh

# Clone the repository
git clone https://github.com/nyannkov/mgc.git
cd mgc
git submodule update --init --recursive
python3 scripts/setup_assets.py

# Build
cd devtest/playground/test_game
./build.sh
```
Copy ./build/playground.elf.uf2 to your Raspberry Pi Pico by dragging and dropping it into the USB mass storage device that appears when the Pico is in boot mode.

## Tools Used

- Map editing: Tiled
  Version: 1.11.1-99-gec89c545

- Tileset creation: mtPaint 3.50

## Font used

This project uses the following open-source bitmap fonts provided by external authors:
 - [misaki](https://littlelimit.net/misaki.htm)
 - [k8x12](https://littlelimit.net/k8x12.htm)

## About MML (Music Macro Language)

In this game, both BGM and SE are generated using an external open-source library PSG sound emulator:
 - [emu2149](https://github.com/digital-sound-antiques/emu2149).

All music and sound effects are written in MML (Music Macro Language).
For details on the MML syntax, please refer to the following specification:
 - [MML Specification (Psgino)](https://github.com/nyannkov/Psgino/blob/main/MML.md).
 
 
 

