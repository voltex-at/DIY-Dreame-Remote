# Validation evidence

This file records actual results, not milestone promises.

- Native core: production C++17 sources, fixed-capacity registry, identity
  separation across fabrics, unknown battery versus a real 0%, route selection,
  unsupported/offline/stale-command rejection, timeout readback, no ACK-as-success,
  wrong-robot/stale observation rejection and removal blocking during a command.
- Native baseline: 63 assertions passed with UBSan locally and ASan/UBSan in
  [CI run 38664676255](https://github.com/voltex-at/DIY-Dreame-Remote/actions/runs/38664676255).
  Four additional checks cover renaming from an aliased existing name (67 total).
  Final full-board and sanitizer results will be recorded after CI.
- Local toolchain: g++ available; ESP-IDF/Xtensa toolchain absent, so full board
  compilation is performed by the pinned GitHub Actions container.
- Local LeakSanitizer cannot inspect sandbox /proc; local validation uses
  `python3 tools/test.py --ubsan`. CI runs full ASan/UBSan without overrides.
- Hardware and real robot tests: not performed for this project.
- No claimed firmware memory/RAM figure includes Matter; it is not linked yet.

Every CI hardware artifact retains configure/build logs, ELF sections, size and
size-components reports, sdkconfig, resolved dependency lock, SHA-256 checksums
and exact source/tested commit IDs. Diagnostic artifact also retains ELF and map.
