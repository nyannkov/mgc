# Development environment setup guide
## Ubuntu 22.04 + Raspberry Pi Pico + OpenOCD + J-Link

This guide outlines the steps to set up the development environment. Ubuntu 22.04 was used, running on Windows 10 via VirtualBox 7.0.

(1) Run the following commands
```bash
sudo apt install git
sudo apt install pkg-config
```
(2) Follow the instructions in "Chapter 1. Quick Pico Setup" of [Getting Started with Raspberry Pi Pico] (https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf).

(3) Download the "J-Link Software and Documentation pack" from [this page](https://www.segger.com/downloads/jlink/) and run the following command (replace the installation file with the one you downloaded)
```bash
sudo apt install ./JLink_Linux_V794i_x86_64.deb
```

(4) Create ${PICO_SDK_PATH}/../openocd/tcl/pico-jlink.cfg with the contents from the "Configuration File" section in [this page](https://mickey-happygolucky.hatenablog.com/entry/2021/02/24/193703).

(5) Change to the [hello_pixelbuffer](examples/pixelbuffer/hello_pixelbuffer) directory and execute the following commands
```bash
make connect
make debug
```
If successful, the environment setup is complete.
