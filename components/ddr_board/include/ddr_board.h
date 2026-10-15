#ifndef DDR_BOARD_H
#define DDR_BOARD_H
#include "esp_err.h"
#ifdef __cplusplus
extern "C" {
#endif
const char *ddr_board_hardware_id(void);
esp_err_t ddr_board_init(void);
#ifdef DDR_BOARD_ES3C35P
#include <stdbool.h>
#include <stdint.h>
#include "lvgl.h"
lv_display_t *ddr_board_display(void);
esp_err_t ddr_board_start_ui_task(void);
esp_err_t ddr_board_set_backlight(uint8_t percent);
void ddr_board_sleep_display(void);
bool ddr_board_touch_available(void);
#endif
#ifdef __cplusplus
}
#endif
#endif
