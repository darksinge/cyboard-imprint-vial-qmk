/* Copyright 2023 Cyboard LLC (@Cyboard-DigitalTailor)
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include QMK_KEYBOARD_H
#include <string.h>

enum imprint_keymap_layers {
    LAYER_BASE = 0,
    LAYER_COLEMAK,
    LAYER_SYMBOLS,
    LAYER_DAVINCI_RESOLVE,
    LAYER_POINTER,
    LAYER_DANGER,
};

/** \brief Automatically enable sniping-mode on the pointer layer. */
#define CHARYBDIS_AUTO_SNIPING_ON_LAYER LAYER_POINTER

#define TOHOME TO(LAYER_BASE)
#define LOWER MO(LAYER_SYMBOLS)
#define TO_CLMK TO(LAYER_COLEMAK)
#define POINTER MO(LAYER_POINTER)
#define TO_DVCI_RSLV TO(LAYER_DAVINCI_RESOLVE)
#define TO_DANGER MO(LAYER_DANGER)
#define AMETHYST S(KC_LALT)
#define TMUX_PREFIX C(KC_S)
#define PT_Z LT(LAYER_POINTER, KC_Z)
#define PT_SLSH LT(LAYER_POINTER, KC_SLSH)
#define S_MS3 S(KC_BTN3) // Shift+MB3 for Panning in Fusion360
#define WS_TOG RGUI(RSFT(RCTL(RALT(KC_T)))) // Whisper Speak Toggle
#define CB_HIST G(C(A(KC_V))) // Open Clipboard History (Vorssaint )

// Trackball is on the right half only; cyboard.h's generic aliases point at undefined keycodes.
#ifdef POINTING_DEVICE_ENABLE
#    undef DPI_MOD
#    undef DPI_RMOD
#    undef S_D_MOD
#    undef S_D_RMOD
#    undef SNIPING
#    undef SNP_TOG
#    undef DRGSCRL
#    undef DRG_TOG
#    define DPI_MOD RIGHT_POINTER_DEFAULT_DPI_FORWARD
#    define DPI_RMOD RIGHT_POINTER_DEFAULT_DPI_REVERSE
#    define S_D_MOD RIGHT_POINTER_SNIPING_DPI_FORWARD
#    define S_D_RMOD RIGHT_POINTER_SNIPING_DPI_REVERSE
#    define SNIPING RIGHT_SNIPING_MODE
#    define SNP_TOG RIGHT_SNIPING_MODE_TOGGLE
#    define DRGSCRL RIGHT_DRAGSCROLL_MODE
#    define DRG_TOG RIGHT_DRAGSCROLL_MODE_TOGGLE
#else
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define DPI_RMOD KC_NO
#    define S_D_MOD KC_NO
#    define S_D_RMOD KC_NO
#    define SNIPING KC_NO
#    define DRG_TOG KC_NO
#    define SNP_TOG KC_NO
#endif // POINTING_DEVICE_ENABLE

// Left-hand home row mods
#define CTL_A LCTL_T(KC_A)
#define ALT_S LALT_T(KC_S)
#define GUI_D LGUI_T(KC_D)

// Left-hand home row mods for Colemak
#define CTL_A_CM LCTL_T(KC_A)
#define ALT_R_CM LALT_T(KC_R)
#define GUI_S_CM LGUI_T(KC_S)

// Right-hand home row mods
#define GUI_K RGUI_T(KC_K)
#define ALT_L LALT_T(KC_L)
#define CTL_SCLN RCTL_T(KC_SCLN)

// Right-hand home row mods for Colemak
#define GUI_E_CM RGUI_T(KC_E)
#define ALT_I_CM LALT_T(KC_I)
#define CTL_O_CM RCTL_T(KC_O)

enum custom_keycodes {
    VI_SLCT_BLK = SAFE_RANGE,
    HUE_INC,
    APPL_GLOBE,
    CC_PICKER,
    SESSION_PICKER,
};

