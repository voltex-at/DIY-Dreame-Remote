/* UI test helpers adapted from DIY Shelly Remote, MIT. See NOTICE.md. */
#include "ddr_ui.h"
#include "ddr_board.h"
#include "esp_app_desc.h"
#include "esp_heap_caps.h"
#include "esp_flash.h"
#include "esp_psram.h"
#include "lvgl.h"
#include <stdint.h>

static lv_obj_t *touch_status;
static lv_obj_t *brightness_text;
static unsigned touches;
static uint8_t brightness = 70;
static const uint32_t BG = 0x10141c, CARD = 0x1b2330, ACCENT = 0x39a7ff;
void ddr_ui_home(lv_event_t *event);
static void menu(lv_event_t *event);

lv_obj_t *ddr_ui_label(lv_obj_t *parent, const char *text, int x, int y, int width)
{
    lv_obj_t *obj = lv_label_create(parent);
    lv_label_set_text(obj, text);
    lv_obj_set_pos(obj, x, y);
    lv_obj_set_width(obj, width);
    lv_label_set_long_mode(obj, LV_LABEL_LONG_MODE_WRAP);
    return obj;
}

lv_obj_t *ddr_ui_button(lv_obj_t *parent, const char *text, int x, int y, int width,
                        lv_event_cb_t callback, void *context)
{
    lv_obj_t *obj = lv_button_create(parent);
    lv_obj_set_pos(obj, x, y);
    lv_obj_set_size(obj, width, 48);
    lv_obj_set_style_bg_color(obj, lv_color_hex(CARD), 0);
    lv_obj_set_style_radius(obj, 12, 0);
    lv_obj_t *caption = lv_label_create(obj);
    lv_label_set_text(caption, text);
    lv_obj_center(caption);
    if (callback) lv_obj_add_event_cb(obj, callback, LV_EVENT_CLICKED, context);
    return obj;
}

lv_obj_t *ddr_ui_screen(const char *title, bool navigation)
{
    touch_status = brightness_text = NULL;
    lv_obj_t *root = lv_screen_active();
    lv_obj_clean(root);
    lv_obj_set_style_bg_color(root, lv_color_hex(BG), 0);
    lv_obj_set_style_text_color(root, lv_color_hex(0xf0f4fb), 0);
    lv_obj_set_style_pad_all(root, 0, 0);
    lv_obj_remove_flag(root, LV_OBJ_FLAG_SCROLLABLE);
    ddr_ui_label(root, "DIY DREAME REMOTE", 16, 14, 288);
    lv_obj_t *heading = ddr_ui_label(root, title, 16, 49, 288);
    lv_obj_set_style_text_color(heading, lv_color_hex(ACCENT), 0);
    if (navigation) {
        ddr_ui_button(root, "Home", 12, 420, 140, ddr_ui_home, NULL);
        ddr_ui_button(root, "Menu", 168, 420, 140, menu, NULL);
    }
    return root;
}

static void touch_hit(lv_event_t *event)
{
    unsigned corner = (unsigned)(uintptr_t)lv_event_get_user_data(event);
    touches++;
    lv_label_set_text_fmt(touch_status, "Corner %u detected\nTouch count: %u", corner, touches);
}

static void touch_test(lv_event_t *event)
{
    (void)event;
    lv_obj_t *root = ddr_ui_screen("", false);
    lv_obj_clean(root);
    ddr_ui_button(root, "1", 4, 4, 72, touch_hit, (void *)(uintptr_t)1);
    ddr_ui_button(root, "2", 244, 4, 72, touch_hit, (void *)(uintptr_t)2);
    ddr_ui_button(root, "3", 4, 428, 72, touch_hit, (void *)(uintptr_t)3);
    ddr_ui_button(root, "4", 244, 428, 72, touch_hit, (void *)(uintptr_t)4);
    touch_status = ddr_ui_label(root, "Tap all four corners.\nEach press should match\nthe corner number.", 30, 170, 260);
    lv_obj_set_style_text_align(touch_status, LV_TEXT_ALIGN_CENTER, 0);
    ddr_ui_button(root, "Back", 100, 325, 120, ddr_ui_home, NULL);
}

static void colors(lv_event_t *event)
{
    (void)event;
    lv_obj_t *root = ddr_ui_screen("Display colors", true);
    ddr_ui_label(root, "Each label should match its color.", 16, 88, 288);
    const char *names[] = {"RED", "GREEN", "BLUE", "WHITE"};
    const uint32_t values[] = {0xff0000, 0x00ff00, 0x0000ff, 0xffffff};
    for (unsigned i = 0; i < 4; i++) {
        lv_obj_t *bar = lv_obj_create(root);
        lv_obj_set_pos(bar, 16, 133 + (int)i * 63);
        lv_obj_set_size(bar, 288, 48);
        lv_obj_set_style_bg_color(bar, lv_color_hex(values[i]), 0);
        lv_obj_set_style_border_width(bar, 0, 0);
        lv_obj_set_style_radius(bar, 8, 0);
        lv_obj_t *caption = lv_label_create(bar);
        lv_label_set_text(caption, names[i]);
        lv_obj_set_style_text_color(caption, lv_color_hex(i == 2 ? 0xffffff : 0x000000), 0);
        lv_obj_center(caption);
    }
}

