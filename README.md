# pico-bias-control

Firmware for the Raspberry Pi Pico 2 that controls the high-voltage module of X-Tream.

---

## Overview

- **Purpose:** Allow to configure & readout the polarity and the value of the high voltage bias applied to the sensor 
- **Control:** A Raspberry Pi 5 driving tx/rx lines.
  
---

## Requirements

- **Toolchain:** CMake ≥ 3.13, ARM GCC (from the Pico SDK toolchain).
- **SDK:** Raspberry Pi Pico SDK (set `PICO_SDK_PATH` or use the import cmake include).
- **Board:** Raspberry Pi Pico 2.

Recommended : use VSCode pico extension at https://github.com/raspberrypi/pico-vscode.

---

## Flash

1. Hold **BOOTSEL**, connect the Pico via USB.
2. Copy the generated `.uf2` from `build/` onto the **RP2**.
3. The board reboots and runs the firmware.

