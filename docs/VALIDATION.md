# Validation evidence

This file records actual results, not milestone promises.

- Native core: production C++17 sources, fixed-capacity registry, identity
  separation across fabrics, unknown battery versus a real 0%, route selection,
  unsupported/offline/stale-command rejection, timeout readback, no ACK-as-success,
  wrong-robot/stale observation rejection and removal blocking during a command.
- **67 assertions passed** with UBSan locally and ASan/UBSan in
  [run 38665012804](https://github.com/voltex-at/DIY-Dreame-Remote/actions/runs/38665012804).
  This includes rename aliasing and stale request-generation callbacks.
- Full ES3C35P compilation, USB packaging and license collection passed in that
  same run. Source `34965d73002e562166c3686dbfca1c7ed6d73a75`; tested PR merge
  `e0c217766f42d653e4e9542c30a7189ed0b0a398`. No C/C++ compiler warnings/errors.
- Local toolchain: g++ available; ESP-IDF/Xtensa toolchain absent, so full board
  compilation is performed by the pinned GitHub Actions container.
- Local LeakSanitizer cannot inspect sandbox /proc; local validation uses
  `python3 tools/test.py --ubsan`. CI runs full ASan/UBSan without overrides.
- Hardware and real robot tests: not performed for this project.
- No claimed firmware memory/RAM figure includes Matter; it is not linked yet.

Every CI hardware artifact retains configure/build logs, ELF sections, size and
size-components reports, sdkconfig, resolved dependency lock, SHA-256 checksums
and exact source/tested commit IDs. Diagnostic artifact also retains ELF and map.

## Image and static-memory results

| Measurement | Bytes |
| --- | ---: |
| Raw application (`diy_dreame_remote.bin`) | 488864 |
| Merged USB image, flash at 0x0 | 619936 |
| App slot size | 6291456 |
| Unused app slot | 5802592 |
| Flash `.text` | 318880 |
| DIRAM `.text` | 54267 |
| DIRAM `.data` | 14816 |
| DIRAM `.bss` | 69480 |
| Flash `.rodata` | 84116 |
| IRAM `.text` | 15356 |

These are build-time section sizes, not live free-heap/stack measurements.
Matter, network sessions and trust stores are not linked. The final exact SDK
resolution is saved in the root `dependencies.lock`; the build container was
`espressif/idf:v5.5.4`, reported digest
`sha256:b9f2d6ea1c19e0c9f7959bdb74a9e3c775642f9d0f3b841937c5fa3363db892b`.

SHA-256 (reported by the successful CI packager):

```text
f8bd8cd0ea5feac2267c0a5271033537a8f1a2775508851bfeb42bdbae1a4bc3  diy_dreame_remote.bin
ba04fb3192086a5593e39d615f56f00b86a4e860de766a719d2f9fa72b61a6ef  DIY-Dreame-Remote-es3c35p-bringup.bin
```

[Development ZIP](https://github.com/voltex-at/DIY-Dreame-Remote/actions/runs/38665012804/artifacts/11940617910),
artifact `11940617910`, 926929 bytes. GitHub reports ZIP digest
`sha256:bb0e8801d4382c77b0972ae075e348dec2fb2e75c0278b443edb72a270ad3920`.
[ELF/map diagnostics](https://github.com/voltex-at/DIY-Dreame-Remote/actions/runs/38665012804/artifacts/11940692530).
The ZIP was not independently downloaded/unpacked here; the evidence is CI logs
and artifact metadata. Check its internal SHA256SUMS before hardware installation.

The following checkpoint only records evidence and adds the exact resolved lock;
it does not replace these tested commit IDs with an untested firmware claim.
