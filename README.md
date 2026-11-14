# Lab 4: Audio Player (ESP32 + DFPlayer Mini)

A standalone audio player on a standard ESP32. It plays animal sounds from an SD card, shows the current animal, play state, and volume on an OLED, and is controlled with three push buttons. Based on Chapter 5 (Audio Player) of *Developing IoT Projects with ESP32*, adapted from the ESP32-S3-Box-Lite to a normal ESP32 DevKit.

## Features
- Play / pause the selected sound
- Next / previous animal (wraps around the list and plays immediately)
- Volume up / down and mute / unmute
- OLED shows the animal name, Playing/Paused status, volume bar, and the active mode
- Two control modes, switched by double-clicking the middle button:
  - **Playlist mode:** left = previous, middle = play/pause, right = next
  - **Volume mode:** left = volume down, middle = mute/unmute, right = volume up

## Hardware
| Part | Notes |
|---|---|
| ESP32 DevKit (WROOM-32) | Main controller |
| DFPlayer Mini (MP3-TF-16P) | Decodes mp3 files from the SD card |
| microSD card | FAT32, mp3 files in an `mp3` folder |
| Speaker, 8 Ω, 0.5 W | Connected directly to the DFPlayer; keep the volume low |
| 0.96" SSD1306 OLED, I2C | 128x64, address 0x3C |
| 3 tactile push buttons | Left, middle, right |
| 1 kΩ resistor | In series on the ESP32 TX to DFPlayer RX line |
| Breadboard, jumper wires, USB cable | |

## Wiring
| From | To |
|---|---|
| DFPlayer VCC | ESP32 VIN (5 V) |
| DFPlayer GND | ESP32 GND |
| DFPlayer RX | GPIO17 through the 1 kΩ resistor |
| DFPlayer TX | GPIO16 |
| DFPlayer SPK1 / SPK2 | Speaker leads (either order, never to GND) |
| OLED VCC / GND | 3V3 / GND |
| OLED SDA / SCL | GPIO21 / GPIO22 |
| Left button | GPIO32 to GND |
| Middle button | GPIO33 to GND |
| Right button | GPIO13 to GND |

Buttons use the ESP32's internal pull-ups, so no external resistors are needed.

## SD card
- Format: FAT32
- Folder: `/mp3`
- Files: `0001.mp3` (Donkey), `0002.mp3` (Goose), `0003.mp3` (Sheep), `0004.mp3` (Dog)

## Software
- PlatformIO (VS Code), Arduino framework, board `esp32dev`
- Libraries: DFRobotDFPlayerMini, OneButton, Adafruit SSD1306, Adafruit GFX

## How it works
1. The ESP32 talks to the DFPlayer over a serial port (Serial2, 9600 baud) and sends simple commands such as "play track 2".
2. The DFPlayer decodes the mp3 from the SD card and drives the speaker with its built-in amplifier.
3. OneButton turns the button presses into click and double-click events.
4. The program keeps track of the current animal, play state, volume, and mode, and redraws the OLED when any of them change.

## How to use
1. Insert the SD card and power the ESP32 over USB.
2. In playlist mode, press the middle button to play or pause, and the left and right buttons to change animal.
3. Double-click the middle button to switch to volume mode, then use the left and right buttons to change the volume and the middle button to mute.
4. Double-click again to return to playlist mode.

## Tests
- [x] Middle button plays and pauses the sound
- [x] OLED updates when the animal changes
- [x] Volume up, volume down, and mute work
- [x] Double-click switches between playlist and volume mode
- [x] Status returns to "Paused" when the track finishes
- [x] Previous / next wrap around the list

## Differences from the book
- ESP32 DevKit instead of the ESP32-S3-Box-Lite
- 128x64 monochrome OLED instead of the color LCD, so no animal images
- DFPlayer Mini plays the mp3 files, so there is no SPIFFS or `info.json`; animal names are stored in the code
- Mute works by setting the volume to 0 and restoring it

## Authors
- KENT JESSER NERI
- ALWYN REUYAN
- JC REDELOSA

**Section:** BSCA-B186 | **Date:** 11/15/2026
