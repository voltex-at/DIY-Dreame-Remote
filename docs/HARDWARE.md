# Hardware: LCDWiki ES3C35P

Hardware ID: `lcdwiki-es3c35p-n16r8`. ESP32-S3, 16 MiB QSPI Flash, 8 MiB
OPI PSRAM, 320 x 480 RGB565 IPS screen, ST77922 QSPI. Bluetooth LE only;
the vendor page's BR/EDR wording is not treated as Classic Bluetooth support.

Source: [manufacturer pin allocation](https://www.lcdwiki.com/3.5inch_ESP32-S3_Display),
checked 2026-10-15. The inherited port references the vendor schematic and the
[exact-board upstream port](https://github.com/78/xiaozhi-esp32/blob/da4bf4feb2f3c343136bd7d19d893ac60e9245b4/main/boards/lcdwiki-es3c35p/lcdwiki-es3c35p.cc).
Code provenance and licenses are in [NOTICE](../NOTICE.md).

| Signal | GPIO | Current treatment |
| --- | --- | --- |
| LCD CS / CLK | 10 / 12 | QSPI, 40 MHz |
| LCD D0/D1/D2/D3 | 11/13/14/9 | QSPI |
| LCD reset | EN shared with MCU | Software reset, no GPIO guessed |
| Backlight | 41 | Active high, 5 kHz PWM |
| Touch SDA/SCL | 38/39 | Shared I2C, 400 kHz |
| Touch reset / IRQ | 48 / 47 | Reset used; IRQ not driven; polling |
| RGB LED | 40 | Not initialized |
| Battery ADC | 8 | Not sampled |
| Audio amplifier enable | 1 | Active low; held high/disabled |
| Audio bus | 17/18/15/21/16 | Not initialized |
| SD CLK/CMD/D0/D1/D2/D3 | 5/4/6/7/2/3 | Not initialized |
| USB D-/D+ | 19/20 | USB Serial/JTAG |
| Expansion | 45/46 | Untouched |

Touch uses the integrated ST77922 protocol at address `0x55`, 16-bit register
addresses: capabilities `0x0009`, info `0x0010`, points `0x0014`. The mapping
comes from the attributed exact-board port. If probing fails, UI reports the
failure, auto screen-off is disabled, and manual screen-off is disabled.

Boot enforces detected Flash=16777216 and PSRAM=8388608. Logs report silicon
revision; **PCB revision needs visual inspection**. Backlight is kept off until
UI setup completes. Sleep switches off the backlight, not the MCU. First wake
touch is consumed until release. Brightness is temporary, not saved to NVS.

The source Shelly port was user-tested for screen/touch/colors/brightness/wake.
This evidence does not replace a physical test of the separate Dreame build.
Runtime heap/task-stack/uptime figures for Dreame are still unmeasured.
