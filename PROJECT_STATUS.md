# Project status

Checkpoint: 2026-10-15, Europe/Vienna. Version: `0.1.0-bringup` (development).
Repository: https://github.com/voltex-at/DIY-Dreame-Remote

| Area | Written | Compiled/tested | Physical result |
| --- | --- | --- | --- |
| ES3C35P ST77922/touch/backlight port | Integrated with attribution | Full board CI passed | Source Shelly port user-tested; Dreame build untested |
| LVGL portrait UI/hardware tests/sleep-wake | Implemented | Full board CI passed | Not tested |
| Portable registry/command guard | Implemented | 67 checks passed locally with UBSan and in CI with ASan/UBSan | Not linked to Matter |
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

## Verified development build

[PR #1 (draft)](https://github.com/voltex-at/DIY-Dreame-Remote/pull/1).
Source `34965d73002e562166c3686dbfca1c7ed6d73a75`, tested PR merge
`e0c217766f42d653e4e9542c30a7189ed0b0a398`.
Both jobs passed in [run 38665012804](https://github.com/voltex-at/DIY-Dreame-Remote/actions/runs/38665012804).
[Hardware-test package](https://github.com/voltex-at/DIY-Dreame-Remote/actions/runs/38665012804/artifacts/11940617910):
USB image 619,936 bytes, app 488,864 bytes, 5,802,592 bytes free in the
provisional 6 MiB app slot (92%). This does not include Matter. Compiler logs
reported no C/C++ warnings or errors. 63 dependency license/notice files were
retained. See [VALIDATION](docs/VALIDATION.md) for hashes and static memory.

The package and logs were inspected through CI metadata/logs; the artifact ZIP
was not downloaded or independently unpacked in this session. The final
documentation checkpoint adds the exact lockfile resolved by this successful
build and records its evidence; the tested source and image hashes above remain
the reference for the hardware test. No runtime source changes followed it.

## Next executable milestone

Perform the [physical bring-up checklist](docs/INSTALL_AND_TEST_PL.md).
Independently select/pin the real commissioner stack: current ESP-Matter main
recommends IDF 6.0.2, whereas this hardware port uses 5.5.4. Integrate retail
attestation and persistent credentials before testing a real robot. Full Matter
image size, internal RAM, three-node sessions and secure OTA remain unmeasured.

No stable release or complete local-control firmware is published at this stage.
