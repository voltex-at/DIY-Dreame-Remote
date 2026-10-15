#include "ddr_board.h"
#include "ddr_ui.h"
#include "ddr_core.hpp"
#include "esp_app_desc.h"
#include "esp_chip_info.h"
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "nvs_flash.h"

extern "C" void app_main(void) {
    const char *tag = "ddr";
    ESP_LOGI(tag, "DIY Dreame Remote %s; board=%s", esp_app_get_description()->version, ddr_board_hardware_id());
    esp_chip_info_t chip{};
    esp_chip_info(&chip);
    ESP_LOGI(tag, "Silicon revision=%u, cores=%u. PCB revision requires visual inspection.", chip.revision, chip.cores);
    const esp_err_t nvs_result = nvs_flash_init();
    if (nvs_result != ESP_OK) {
        ESP_LOGE(tag, "NVS init failed: %s; preserving existing settings", esp_err_to_name(nvs_result));
        return;
    }
    const esp_err_t board_result = ddr_board_init();
    if (board_result != ESP_OK) {
        ESP_LOGE(tag, "Board init failed: %s", esp_err_to_name(board_result));
        return;
    }
    ddr_ui_init(); // Initial tree is built before the UI task owns LVGL.
    static ddr::Registry registry;
    ESP_LOGI(tag, "Robot registry: %u paired; capacity=%u", (unsigned)registry.size(), (unsigned)ddr::Registry::capacity);
    ESP_ERROR_CHECK(ddr_board_start_ui_task());
    ESP_ERROR_CHECK(ddr_board_set_backlight(70));
    ESP_LOGI(tag, "UI started. No Wi-Fi, Matter pairing or OTA installer in this bring-up.");
    // No automatic healthy-boot approval: future OTA requires user validation of
    // this hardware UI, plus persistent-state/network health checks.
    const esp_partition_t *running = esp_ota_get_running_partition();
    esp_ota_img_states_t state;
    if (running && esp_ota_get_state_partition(running, &state) == ESP_OK && state == ESP_OTA_IMG_PENDING_VERIFY) {
        ESP_LOGW(tag, "Pending OTA image is NOT marked valid by this diagnostic build");
    }
}
