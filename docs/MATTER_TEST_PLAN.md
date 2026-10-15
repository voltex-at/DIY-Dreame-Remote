# First local Matter proof (next milestone)

This bring-up cannot run these steps yet. Implement and compile a pinned basic
commissioner first; never pretend these are commands already available here.

1. Collect exact X20 model and all three units' firmware/region/Matter-menu state.
   Choose the first unit that genuinely exposes Matter (X50 is a candidate only).
2. Keep the existing Dreamehome binding. Use its supported Matter or multi-admin
   commissioning window; do not factory-reset or flash the robot. Read the setup
   payload privately. Configure the remote on the same IPv6-capable LAN without
   client isolation; enable multicast/mDNS between the endpoints as needed.
3. Verify DAC chain, CD and revocation using retail PAA roots, not test roots.
   Persist controller root/NOC/keys and robot identity. Establish operator/admin
   ACL privileges using CASE. Never disable attestation to get a green result.
4. Read Basic Information and Descriptor endpoints. Record server-cluster lists,
   AttributeList, AcceptedCommandList, FeatureMap, RVC supported run/clean modes
   and operational states, plus Service Area if present. Read battery only if a
   supported attribute exists. Node/endpoint/mode IDs are discovered, not guessed.
5. Read current state and establish subscriptions. Make an explicit user action
   for one supported command, binding the selected robot identity. Show pending;
   confirm with fresh readback and physical observation. After timeout, read back
   first and never blindly replay start/dock. Confirm interruption before dock.
6. Reboot controller and robot separately. Re-establish CASE sessions and state;
   test operation from the original app and another existing controller.
7. Disconnect WAN while keeping Wi-Fi/LAN active. Repeat state read and one
   supported command. Test reconnect and UI responsiveness. Record results
   without credentials. An Internet-blocked pass is required for local-first.
8. Add the other robots one by one, switch active identity during a pending
   result, test offline behavior, then run uptime/heap/stack and OTA scenarios.

No graphical map is planned without an actual lawful local map data source.
If a unit has no verified local transport, record unverified or unsupported after
evidence, and keep its controls disabled. There is no hidden cloud fallback.
