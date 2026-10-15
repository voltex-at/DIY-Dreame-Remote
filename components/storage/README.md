# Storage boundary

Only nvs_flash_init is currently called by app_main, without erase-on-error.
Robot registry, settings, issuer keys and Matter fabric persistence/migrations
are not implemented. Protect credentials and plan NVS/Flash encryption before
production; current bring-up does not enable encryption or program eFuses.
