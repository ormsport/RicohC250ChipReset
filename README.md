# Ricoh Aficio SP C250/260/261 Toner Cartridge Chip Reset
This sketch using ATmega328P(Arduino Uno/Nano) to read/write Ricoh Aficio SP C250/260/261 toner cartridge chip(24C02) with I2C communication.

## This toner cartridge used in
- Ricoh SP C250DN
- Ricoh SP C250SF
- Ricoh SP C260DNw
- Ricoh SP C260SFNw
- Ricoh SP C261DNw
- Ricoh SP C261SFNw

## Features
- Set toner to full. (chip reset)
- Set toner to empty. (may useless)
- Selectable. (each toner cartridge, all present toner cartridge)
- Write custom data. (1 to 128 bytes, but can not use with all present toner cartridge.)

## Usage
1. Open sketch in Arduino IDE.
2. To write custom data, add your data to variable shown below. If just want to do a chip reset, skip this step.
```C++
const byte CUSTOM_DATA[] = {168,0,1,...};
```
3. Upload sketch to your board.
4. Following the instruction shown in serial monitor.

## Options
### Operation
- f - set to full
- e - set to empty
- c - write custom data

### Target
- c - cyan
- m - magenta
- y - yellow
- k - black
- a - all present toner cartridge (can not use with custom data)

Note: Each entry will appear only when detected.

## I2C Addresses
| Toner         | Addresses(DEC - HEX)  |
| ------------- | --------------------- |
| K - black     | 83 - 0x53             |
| C - cyan      | 82 - 0x52             |
| M - magenta   | 81 - 0x51             |
| Y - yellow    | 80 - 0x50             |

## Sample Schematic
Soon!