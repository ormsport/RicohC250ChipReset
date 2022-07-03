# Ricoh Aficio SP C250/260/261 Toner Cartridge Chip Reset
This sketch using ATmega328P(Arduino Uno/Nano) to read/write Ricoh Aficio SP C250/260/261 toner cartridge chip(24C02) with I2C communication.

## This toner cartridge used in
- Ricoh SP C250DN
- Ricoh SP C250SF
- Ricoh SP C260DNw
- Ricoh SP C260SFNw
- Ricoh SP C261DNw
- Ricoh SP C261SFNw

## I2C Addresses
| Toner         | Addresses(DEC - HEX)  |
| ------------- | --------------------- |
| K - black     | 83 - 0x53             |
| C - cyan      | 82 - 0x52             |
| M - magenta   | 81 - 0x51             |
| Y - yellow    | 80 - 0x50             |