# Robot compatibility - checked 2026-10-15

No robot is physically paired or controlled by this project yet.
Marketing support, a rollout announcement and observed capabilities are separate evidence.

| Model owned / target | Source evidence | Status for this user's unit |
| --- | --- | --- |
| X20, exact variant unknown | No verified model-specific Matter support established | Unverified; need exact label/app model, region and firmware |
| X40 Ultra | Manufacturer support on its forum announced Matter for **X40 Ultra Complete**, gradual rollout from **4.3.9_1771**, on 2026-04-28 | Does not prove support on a different variant or this unit; inspect Matter menu and read endpoints |
| X50 Ultra | Official global page for **X50 Ultra Complete** explicitly advertises Matter | Family evidence only; actual variant, firmware, region and exposed clusters need testing |

Sources:

1. [Dreame Support DE response, X40 Ultra Complete](https://forum.dreametech.com/de/forum.php?mod=viewthread&tid=6543).
   The evidence is the named manufacturer support reply, not other users' speculation.
2. [Dreame Global X50 Ultra Complete](https://global.dreametech.com/products/dreame-x50-ultra-complete).
3. [Dreame Matter configuration](https://dreametech.zendesk.com/hc/en-us/articles/53660856250265-Matter-Configuration),
   updated 2026-07-31: bind in Dreamehome, then device settings -> auxiliary functions
   -> Matter for a QR/PIN; it is a generic guide, not a per-model support matrix.

An X20 might refer to distinct products/regions; don't infer Xiaomi Mi Home or
MiIO support from the shorthand. No fallback protocol is integrated.

After commissioning, discover Descriptor endpoints/server clusters, global
AttributeList/AcceptedCommandList/FeatureMap, supported RVC modes and operational
states. Only expose actions with an actual valid route and parameters. Missing
battery is unknown, not 0%. Service Area support can enable room selection;
it does not guarantee a drawable floor map or robot coordinates.

Required unit evidence: exact model/variant, app/robot firmware, region, whether
Matter menu is present, discovered endpoint/capability summary and WAN-off result.
Never place setup PINs, QR payloads, fabric credentials or Wi-Fi passwords in reports.
