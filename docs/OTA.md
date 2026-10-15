# OTA: target contract and current status

**Not implemented in 0.1.0-bringup.** The Firmware Update screen reports this
plainly. Dual app slots and bootloader rollback configuration alone are not OTA.
There is no download, installation, signature verification or boot health commit.

Target flow: Settings -> System -> Firmware Update -> check release -> display
version/changelog -> download -> verify -> user confirms install -> boot candidate
-> health validation -> mark valid, or bootloader rollback on failed boot.
Downloading and installing are separate actions. No automatic install while a
robot command/session is active. Keep USB recovery.

Manifest must bind version, `lcdwiki-es3c35p-n16r8`, app size, SHA-256 and an
authenticity policy. SHA-256 from an untrusted manifest is not a signature.
Pin the release signing public key or select an explicitly documented ESP-IDF
signed-app policy. TLS certificate verification and redirect/host policy must
remain enabled; never log or publish private signing keys. Refuse downgrade,
hardware mismatch, image larger than the slot, digest/signature failure and
unsafe power if battery measurement becomes reliable.

Stream to the inactive OTA partition; only set boot target after full image
verification. Preserve NVS/fabric data; test migrations with rollback, including
old-firmware readability. No erase-on-schema-error. The running-slot health check
must include required memory, touch/display, persistent configuration and network
services; don't mark valid at the first line of app_main.

Current two 6 MiB slots are provisional. Re-evaluate after linking the full
commissioner and trust store, using measured app size and RAM. Flash encryption,
NVS encryption, secure boot provisioning and anti-rollback policy are undecided
and **not enabled**. No irreversible eFuse change is performed by this project.

Binary CI packages are development artifacts, not GitHub Releases. The raw
`diy_dreame_remote.bin` is retained for diagnostics/future OTA integration;
its presence does not make Settings -> Update functional.
