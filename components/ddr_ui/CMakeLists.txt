idf_component_register(SRCS "ddr_ui.c" INCLUDE_DIRS "include"
    PRIV_REQUIRES ddr_board app_update heap spi_flash esp_psram)
