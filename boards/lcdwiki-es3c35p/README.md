# lcdwiki-es3c35p-n16r8

Exact target: ESP32-S3 N16R8, ST77922 QSPI portrait 320 x 480, integrated I2C touch.
See [HARDWARE.md](../../docs/HARDWARE.md) for pinout, provenance and boundaries.
Build from a clean checkout using ESP-IDF 5.5.4. The defaults here are part of
the board profile, not settings for arbitrary 3.5-inch boards.

Provisional flash map (not a measured Matter capacity promise):

| Partition | Offset | Size |
| --- | --- | --- |
| nvs | 0x9000 | 0x10000 (64 KiB) |
| otadata | 0x19000 | 0x2000 |
| phy_init | 0x1b000 | 0x1000 |
| ota_0 | 0x20000 | 0x600000 (6 MiB) |
| ota_1 | 0x620000 | 0x600000 (6 MiB) |

End: 0xc20000, below the 0x1000000 Flash limit. Remaining space is unassigned.
No PAA trust-store or production encrypted NVS partition has been provisioned.
Rollback bootloader support is configured; there is no OTA installer or
healthy-boot approval in this bring-up. Do not install it via another project's OTA.
