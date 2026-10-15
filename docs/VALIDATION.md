# Validation evidence

This file records actual results, not milestone promises.

- Native core: production C++17 sources, fixed-capacity registry, identity
  separation across fabrics, unknown battery versus a real 0%, route selection,
  unsupported/offline/stale-command rejection, timeout readback, no ACK-as-success,
  wrong-robot/stale observation rejection and removal blocking during a command.
- Full native ASan/UBSan and ES3C35P build: results will be recorded after CI.
- Local toolchain: g++ available; ESP-IDF/Xtensa toolchain absent, so full board
  compilation is performed by the pinned GitHub Actions container.
- Local LeakSanitizer cannot inspect sandbox /proc; local execution may use
  ASAN_OPTIONS=detect_leaks=0. CI must run without that override.
- Hardware and real robot tests: not performed for this project.
- No claimed firmware memory/RAM figure includes Matter; it is not linked yet.

Every CI hardware artifact retains configure/build logs, ELF sections, size and
size-components reports, sdkconfig, resolved dependency lock, SHA-256 checksums
and exact source/tested commit IDs. Diagnostic artifact also retains ELF and map.