static uint8_t  base_layer_hue = 0;
static uint16_t hue_inc_timer  = 0;
static bool     hue_inc_held   = false;
bool            process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case VI_SLCT_BLK:
            if (record->event.pressed) {
                SEND_STRING("V$%");
            }
            break;
        case HUE_INC:
            if (record->event.pressed) {
                base_layer_hue = (base_layer_hue + 1) % 256;
                hue_inc_held   = true;
                hue_inc_timer  = timer_read();
            } else {
                hue_inc_held = false;
            }
            break;
        case APPL_GLOBE:
            host_consumer_send(record->event.pressed ? AC_NEXT_KEYBOARD_LAYOUT_SELECT : 0);
            return false;
        case CC_PICKER:
            if (record->event.pressed) {
                tap_code16(TMUX_PREFIX);
                tap_code16(S(KC_C));
            }
            break;
        case SESSION_PICKER:
            if (record->event.pressed) {
                tap_code16(TMUX_PREFIX);
                tap_code16(KC_S);
            }
            break;
#ifdef POINTING_DEVICE_ENABLE
#    ifdef CHARYBDIS_AUTO_SNIPING_ON_LAYER
        case KC_BTN3:
            if (get_mods() == MOD_BIT(KC_LSFT)) {
                if (record->event.pressed) {
                    charybdis_set_pointer_sniping_enabled(false, false);
                } else {
                    charybdis_set_pointer_sniping_enabled(layer_state_is(CHARYBDIS_AUTO_SNIPING_ON_LAYER), false);
                }
            }
            break;
