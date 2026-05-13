# Xonix Game — Complete README

**Developed by:** Maham Anjum & Romaisa  
**Language:** C++  
**Graphics/Audio Library:** SFML (Simple and Fast Multimedia Library)  
**Build System:** CMake

---

## Table of Contents

1. [What is Xonix?](#what-is-xonix)
2. [Project Structure](#project-structure)
3. [Dependencies](#dependencies)
4. [How to Build and Run](#how-to-build-and-run)
5. [How to Run on Turbo C++ (TC)](#how-to-run-on-turbo-c)
6. [Menu Navigation Guide](#menu-navigation-guide)
7. [Game Controls](#game-controls)
8. [Levels & Difficulty](#levels--difficulty)
9. [Player Modes](#player-modes)
10. [Gameplay Rules & Mechanics](#gameplay-rules--mechanics)
11. [Scoring System](#scoring-system)
12. [Power-Up System](#power-up-system)
13. [Enemy Behavior](#enemy-behavior)
14. [Win & Lose Conditions](#win--lose-conditions)
15. [File Descriptions](#file-descriptions)
16. [Common Errors & Fixes](#common-errors--fixes)

---

## What is Xonix?

Xonix is a classic territory-capture arcade game. You control a player that moves across a grid. Your goal is to **fill/capture tiles** by drawing paths and enclosing areas — without getting hit by enemies. The more area you capture, the higher your score.

---

## Project Structure

```
xonix/
├── main.cpp                    ← Full game source code
├── CMakeLists.txt              ← Build configuration file
├── images/
│   ├── bg.png                  ← Background image
│   ├── tiles.png               ← Tile sprites
│   ├── enemy.png               ← Enemy sprite
│   └── gameover.png            ← Game over screen image
├── lt_internet/
│   ├── LTInternet-Bold.ttf     ← Font (Bold)
│   └── LTInternet-Regular.ttf  ← Font (Regular)
└── build/
    ├── xonix                   ← Compiled executable (Linux)
    ├── ARIAL.TTF               ← Font used in-game
    ├── bgmusic.ogg             ← Background music
    ├── menu.txt                ← Main menu display text
    ├── levels.txt              ← Levels menu display text
    ├── endmenu.txt             ← End/game-over menu text
    ├── playermodes.txt         ← Player mode selection text
    ├── scoreboard.txt          ← Saved high scores (auto-updated)
    ├── highscore.txt           ← Highest score record
    └── images/                 ← Copy of images used at runtime
```

---

## Dependencies

Before building, you must install the following:

### SFML (Required)
SFML handles the game window, graphics, and audio.

**On Ubuntu/Debian Linux:**
```bash
sudo apt-get update
sudo apt-get install libsfml-dev
```

**On Fedora/RHEL:**
```bash
sudo dnf install SFML-devel
```

**On Windows:**
Download from https://www.sfml-dev.org/download.php and link manually.

**On macOS:**
```bash
brew install sfml
```

### CMake (Required)
```bash
sudo apt-get install cmake
```

### g++ Compiler (Required)
```bash
sudo apt-get install g++
```

---

## How to Build and Run

### Step 1 — Extract the project
```bash
unzip xonix.zip
cd xonix
```

### Step 2 — Create a build directory
```bash
mkdir build
cd build
```

### Step 3 — Run CMake to configure
```bash
cmake ..
```

### Step 4 — Compile the project
```bash
make
```

This produces an executable called `xonix` inside the `build/` folder.

### Step 5 — Run the game
```bash
./xonix
```

> **Important:** You MUST run the game from inside the `build/` directory because it reads files like `ARIAL.TTF`, `menu.txt`, `bgmusic.ogg`, and `images/` from the **current working directory**.

If you try to run it from a different location, you will get errors like:
- `Error loading background music!`
- `failed to load background image!`

**Correct way:**
```bash
cd /path/to/xonix/build
./xonix
```

---

## How to Run on Turbo C++ (TC)

> ⚠️ **Important Notice:** This project **cannot run natively in Turbo C++** because:
> - It uses **SFML**, which is a modern C++ library not supported by Turbo C++.
> - Turbo C++ is a 16-bit compiler (circa 1990s) and does not support modern C++11/14/17 features used in this code (like `stringstream`, `max()`, `min()`, modern STL, etc.).
> - It uses `#include<SFML/Graphics.hpp>` and `#include<SFML/Audio.hpp>` which are unavailable in TC.

### What you should do instead:

**Option A — Use a modern IDE on Windows:**

1. Install **Visual Studio Community** (free) from https://visualstudio.microsoft.com/
2. Install **SFML for Windows** from https://www.sfml-dev.org/
3. Create a new C++ project, add `main.cpp`, link SFML libraries.
4. Copy `build/` contents (images, fonts, txt files, ogg file) next to your executable.
5. Build and run.

**Option B — Use Code::Blocks with MinGW (Windows):**

1. Download Code::Blocks with MinGW: http://www.codeblocks.org/
2. Download SFML (MinGW version) from https://www.sfml-dev.org/
3. In Code::Blocks:
   - Go to Project → Build Options → Linker Settings
   - Add: `sfml-graphics`, `sfml-window`, `sfml-system`, `sfml-audio`, `sfml-network`
   - Add the SFML include and lib paths in Search Directories.
4. Add `main.cpp` and compile.
5. Copy runtime files (ARIAL.TTF, images/, bgmusic.ogg, etc.) to the output folder.

**Option C — Use WSL (Windows Subsystem for Linux):**

1. Enable WSL on Windows: open PowerShell as Admin and run:
   ```
   wsl --install
   ```
2. Open Ubuntu terminal via WSL.
3. Install SFML: `sudo apt-get install libsfml-dev`
4. Follow the Linux build steps above.

**Option D — Already have a pre-built binary (Linux):**

The `build/xonix` file inside the zip is a **pre-compiled Linux executable**. If you are on Linux, you can run it directly:
```bash
cd xonix/build
chmod +x xonix
./xonix
```

---

## Menu Navigation Guide

All menus are controlled using **keyboard number keys** and **Escape**.

### Main Menu
```
Welcome to Xonix Game!
1. Start Game
2. Scoreboard
3. Exit
```
| Key | Action |
|-----|--------|
| `1` | Go to Level Selection |
| `2` | View Scoreboard |
| `3` | Exit the game |

---

### Level Selection Menu
```
Select Difficulty Level:
1. Easy         (2 enemies)
2. Medium       (4 enemies)
3. Hard         (6 enemies)
4. Continuous Mode
```
| Key | Action |
|-----|--------|
| `1` | Easy — 2 enemies |
| `2` | Medium — 4 enemies |
| `3` | Hard — 6 enemies |
| `4` | Continuous Mode — starts with 2, adds more every 20s |
| `Esc` | Back to Main Menu |

---

### Player Mode Menu
```
SELECT PLAYER MODE
1. Single Player
2. Two Players
```
| Key | Action |
|-----|--------|
| `1` | Single Player |
| `2` | Two Players |
| `Esc` | Back to Level Selection |

---

### Scoreboard Screen
Displays the **top scores** from past sessions (read from `scoreboard.txt`).

| Key | Action |
|-----|--------|
| `Esc` | Back to Main Menu |

---

### End / Game Over Menu
Shown after you win or lose:
```
Options:
1 - Restart
2 - Scoreboard
3 - Main Menu
4 - Exit
```
| Key | Action |
|-----|--------|
| `1` | Restart (goes to Level Selection) |
| `2` | View Scoreboard |
| `3` | Back to Main Menu |
| `4` | Close the game |

---

## Game Controls

### Single Player Mode

| Key | Action |
|-----|--------|
| `Arrow Left`  | Move player left |
| `Arrow Right` | Move player right |
| `Arrow Up`    | Move player up |
| `Arrow Down`  | Move player down |
| `Space`       | Activate Power-Up (freeze enemies) |

### Two Player Mode
> Two-player controls are present in the code. Player 2 uses a separate key set (see `twoPlayerMode` flag in code — implementation depends on current build).

---

## Levels & Difficulty

| Level | Name | Enemies at Start | Special |
|-------|------|-----------------|---------|
| 1 | Easy | 2 | Normal speed |
| 2 | Medium | 4 | Normal speed |
| 3 | Hard | 6 | Normal speed |
| 4 | Continuous Mode | 2 | Every 20 seconds, 2 more enemies are added (max 20) |

**Speed Increase:** Every 20 seconds in any level, all enemies increase their speed by `+0.2x`.

**Pattern Change:** After 30 seconds of gameplay, half the enemies switch from linear movement to zigzag or circular movement.

---

## Player Modes

- **Single Player:** You control one player using arrow keys.
- **Two Player:** Both players share the grid. Controlled via keyboard (check `twoPlayerMode` logic in `main.cpp`).

---

## Gameplay Rules & Mechanics

### The Grid
- The game board is a **25 × 40 tile grid**.
- Each tile is **18 pixels** wide.
- The border (outermost ring) is pre-filled and acts as a wall.

### Tile States (Color Meanings)
| Color | Meaning |
|-------|---------|
| **Slate Gray** | Unfilled/uncaptured area |
| **Royal Blue** | Border / captured wall |
| **Orange-Red** | Your active trail (currently being drawn) |
| **Gold/Yellow** | Successfully filled/captured area |
| **Green** | Your current player position |

### How Capturing Works
1. Move off the border into the open area — your trail is drawn in **orange-red**.
2. Return to the border (blue wall) to **close** the path.
3. The system uses a **flood-fill algorithm** from each enemy's position.
4. Areas the flood-fill does **not** reach are considered enclosed and get **filled (gold)**.
5. You earn **1 point per tile** you enter, plus bonuses.

### Death Conditions
You die (game over) if:
- An **enemy touches your current trail** (orange-red tiles).
- An **enemy reaches your player position**.
- An enemy enters a **filled area** (gold tiles).

---

## Scoring System

| Event | Points |
|-------|--------|
| Moving into an unfilled tile | +1 point |
| Every 10 tiles entered consecutively | +1 bonus point |
| Capturing all tiles | Victory screen |

**High Score:** Saved automatically to `scoreboard.txt` at game over or victory. Top 5 scores are shown in the Scoreboard screen.

---

## Power-Up System

**Earning Power-Ups:**
- You receive 1 power-up when you reach **score 50**.
- After that, you get 1 power-up **every 30 additional points** (80, 110, 140, ...).

**Using Power-Ups:**
- Press **`Space`** during gameplay.
- All enemies are **frozen for 3 seconds**.
- Power-up count is shown on the HUD (top-left of screen).

---

## Enemy Behavior

Enemies start with **linear movement** (bouncing diagonally). After 30 seconds:

| Pattern | Behavior |
|---------|----------|
| Linear (default) | Moves diagonally, bounces off walls |
| Zigzag | Moves with a sine-wave vertical oscillation |
| Circular | Orbits around a center point |

When an enemy hits a wall tile, it **reverses direction**. Enemy speed cannot be zero — if it reaches zero it is randomly assigned a new direction.

---

## Win & Lose Conditions

### You WIN when:
- Every interior tile of the grid is captured (filled gold or wall blue).
- Screen shows: `VICTORY! You captured all tiles!`
- Final score, time played, and total moves are shown.

### You LOSE when:
- An enemy hits your trail or player position.
- Screen shows: `GAME OVER` with your final score.

After either outcome, music stops and the End Menu appears.

---

## File Descriptions

| File | Purpose |
|------|---------|
| `main.cpp` | Complete game source code — all logic, rendering, menus |
| `CMakeLists.txt` | CMake build script — links SFML and copies images |
| `build/xonix` | Pre-compiled Linux executable |
| `build/ARIAL.TTF` | Font file loaded at runtime for all text rendering |
| `build/bgmusic.ogg` | Background music (loops during gameplay) |
| `build/menu.txt` | Text displayed on the main menu screen |
| `build/levels.txt` | Text displayed on the level selection screen |
| `build/endmenu.txt` | Text displayed on game-over/victory screen |
| `build/playermodes.txt` | Text displayed on player mode screen |
| `build/scoreboard.txt` | Stores all past scores (sorted descending, auto-updated) |
| `build/highscore.txt` | Stores the single highest score |
| `images/bg.png` | Background image shown on menu screens |
| `images/tiles.png` | Sprite sheet for grid tiles |
| `images/enemy.png` | Enemy sprite |
| `images/gameover.png` | Game over overlay image |

---

## Common Errors & Fixes

| Error | Cause | Fix |
|-------|-------|-----|
| `Error loading background music!` | `bgmusic.ogg` not found | Run the game from inside the `build/` directory |
| `failed to load background image!` | `images/bg.png` not found | Same — must run from `build/` |
| Font not loaded / blank text | `ARIAL.TTF` missing | Ensure `ARIAL.TTF` is in the same folder as the executable |
| `SFML/Graphics.hpp: No such file` | SFML not installed | Run `sudo apt-get install libsfml-dev` |
| `cmake: command not found` | CMake not installed | Run `sudo apt-get install cmake` |
| Executable won't run on Windows | Binary compiled for Linux | Recompile on Windows using SFML for MinGW or Visual Studio |
| Game opens but music crashes | Hardcoded absolute path in code | The code has a hardcoded path `/home/maham/Downloads/xonix/build/bgmusic.ogg`. You must **edit `main.cpp` line 247** and change it to just `"bgmusic.ogg"` before recompiling |

### Fix for the Hardcoded Music Path

Open `main.cpp` and find line ~247:
```cpp
if(!backgroundMusic.openFromFile("/home/maham/Downloads/xonix/build/bgmusic.ogg")) {
```
Change it to:
```cpp
if(!backgroundMusic.openFromFile("bgmusic.ogg")) {
```
Then recompile using `make`.

---

## Quick Start Summary

```bash
# 1. Extract
unzip xonix.zip && cd xonix

# 2. Install SFML (Linux)
sudo apt-get install libsfml-dev cmake g++

# 3. Fix the hardcoded music path in main.cpp (line ~247)
# Change: /home/maham/Downloads/xonix/build/bgmusic.ogg
# To:     bgmusic.ogg

# 4. Build
mkdir build && cd build
cmake ..
make

# 5. Run (must be inside build/ folder)
./xonix
```

---

*README generated from source code analysis of main.cpp, CMakeLists.txt, and all runtime text files.*