static void change_brightness(lv_event_t *event)
{
    uint8_t percent = (uint8_t)lv_slider_get_value(lv_event_get_target(event));
    if (ddr_board_set_backlight(percent) == ESP_OK) {
        brightness = percent;
        lv_label_set_text_fmt(brightness_text, "Brightness: %u%%", (unsigned)brightness);
    }
}

static void sleep_display(lv_event_t *event) { (void)event; ddr_board_sleep_display(); }

static void hardware(lv_event_t *event);
static void firmware(lv_event_t *event)
{
    (void)event;
    lv_obj_t *root = ddr_ui_screen("Firmware Update", true);
    ddr_ui_label(root, esp_app_get_description()->version, 16, 100, 288);
    ddr_ui_label(root, "USB bring-up build\n\nOTA installation is not available yet.\n\nThis build does not check or install\nupdates over Wi-Fi.", 16, 155, 288);
}

static void settings(lv_event_t *event)
{
    (void)event;
    lv_obj_t *root = ddr_ui_screen("Settings", true);
    brightness_text = ddr_ui_label(root, "", 16, 92, 288);
    lv_label_set_text_fmt(brightness_text, "Brightness: %u%%", (unsigned)brightness);
    lv_obj_t *slider = lv_slider_create(root);
    lv_obj_set_pos(slider, 24, 144);
    lv_obj_set_size(slider, 272, 18);
    lv_slider_set_range(slider, 10, 100);
    lv_slider_set_value(slider, brightness, LV_ANIM_OFF);
    lv_obj_add_event_cb(slider, change_brightness, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_t *off = ddr_ui_button(root, "Screen off / touch to wake", 16, 204, 288, sleep_display, NULL);
    if (!ddr_board_touch_available()) lv_obj_add_state(off, LV_STATE_DISABLED);
    ddr_ui_button(root, "Firmware Update", 16, 270, 288, firmware, NULL);
    ddr_ui_label(root, "Auto off: 60 seconds if touch responds.\nBrightness resets after reboot.", 16, 346, 288);
}

static void matter_info(lv_event_t *event)
{
    (void)event;
    lv_obj_t *root = ddr_ui_screen("Local Matter control", true);
    ddr_ui_label(root, "Not paired / no robot data\n\nMatter commissioning is not available\nin this hardware bring-up.\n\nNext step: verify the Matter menu\nand firmware on your robot.\n\nNo map or battery data is simulated.", 16, 97, 288);
}

static void menu(lv_event_t *event)
{
    (void)event;
    lv_obj_t *root = ddr_ui_screen("Menu", true);
    ddr_ui_button(root, "Hardware tests", 16, 100, 288, hardware, NULL);
    ddr_ui_button(root, "Settings", 16, 164, 288, settings, NULL);
    ddr_ui_button(root, "Matter status", 16, 228, 288, matter_info, NULL);
    ddr_ui_label(root, "Local first / independent project\nWi-Fi and Matter: not started\nNo robot commands in this build.", 16, 314, 288);
}

void ddr_ui_home(lv_event_t *event)
{
    (void)event;
    lv_obj_t *root = ddr_ui_screen("Robots - none paired", true);
    const char *names[] = {"X20 - variant to verify", "X40 Ultra", "X50 Ultra"};
    for (unsigned i = 0; i < 3; ++i) {
        ddr_ui_button(root, names[i], 16, 94 + (int)i * 65, 288, matter_info, NULL);
    }
    ddr_ui_label(root, "Planned models, not discovered devices.\nOpen Menu for hardware tests.\n\nLAN: not configured", 16, 306, 288);
}

static void hardware(lv_event_t *event)
{
    (void)event;
    lv_obj_t *root = ddr_ui_screen("Hardware tests", true);
    uint32_t flash = 0;
    (void)esp_flash_get_size(esp_flash_default_chip, &flash);
    lv_obj_t *memory = ddr_ui_label(root, "", 16, 92, 288);
    lv_label_set_text_fmt(memory, "Flash: %u MiB / PSRAM: %u MiB\nInternal heap free: %u bytes\nTouch: %s", (unsigned)(flash / 1048576),
        (unsigned)(esp_psram_get_size() / 1048576), (unsigned)heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
        ddr_board_touch_available() ? "responding" : "NOT responding");
    ddr_ui_button(root, "Check touchscreen", 16, 213, 288, touch_test, NULL);
    ddr_ui_button(root, "Check display colors", 16, 277, 288, colors, NULL);
    ddr_ui_label(root, "ES3C35P / 320 x 480\nInspect PCB revision on your board.\nCheck Settings for brightness / wake.", 16, 350, 288);
}

void ddr_ui_init(void) { ddr_ui_home(NULL); }

