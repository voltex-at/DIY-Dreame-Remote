# Architecture

## Current executable

`main/app_main.cpp` initializes NVS without erase-on-error, checks board memory,
starts display/touch, constructs the UI, then gives one task ownership of LVGL.
The touch task writes a protected sample; the LVGL read callback never waits for
I2C. A 5 ms timer only calls `lv_tick_inc`; the panel DMA completion only signals
flush-ready, as permitted by LVGL. Network and Bluetooth are not initialized.
One 320 x 40 x 2-byte DMA buffer consumes 25,600 bytes of internal RAM.

`components/ddr_core` is production portable C++17 logic, compiled by both IDF
and native tests. A bounded registry holds three robots with a compressed
Fabric ID + NodeId identity, optional battery value and discovered command
routes. No robot records are fabricated at startup and none are persisted yet.
UI currently shows model targets, explicitly not discovered devices.

Command lifecycle: sending -> awaiting readback -> confirmed/failed. Timeout
enters needs-readback and blocks a new command. ACK alone cannot confirm a
physical result. Observations need increasing revisions and a monotonic receive
time; target identity must match. A future adapter must pass an actual fresh
post-command read response, never cached state, and invalidate observations on
disconnect. Confirmation means the requested state was observed; it does not
prove that no other controller contributed to that state.

## Planned adapters (not implementations)

| Boundary | Responsibility | Current status |
| --- | --- | --- |
| `ddr_board` | LCD/touch/backlight/memory | Port integrated |
| `ddr_ui` | LVGL views and hardware tests | Bring-up implemented |
| `ddr_core` | Registry, routes, command lifecycle | Portable core tested |
| `matter` | Commissioner, CASE, discovery, RVC reads/subscriptions | Design only |
| `wifi` | Touch onboarding, IPv6 LAN/mDNS, reconnect | Design only |
| `storage` | Versioned NVS, fabric persistence, migrations | NVS init only |
| `ota` | Authenticated manifests, HTTPS, install/rollback | Design only |

Use a bounded request queue to a network/Matter worker and copied result events
to the UI. Never retain LVGL object pointers in network callbacks. Identify each
request by robot identity plus a generation counter; ignore late results after
navigation/removal/re-pairing. No network call may run in the UI task. Registry
updates will have a single owner; consumers receive snapshots.

## Matter stack selection

Research checked 2026-10-15 against official upstream sources:

- [ESP32-S3 controller documentation](https://docs.espressif.com/projects/esp-matter/en/latest/esp32s3/controller.html)
  provides controller/commissioner, on-network and BLE-Wi-Fi commissioning.
- [Current ESP-Matter README](https://github.com/espressif/esp-matter/blob/main/README.md)
  recommends ESP-IDF **v6.0.2**, connectedhomeip **539342f32d**; main tracks
  ongoing Matter 1.7 work. These versions are a research snapshot, not a lockfile.
- [Basic controller example](https://github.com/espressif/esp-matter/tree/main/examples/controller)
  is the starting point for a standalone fabric. RainMaker examples require
  additional RainMaker infrastructure and are not this project's local-first path.
- The basic example defaults to test PAA trust. This cannot authenticate a retail
  Dreame robot. Add a real PAA trust store and verification of DAC/CD/revocation.
- Use a persistent custom local operational-credentials issuer, with a documented
  root-key lifecycle, instead of treating the SDK test issuer as production-ready.

Decision: retain IDF 5.5.4 for board bring-up. Next isolate a controller build on
a documented, pinned ESP-Matter/IDF/CHIP combination (or migrate the board to
6.0.2 with a regression build). No claim is made that current main links with
5.5.4. Record exact commits and a resolved lock before integrating the controller.
Measure its image and RAM before committing to the provisional 6 MiB slots.

Commissioning must preserve existing controllers using multi-admin where
available. BLE is onboarding transport; routine commands use authenticated
Matter IP sessions in the LAN. Fabric data is never replaced by a saved IP.
Production credential protection and revocation freshness policy remain open
decisions, explicitly blocking a production release rather than bypassing checks.
