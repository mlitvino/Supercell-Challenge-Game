

# Supercell Challenge — Endless Runner

> **A fast-paced endless runner game created in 12 hours for the Supercell Challenge**

## 🎮 About The Game

This is an action-packed endless runner where you navigate through dynamic environments filled with challenges and opportunities. Jump across moving platforms, battle enemies, discover portals to mysterious worlds, and collect valuable resources to craft and upgrade your weapons.

### ✨ Key Features

- **Dynamic Platforms** — Navigate through constantly moving platforms that test your reflexes
- **Combat System** — Face enemies and defend yourself with sword attacks
- **Portal Mechanics** — Travel between different worlds including the mysterious Abyss
- **Crafting System** — Collect materials (Iron, Wood, Spirit) to craft and upgrade weapons
- **Power-Ups** — Grab Bombs and Shields to enhance your abilities
- **Endless Gameplay** — Survive as long as you can in this procedurally challenging experience

---

## Prerequisites

You must have [CMake](https://cmake.org/) version 3.22.1 or later installed to build this project.

If you are on Linux, make sure to install SFML's dependencies. For Debian-based distros:
```
sudo apt update
sudo apt install \
    libxrandr-dev \
    libxcursor-dev \
    libudev-dev \
    libfreetype-dev \
    libopenal-dev \
    libflac-dev \
    libvorbis-dev \
    libgl1-mesa-dev \
    libegl1-mesa-dev
```

## Building

Simply run `cmake -B build` and `cmake --build build`.
This builds the executable in `./build/bin/`.

The exact location of the executable may differ depending on your system and compiler. Example locations are `./build/bin/runner` and `./build/bin/Debug/runner.exe`.

[**SUBMIT**](https://supr.cl/hive2026submit)

---

## 🎯 How to Play

### Controls

| Key | Action |
|-----|--------|
| **W** | Jump up to higher platform |
| **S** | Drop down to lower platform |
| **Space** | Attack with sword |

### Game Elements

| Visual | Type | Description |
|--------|------|-------------|
| 🔵 Small Circle | **Material** | Collect Iron, Wood, or Spirit for crafting |
| 🟡 Medium Circle | **Power-Up** | Grab Bombs or Shields for temporary abilities |
| 🔴 Large Circle | **Portal** | Travel to the Abyss and back |

### Crafting System

Collect materials during your run to craft and upgrade your weapons. The stronger your weapon, the better your chances of survival!

---

## 🎵 Credits
Original music by Marllon Silva (xDeviruchi) https://www.youtube.com/xdeviruchi