#    endif // CHARYBDIS_AUTO_SNIPING_ON_LAYER
#endif     // POINTING_DEVICE_ENABLE
    }
    return true;
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT_num_full_bottom_row(
       KC_ESC,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,     KC_EQL,
       KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                                        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,     KC_BSLS,
       KC_MINS,   CTL_A,   ALT_S,   GUI_D,   KC_F,    KC_G,                                        KC_H,    KC_J,    GUI_K,   ALT_L,   CTL_SCLN, KC_QUOT,
       POINTER,   KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                                        KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  CW_TOGG,
       XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     KC_LSFT, LOWER,   KC_ENT,   KC_SPC,  KC_BSPC, XXXXXXX,     KC_LEFT, KC_UP,   KC_DOWN, KC_RIGHT, XXXXXXX,
                                                          KC_LGUI, KC_LCTL, XXXXXXX,  XXXXXXX, XXXXXXX, AMETHYST
  ),

  [LAYER_COLEMAK] = LAYOUT_num_full_bottom_row(
       KC_ESC,    KC_1,     KC_2,     KC_3,     KC_4,    KC_5,                                     KC_6,    KC_7,    KC_8,     KC_9,     KC_0,     KC_EQL,
       KC_TAB,    KC_Q,     KC_W,     KC_F,     KC_P,    KC_G,                                     KC_J,    KC_L,    KC_U,     KC_Y,     KC_SCLN,  KC_BSLS,
       KC_MINS,   CTL_A_CM, ALT_R_CM, GUI_S_CM, KC_T,    KC_D,                                     KC_H,    KC_N,    GUI_E_CM, ALT_I_CM, CTL_O_CM, KC_QUOT,
       POINTER,   KC_Z,     KC_X,     KC_C,     KC_V,    KC_B,                                     KC_K,    KC_M,    KC_COMM,  KC_DOT,   KC_SLSH,  TOHOME,
       XXXXXXX,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,     KC_LSFT, LOWER,   KC_ENT,   KC_SPC,  KC_BSPC, XXXXXXX,     KC_LEFT, KC_UP,   KC_DOWN, KC_RIGHT, XXXXXXX,
                                                             KC_LGUI, KC_LCTL, XXXXXXX,  XXXXXXX, XXXXXXX, AMETHYST
  ),

  [LAYER_SYMBOLS] = LAYOUT_num_full_bottom_row(
       C(KC_UP),  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                       KC_F6,         KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
       RGB_MOD,   WS_TOG,  KC_AT,   KC_LCBR, KC_RCBR, VI_SLCT_BLK,                                 S(A(KC_MINS)), KC_PLUS, KC_ASTR, KC_EXLM, KC_RBRC, KC_F12,
       RGB_TOG,   KC_HASH, KC_DLR,  KC_LPRN, KC_RPRN, KC_TAB,                                      KC_MINS,       KC_EQL,  KC_GT,   KC_PIPE, KC_TILD, KC_SLSH,
       KC_DEL,    KC_PERC, KC_CIRC, KC_LBRC, KC_RBRC, KC_GRAVE,                                    KC_AMPR,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TO_CLMK,
       _______,   _______, _______, _______, _______,     TMUX_PREFIX, _______,   KC_CAPS,  KC_SPC,  SESSION_PICKER, _______,     _______, _______, _______, _______, _______,
                                                          APPL_GLOBE,  TO_DANGER, _______,  _______, _______,        S(KC_ENT)
  ),

  [LAYER_DAVINCI_RESOLVE] = LAYOUT_num_full_bottom_row(
       KC_ESC,    KC_F1,   KC_F2,         KC_F3,      KC_F4,         KC_T,                         KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  TOHOME,
       KC_D,      KC_N,    C(G(KC_L)),    G(A(KC_L)), A(KC_Y),       A(KC_X),                      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
       G(KC_R),   KC_A,    S(G(KC_LBRC)), G(KC_B),    S(G(KC_RBRC)), S(KC_BSPC),                   KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
       POINTER,   KC_Z,    KC_J,          KC_SPC,     KC_L,          KC_BSPC,                      G(KC_C), A(KC_V), KC_COMM, KC_DOT,  KC_SLSH, KC_LGUI,
       _______,   _______, _______,       _______,    _______,           KC_LSFT, A(KC_V), KC_ENT,   KC_SPC,  KC_BSPC, _______,     _______, _______, _______, _______, _______,
                                                                         KC_LGUI, KC_LCTL, _______,  _______, _______, KC_LALT
  ),

  [LAYER_POINTER] = LAYOUT_num_full_bottom_row(
       CB_HIST,   CC_PICKER,  XXXXXXX, XXXXXXX,     XXXXXXX, QK_BOOT,                              QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TO_DVCI_RSLV,
       HUE_INC,   C(KC_LEFT), G(KC_W), C(KC_RIGHT), DPI_MOD, DPI_RMOD,                             KC_PLUS, KC_7,    KC_8,    KC_9,    KC_ASTR, KC_SLSH,
       KC_LGUI,   G(KC_A),    G(KC_S), XXXXXXX,     G(KC_F), S_D_MOD,                              KC_MINS, KC_4,    KC_5,    KC_6,    KC_ENT,  KC_BTN1,
       _______,   DRGSCRL,    G(KC_X), G(KC_C),     G(KC_V), S_D_RMOD,                             KC_0,    KC_1,    KC_2,    KC_3,    KC_DOT,  SNP_TOG,
       _______,   _______,    _______, _______,     _______,     KC_BTN1, KC_BTN2, KC_BTN3,   KC_BTN5, KC_BTN4, _______,     _______, _______, _______, _______, _______,
                                                                 S_MS3,   KC_LOPT, _______,   _______, _______, KC_LSFT
  ),

  [LAYER_DANGER] = LAYOUT_num_full_bottom_row(
       XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, EE_CLR,  QK_BOOT,                                     QK_BOOT, EE_CLR,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
       XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
       XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
       XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
       XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //                                                               👇🏻from TO_DANGER key on LAYER_SYMBOLS
                                                          XXXXXXX, _______, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX
  )
};
// clang-format on

