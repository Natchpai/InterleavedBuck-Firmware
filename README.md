# InterleavedBuck Firmware

**Firmware for a 2-phase interleaved buck converter on an STM32G474RE, with an LQR-based PWM controller.**

## Overview

STM32CubeIDE project for the STM32G474RETx. The project is named `LQR_2PhaseBuck_RE`. The control code is in `LQR_PWM_ct.c` and `LQR_PWM_ct.h`.

## Specifications

| Parameter | Specification |
|---|---|
| MCU | STM32G474RETx |
| Toolchain | STM32CubeIDE (CubeMX `.ioc` project) |

*Specifications describe design targets unless explicitly identified as measured or verified results.*

## Getting Started

1. Open `LQR_2PhaseBuck_RE` in STM32CubeIDE (File > Open Projects from File System).
2. Build the Debug or Release configuration.
3. Flash with an ST-LINK debugger.

## Repository Structure

```text
InterleavedBuck_Firmware/
├── .gitignore
├── LICENSE
├── README.md
└── LQR_2PhaseBuck_RE/
    ├── Core/
    │   ├── Inc/            (main.h, LQR_PWM_ct.h, HAL config and IRQ headers)
    │   ├── Src/            (main.c, LQR_PWM_ct.c, HAL MSP, IRQ, system files)
    │   └── Startup/        (startup_stm32g474retx.s)
    ├── Drivers/            (CMSIS and STM32G4xx HAL driver)
    ├── LQR_2PhaseBuck_RE.ioc
    ├── STM32G474RETX_FLASH.ld
    └── STM32G474RETX_RAM.ld
```

## License

This project is licensed under the GNU General Public License v3.0. See [LICENSE](LICENSE).

Third-party code under `LQR_2PhaseBuck_RE/Drivers/` carries its own license (see the `LICENSE.txt` files in that folder).
