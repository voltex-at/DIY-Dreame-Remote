#!/usr/bin/env python3
"""Manifest a development build; not an OTA trust/signature manifest."""
import argparse
import hashlib
import json
from pathlib import Path

parser = argparse.ArgumentParser()
parser.add_argument('--out', type=Path, required=True)
parser.add_argument('--source', required=True)
parser.add_argument('--tested', required=True)
args = parser.parse_args()
app = args.out / 'diy_dreame_remote.bin'
usb = args.out / 'DIY-Dreame-Remote-es3c35p-bringup.bin'
slot_size = 0x600000
if not 0 < app.stat().st_size <= slot_size:
    raise SystemExit('Application exceeds the provisional 6 MiB OTA slot')
report = {
    'hardware_id': 'lcdwiki-es3c35p-n16r8',
    'version': '0.1.0-bringup',
    'source_commit': args.source,
    'tested_commit': args.tested,
    'app_bytes': app.stat().st_size,
    'app_sha256': hashlib.sha256(app.read_bytes()).hexdigest(),
    'usb_bytes': usb.stat().st_size,
    'usb_sha256': hashlib.sha256(usb.read_bytes()).hexdigest(),
    'slot_bytes': slot_size,
    'free_slot_bytes': slot_size - app.stat().st_size,
    'matter_linked': False,
    'ota_installer': False,
    'physical_test': 'not performed',
}
(args.out / 'BUILD_REPORT.json').write_text(json.dumps(report, indent=2) + '\n')
lines = []
for file in sorted(args.out.rglob('*')):
    if file.is_file() and file.name != 'SHA256SUMS.txt':
        lines.append(f'{hashlib.sha256(file.read_bytes()).hexdigest()}  {file.relative_to(args.out).as_posix()}')
(args.out / 'SHA256SUMS.txt').write_text('\n'.join(lines) + '\n')
print(json.dumps(report, indent=2))
print((args.out / 'size.txt').read_text())
# Public dependency metadata only, retained in logs for reproducible source locks.
print('DEPENDENCY_LOCK_JSON=' + json.dumps((args.out / 'dependencies.lock').read_text()))
