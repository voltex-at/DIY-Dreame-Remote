# Third-party notices

New project code is MIT. The board port and UI hardware-test helpers are adapted
from [voltex-at/DIY-Shelly-Remote](https://github.com/voltex-at/DIY-Shelly-Remote/tree/51c5840e4098829fe8bd4cd0dfc13a552ce2133b),
commit `51c5840e4098829fe8bd4cd0dfc13a552ce2133b`, MIT, copyright (c) 2026
DIY Shelly Remote contributors. Its full license is retained in
`third_party/DIY-Shelly-Remote-LICENSE`.

`components/ddr_board/st77922_init.h` originated in the exact-board
MIT port of `78/xiaozhi-esp32`, upstream blob
`138be5c01b9cb98ba8bec62e899c60ab72af010c`. The original notices and full
license are in `third_party/xiaozhi-esp32-LICENSE`. The touch register mapping
was informed by that same port; it is not a generic FT6336 driver.

Managed dependencies: LVGL 9.3.0 (MIT), Espressif ST77922 driver 2.1.1
(Apache-2.0). ESP-IDF v5.5.4 is primarily Apache-2.0 and includes separately
licensed components. Binary test packages collect SDK and managed dependency
license notices using `tools/package_licenses.py`. The collection is intentionally
inclusive and does not imply every included component is linked.

ESP-Matter and CHIP are not linked in this release. Their notices and exact
dependency revision will be added when integrated. Dreame is a third-party
trademark; no proprietary code, logos, icons or app assets are included.
