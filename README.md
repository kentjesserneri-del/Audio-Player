# Lab 4: Audio Player (ESP32 + DFPlayer Mini)

A standalone audio player running on a standard ESP32 and built with the **ESP-IDF framework**. It plays songs from an SD card, shows the current song, play state, and volume on an OLED display, and is controlled with three push buttons.

Based on Chapter 5 (*Audio Player*) of *Developing IoT Projects with ESP32*, adapted from the ESP32-S3-Box-Lite to a standard ESP32 DevKit.

![ESP-IDF](https://img.shields.io/badge/framework-ESP--IDF-red)
![Platform](https://img.shields.io/badge/platform-ESP32-blue)
![Audio](https://img.shields.io/badge/audio-DFPlayer%20Mini-orange)

---

## Table of Contents

1. [Features](#features)
2. [Hardware Requirements](#hardware-requirements)
3. [Wiring](#wiring)
4. [SD Card Preparation](#sd-card-preparation)
5. [Software Requirements](#software-requirements)
6. [Project Structure](#project-structure)
7. [How It Works](#how-it-works)
8. [Build & Flash](#build--flash)
9. [Usage](#usage)
10. [Test Checklist](#test-checklist)
11. [Differences from the Book](#differences-from-the-book)
12. [Authors](#authors)

---

## Features

- Play / pause the selected song
- Next / previous song (wraps around the playlist and plays immediately)
- Volume up / down and mute / unmute
- OLED shows the song title, **Playing / Paused** status, volume bar, and the active mode
- Two control modes, switched by **double-clicking the middle button**

| Mode | Left button | Middle button | Right button |
|------|-------------|---------------|--------------|
| **Playlist** | Previous | Play / Pause | Next |
| **Volume** | Volume down | Mute / Unmute | Volume up |

## Hardware Requirements

| Qty | Component | Notes |
|-----|-----------|-------|
| 1 | ESP32 DevKit (WROOM-32) | Main controller |
| 1 | DFPlayer Mini (MP3-TF-16P) | Decodes MP3 files from the SD card |
| 1 | microSD card | FAT32, MP3 files in an `mp3` folder |
| 1 | Speaker, 8 Ω, 0.5 W | Connected directly to the DFPlayer; keep the volume low |
| 1 | 0.96" SSD1306 OLED, I2C | 128x64, address `0x3C` |
| 3 | Tactile push buttons | Left, middle, right |
| 1 | 1 kΩ resistor | In series on the ESP32 TX → DFPlayer RX line |
| - | Breadboard, jumper wires, USB cable | |

## Wiring

### DFPlayer Mini

| From | To |
|------|----|
| DFPlayer VCC | ESP32 VIN (5 V) |
| DFPlayer GND | ESP32 GND |
| DFPlayer RX | GPIO 17 through the 1 kΩ resistor |
| DFPlayer TX | GPIO 16 |
| DFPlayer SPK1 / SPK2 | Speaker leads (either order, **never to GND**) |

### OLED Display

| From | To |
|------|----|
| OLED VCC / GND | 3V3 / GND |
| OLED SDA | GPIO 21 |
| OLED SCL | GPIO 22 |

### Buttons

| Button | Connection |
|--------|-----------|
| Left | GPIO 32 to GND |
| Middle | GPIO 33 to GND |
| Right | GPIO 13 to GND |

> The buttons use the ESP32's **internal pull-ups**, so no external resistors are needed.

## SD Card Preparation

- **Format:** FAT32
- **Folder:** `/mp3`
- **Files:**

| File | Song |
|------|------|
| `0001.mp3` | Nobudots |
| `0002.mp3` | Magbalik |
| `0003.mp3` | Billie Jean |
| `0004.mp3` | Pamela One Brazil |
| `0005.mp3` | Multo |
| `0006.mp3` | Gangnam Style |

> File names must start with the 4-digit track number. The DFPlayer plays tracks by number, and the song titles shown on the OLED are stored in the firmware.

## Software Requirements

- **ESP-IDF** (via PlatformIO with `framework = espidf`, board `esp32dev`)
- VS Code with the PlatformIO extension
- Serial terminal at 115200 baud (for logs)

The firmware uses native ESP-IDF drivers (UART, I2C, GPIO) for the DFPlayer, OLED, and buttons.

## Project Structure

```
Audio-Player/
├── .vscode/              # VS Code / PlatformIO settings
├── include/
├── lib/
├── src/                  # Application source (app_main)
├── test/
├── .gitignore
├── CMakeLists.txt        # ESP-IDF build configuration
├── platformio.ini        # PlatformIO config (framework = espidf)
├── sdkconfig.esp32dev    # ESP-IDF configuration for the esp32dev environment
└── README.md
```

## How It Works

1. The ESP32 talks to the DFPlayer over **UART2 at 9600 baud**, sending simple serial commands such as "play track 2".
2. The DFPlayer decodes the MP3 from the SD card and drives the speaker with its built-in amplifier.
3. Button handling converts raw presses into **click** and **double-click** events.
4. The program tracks the current song, play state, volume, and mode, and **redraws the OLED** whenever any of them change.

## Build & Flash

```bash
# Build and upload
pio run -t upload

# Open the serial monitor
pio device monitor -b 115200
```

## Usage

1. Insert the SD card and power the ESP32 over USB.
2. In **playlist mode**, press the middle button to play or pause, and use the left and right buttons to change song.
3. **Double-click** the middle button to switch to **volume mode**, then use the left and right buttons to change the volume and the middle button to mute.
4. Double-click again to return to playlist mode.

## Test Checklist

- [ ] Middle button plays and pauses the song
- [ ] OLED updates when the song changes
- [ ] Volume up, volume down, and mute work
- [ ] Double-click switches between playlist and volume mode
- [ ] Status returns to "Paused" when the track finishes
- [ ] Previous / next wrap around the list

## Differences from the Book

| Book | This Project |
|------|--------------|
| ESP32-S3-Box-Lite | Standard ESP32 DevKit |
| Color LCD with images | 128x64 monochrome OLED, so no images |
| Audio from SPIFFS with `info.json` | DFPlayer Mini plays MP3 files from the SD card; song titles are stored in the code |
| Dedicated mute | Mute sets the volume to 0 and restores it afterwards |

## Authors

- Kent Jesser Neri
- Alwyn Reuyan
- JC Redelosa

**Section:** BSCA-B186  |  **Date:** 11/15/2026
