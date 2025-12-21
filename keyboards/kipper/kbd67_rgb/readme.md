# kbd67_rgb

A 65% single-layout keyboard (there are 67 keys in total) with 1 RGB in capslock key.
This keyboard use 25mhz HSE and STM32F411 as MCU.
Using CH582F as bluetooth RF MCU.

Based on hl6095

- Keyboard Maintainer: https://github.com/KeyMagicHorse/qmk_firmware
- Hardware Supported: hl6095
- Hardware Availability:
Make example for this keyboard (after setting up your build environment):
    ```
    make kipper/kbd67_rgb:default
    ```
    ```
    make kipper/kbd67_rgb:ble
    ```
See [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) then the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information.

## Bootloader

Enter the bootloader in 2 ways:

- **Bootmagic reset**: Hold down the key at (0,0) in the matrix (usually the top left key which is Escape in this keyboard) and plug in the keyboard
- **Keycode in layout**: Press the key mapped to `QK_BOOT` if it is available.