void matrix_scan_user(void) {
    if (hue_inc_held && TIMER_DIFF_16(timer_read(), hue_inc_timer) >= 50) {
        base_layer_hue = (base_layer_hue + 1) % 256;
        hue_inc_timer  = timer_read();
    }
}

#ifdef POINTING_DEVICE_ENABLE
#    ifdef CHARYBDIS_AUTO_SNIPING_ON_LAYER
layer_state_t layer_state_set_user(layer_state_t state) {
    charybdis_set_pointer_sniping_enabled(layer_state_cmp(state, CHARYBDIS_AUTO_SNIPING_ON_LAYER), false);
    return state;
}
#    endif // CHARYBDIS_AUTO_SNIPING_ON_LAYER
#endif     // POINTING_DEVICE_ENABLE

typedef struct {
    const char *name;
    uint8_t     hue;
} ColorMap;

static const ColorMap colors[] = {{"red", 0}, {"green", 85}, {"blue", 170}, {"cyan", 128}, {"orange", 6}, {"yellow", 43}, {"pink", 234}, {"purple", 200}, {"teal", 150}, {"amber", 36}, {"indigo", 190}, {"lime", 64}, {NULL, 0}};

uint8_t get_hue_by_name(const char *name) {
    for (int i = 0; colors[i].name != NULL; i++) {
        if (strcmp(colors[i].name, name) == 0) {
            return colors[i].hue;
        }
    }
    return 0; // Default to red
}

hsv_t get_hsv_by_name(const char *name) {
    uint8_t hue = get_hue_by_name(name);
    return (hsv_t){hue, 255, RGB_MATRIX_MAXIMUM_BRIGHTNESS / 2};
}

bool rgb_matrix_indicators_user(void) {
    static bool initialized = false;
    uint8_t     layer       = get_highest_layer(layer_state);

    hsv_t red    = get_hsv_by_name("red");
    hsv_t green  = get_hsv_by_name("green");
    hsv_t blue   = get_hsv_by_name("blue");
    hsv_t cyan   = get_hsv_by_name("cyan");
    hsv_t orange = get_hsv_by_name("orange");

    rgb_t rgb_red = hsv_to_rgb(red);

    if (!initialized) {
        base_layer_hue = get_hue_by_name("orange");
        initialized    = true;
    }

    hsv_t hsv;
    switch (layer) {
        case LAYER_BASE:
            hsv = get_hsv_by_name("purple");
            break;
        case LAYER_COLEMAK:
            hsv = orange;
            break;
        case LAYER_SYMBOLS:
            hsv = blue;
            break;
        case LAYER_POINTER:
            hsv = green;
            break;
        case LAYER_DAVINCI_RESOLVE:
            hsv = cyan;
            break;
        case LAYER_DANGER:
            hsv = red;
            break;
        default:
            hsv = blue;
            break;
    }

    hsv.v = RGB_MATRIX_MAXIMUM_BRIGHTNESS / 3;

    uint8_t arrow_key_indexes[] = {41, 46, 51, 56};
    uint8_t to_danger_key_index = 33; // TO_DANGER key position

    rgb_t rgb        = hsv_to_rgb(hsv);
    rgb_t rgb_orange = hsv_to_rgb(orange);

    for (int i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        if (layer == LAYER_SYMBOLS) {
            bool is_arrow_key = false;
            for (int j = 0; j < sizeof(arrow_key_indexes) / sizeof(arrow_key_indexes[0]); j++) {
                if (i == arrow_key_indexes[j]) {
                    rgb_matrix_set_color(i, rgb_red.r, rgb_red.g, rgb_red.b);
                    is_arrow_key = true;
                    break;
                }
            }
            if (!is_arrow_key) {
                if (i == to_danger_key_index) {
                    rgb_matrix_set_color(i, rgb_orange.r, rgb_orange.g, rgb_orange.b);
                } else {
                    rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
                }
            }
        } else {
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
    }

    return true;
}
