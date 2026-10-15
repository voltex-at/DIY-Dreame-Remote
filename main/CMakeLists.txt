idf_component_register(SRCS "app_main.cpp" INCLUDE_DIRS "."
    PRIV_REQUIRES ddr_board ddr_core ddr_ui nvs_flash app_update esp_timer)
