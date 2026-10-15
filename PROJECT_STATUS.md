# Project status

Checkpoint: 2026-10-15, Europe/Vienna. Version: `0.1.0-bringup` (development).
Repository: https://github.com/voltex-at/DIY-Dreame-Remote

| Area | Written | Compiled/tested | Physical result |
| --- | --- | --- | --- |
| ES3C35P ST77922/touch/backlight port | Integrated with attribution | Awaiting this project's CI | Source Shelly port user-tested; Dreame build untested |
| LVGL portrait UI/hardware tests/sleep-wake | Implemented | Awaiting CI | Not tested |
| Portable registry/command guard | Implemented | Local and CI results in VALIDATION | Not linked to Matter |
| Wi-Fi setup/reconnect | Not implemented | None | None |
| Matter commissioner/CASE/discovery/RVC | Architecture and test plan only | No stack build | No robot paired or controlled |
| OTA installation/signatures/healthy boot | Design only; provisional dual slots | No installer | None |
| NVS persistence/migrations/security | NVS initialization only | No registry persistence | None |

## Evidence boundary

The board reference was read at Shelly commit
`51c5840e4098829fe8bd4cd0dfc13a552ce2133b`; only attributed hardware and UI test
helpers were copied. Shelly and sensor sources were not modified. Source Shelly
physical reports cover boot/display/touch/colors/brightness/wake, not this new app.

Manufacturer compatibility research is in [COMPATIBILITY](docs/COMPATIBILITY.md).
X50 Ultra Complete has official Matter marketing; X40 Ultra Complete has a
manufacturer rollout response. Exact variants and actual units remain unverified.
X20 needs exact identification. No local fallback or floor-map availability is claimed.

## Next executable milestone

Finish and record board CI and memory results, then request physical bring-up.
Independently select/pin the real commissioner stack: current ESP-Matter main
recommends IDF 6.0.2, whereas this hardware port uses 5.5.4. Integrate retail
attestation and persistent credentials before testing a real robot. Full Matter
image size, internal RAM, three-node sessions and secure OTA remain unmeasured.

No stable release or complete local-control firmware is published at this stage.
