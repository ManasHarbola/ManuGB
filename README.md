# ManuGB: Open Source GameBoy Emulator Written in C++
## Written by Manas Harbola

# Features
* Cycle-Accurate DMG Gameboy ROM emulation support (up to MBC3)
* * Sound currently unsupported
* Automatic save states
* Cycle-accurate CPU implementation: Passes Blargg's cpu_instrs test suite
* Scanline-based PPU implementation: Passes dmg-acid2 test
* Graphics-Independent implementation: Supports any graphics library, provided they implement Display and Joypad interfaces

## Supported ROM Cartridges
* MBC0
* MBC1
* MBC3

## Usage
* ./ManuGB \<path/to/rom\>
* ### Note: ManuGB writes save files to its "./saves" directory

## Key Mappings
| Button      | Key |
| :---------- | --: |
| Start       | N   |
| Select      | M   |
| B           | J   | 
| A           | K   |
| Up          | W   |
| Down        | A   |
| Left        | S   |
| Right       | D   |


## Emulator Commands
| Purpose    | Key |
| :------    | --: |
| Pause      | P   |
| Save State | T   |

## Supported Platforms
* MacOS
* Windows
* Linux

## Dependencies
* SDL2: Graphics library
* ManuGB but they will need to implement the "Display" and "Joypad" virtual classes

## Compiling
* Requires GNU Make
* May need to change SDL Include and Lib Paths
* Build release version: ```make emulator```
* Build debug version: ```make debug```
* Build debug + fps profiling version: ```make debug-profile```
