#include QMK_KEYBOARD_H
#include "bootloader.h"
#ifdef PROTOCOL_LUFA
  #include "lufa.h"
  #include "split_util.h"
#endif
#ifdef SSD1306OLED
  #include "ssd1306.h"
#endif

#include "./config.h"
#include "quantum.h"
#include "action.h"
#include "process_keycode/process_tap_dance.h"

extern keymap_config_t keymap_config;

#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
#endif

extern uint8_t is_master;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum layer_number {
    _INSERT = 0,
    _NORMAL,
    _VISUAL,
    _LOWER,
    _RAISE,
    _ADJUST,
};
enum custom_keycodes {
  BASE = SAFE_RANGE,
  INSERT,
  NORMAL,
  VISUAL,

  LOWER,
  RAISE,
  ADJUST,

  VIM_A,
  VIM_B,
  VIM_E,
  VIM_O,
  VIM_P,
  VIM_U,
  VIM_W,
  VISUAL_CUT,
  VISUAL_YANK,

  BACKLIT,
  RGBRST
};

enum tap_dance_keycodes {
  VIM_DD,
  VIM_YY,
  VIM_ZZ,
};

void vim_dd (qk_tap_dance_state_t *state, void *user_data) {
  if (state->count >= 2) {
    SEND_STRING(SS_LGUI(SS_TAP(X_LEFT)));
    SEND_STRING(SS_DOWN(X_LSHIFT));
    SEND_STRING(SS_LGUI(SS_TAP(X_RIGHT)));
    SEND_STRING(SS_UP(X_LSHIFT));
    SEND_STRING(SS_LGUI("x"));
    SEND_STRING(SS_TAP(X_DELETE));
    reset_tap_dance (state);
  }
}

void vim_yy (qk_tap_dance_state_t *state, void *user_data) {
  if (state->count >= 2) {
    SEND_STRING(SS_LGUI(SS_TAP(X_LEFT)));
    SEND_STRING(SS_DOWN(X_LSHIFT));
    SEND_STRING(SS_LGUI(SS_TAP(X_RIGHT)));
    SEND_STRING(SS_TAP(X_DOWN));
    SEND_STRING(SS_UP(X_LSHIFT));
    SEND_STRING(SS_LGUI("c"));
    reset_tap_dance (state);
  } else {
    SEND_STRING(SS_LGUI("c"));
  }
}

void vim_zz (qk_tap_dance_state_t *state, void *user_data) {
  if (state->count >= 2) {
    SEND_STRING(SS_LCTRL(SS_TAP(X_L)));
    reset_tap_dance (state);
  } else {
  }
}

qk_tap_dance_action_t tap_dance_actions[] = {
  [VIM_DD]  =  ACTION_TAP_DANCE_FN (vim_dd),
  [VIM_YY]  =  ACTION_TAP_DANCE_FN (vim_yy),
  [VIM_ZZ]  =  ACTION_TAP_DANCE_FN (vim_zz),
};

#define KC______ KC_TRNS
#define KC_XXXXX KC_NO
#define KC_LOWER LOWER
#define KC_RAISE RAISE
#define KC_RST   RESET
#define KC_LRST  RGBRST
#define KC_LTOG  RGB_TOG
#define KC_LHUI  RGB_HUI
#define KC_LHUD  RGB_HUD
#define KC_LSAI  RGB_SAI
#define KC_LSAD  RGB_SAD
#define KC_LVAI  RGB_VAI
#define KC_LVAD  RGB_VAD
#define KC_LSMOD RGB_SMOD
#define KC_CTLTB CTL_T(KC_TAB)
#define KC_GUIEI GUI_T(KC_LANG2)
#define KC_ALTKN ALT_T(KC_LANG1)

#define KC_NORMAL NORMAL
#define KC_INSERT INSERT
#define KC_VISUAL VISUAL
#define KC_VIM_A VIM_A
#define KC_VIM_B VIM_B
#define KC_VIM_E VIM_E
#define KC_VIM_O VIM_O
#define KC_VIM_P VIM_P
#define KC_VIM_U VIM_U
#define KC_VIM_W VIM_W
#define KC_V_CUT VISUAL_CUT
#define KC_V_YANK VISUAL_YANK
#define KC_VIM_DD TD(VIM_DD)
#define KC_VIM_YY TD(VIM_YY)
#define KC_VIM_ZZ TD(VIM_ZZ)
#define KC_SC_MAX LGUI(LALT(KC_UP))
#define KC_SC_LEFT LGUI(LALT(KC_LEFT))
#define KC_SC_RIGHT LGUI(LALT(KC_RIGHT))
#define KC_DEV_TOOL LGUI(LALT(KC_I))
#define KC_CSF LGUI(LSFT(KC_F))

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_INSERT] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
        TAB,     Q,     W,     E,     R,     T,                      Y,     U,     I,     O,     P,  BSPC,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       LCTL,     A,     S,     D,     F,     G,                      H,     J,     K,     L,  SCLN,  QUOT,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       LSFT,     Z,     X,     C,     V,     B,                      N,     M,  COMM,   DOT,  SLSH,  MINS,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                 NORMAL, LOWER,   SPC,      ENT, RAISE,  RGUI\
                              //`--------------------'  `--------------------'
  ),

  [_NORMAL] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
        TAB,  TRNS, VIM_W, VIM_E,  TRNS,  TRNS,                 VIM_YY, VIM_U,INSERT, VIM_O, VIM_P,  BSPC,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       LCTL, VIM_A,  TRNS,VIM_DD,  TRNS,  TRNS,                   LEFT,  DOWN,    UP, RIGHT,  TRNS,  TRNS,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       LSFT,VIM_ZZ,   DEL,  TRNS,VISUAL, VIM_B,                   TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                   LALT, LOWER,   SPC,      ENT, RAISE,  RGUI\
                              //`--------------------'  `--------------------'
  ),

  [_VISUAL] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
       TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,                 V_YANK,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       TRNS,  TRNS,  TRNS, V_CUT,  TRNS,  TRNS,                   LEFT,  DOWN,    UP, RIGHT,  TRNS,  TRNS,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       TRNS,  TRNS,  TRNS,  TRNS,NORMAL,  TRNS,                   TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                 NORMAL, LOWER,   SPC,      ENT, RAISE,  RGUI\
                              //`--------------------'  `--------------------'
  ),

  [_LOWER] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
       TRNS,  TRNS,     1,     2,     3,  PLUS,                   LPRN,  RPRN,  LBRC,  RBRC,  LCBR,  RCBR,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       TRNS,  TRNS,     4,     5,     6,   EQL,                   LEFT,  DOWN,    UP, RIGHT,  TRNS,  PIPE,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       TRNS,  TRNS,     7,     8,     9,     0,                   TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                   LALT, LOWER,   SPC,     ENT,  RAISE,  RGUI\
                              //`--------------------'  `--------------------'
  ),

  [_RAISE] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
        ESC,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,                   TILD,  EXLM,    AT,  HASH,   DLR,  BSPC,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,                   PERC,  CIRC,  AMPR,  ASTR,  TRNS,  TRNS,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,                    GRV,  BSLS,  TRNS,  TRNS,  TRNS,  TRNS,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                   LALT, LOWER,   SPC,      ENT, RAISE,  RGUI\
                              //`--------------------'  `--------------------'
  ),

  [_ADJUST] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
      XXXXX, SLCK,  PAUS,_MUTE,_VOLDOWN,_VOLUP,                  XXXXX,XXXXX,DEV_TOOL,XXXXX, XXXXX, XXXXX,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
      XXXXX, XXXXX, XXXXX, XXXXX,   CSF, XXXXX,                  XXXXX,PGDOWN,  PGUP, XXXXX, XXXXX, XXXXX,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
      XXXXX, XXXXX, XXXXX, XXXXX, XXXXX, XXXXX,                SC_LEFT,XXXXX,SC_MAX,SC_RIGHT,XXXXX, XXXXX,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                   LALT, LOWER,   SPC,      ENT, RAISE,  RGUI\
                              //`--------------------'  `--------------------'
  )
};

int RGB_current_mode;

void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
}

// Setting ADJUST layer RGB back to default
void update_tri_layer_RGB(uint8_t layer1, uint8_t layer2, uint8_t layer3) {
  if (IS_LAYER_ON(layer1) && IS_LAYER_ON(layer2)) {
    layer_on(layer3);
  } else {
    layer_off(layer3);
  }
}

void matrix_init_user(void) {
    #ifdef RGBLIGHT_ENABLE
      RGB_current_mode = rgblight_config.mode;
    #endif
    //SSD1306 OLED init, make sure to add #define SSD1306OLED in config.h
    #ifdef SSD1306OLED
        iota_gfx_init(!has_usb());   // turns on the display
    #endif
}

//SSD1306 OLED update loop, make sure to add #define SSD1306OLED in config.h
#ifdef SSD1306OLED

// When add source files to SRC in rules.mk, you can use functions.
const char *read_layer_state(void);
const char *read_logo(void);
void set_keylog(uint16_t keycode, keyrecord_t *record);
const char *read_keylog(void);
const char *read_keylogs(void);

// const char *read_mode_icon(bool swap);
// const char *read_host_led_state(void);
// void set_timelog(void);
// const char *read_timelog(void);

void matrix_scan_user(void) {
   iota_gfx_task();
}

#define L_BASE 0
#define L_NORMAL (1<<_NORMAL)
#define L_INSERT (1<<_INSERT)
#define L_VISUAL (1<<_VISUAL)
#define L_LOWER (1<<_LOWER)
#define L_RAISE (1<<_RAISE)
#define L_ADJUST (1<<_ADJUST)
#define L_ADJUST_TRI (L_ADJUST|L_RAISE|L_LOWER)

char layer_state_str[24];

const char *read_layer_state(void) {
  switch (layer_state)
  {
  case L_BASE:
    snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Insert");
    break;
  case L_RAISE:
    snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Raise");
    break;
  case L_LOWER:
    snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Lower");
    break;
  case L_ADJUST:
  case L_ADJUST_TRI:
    snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Adjust");
    break;
  case L_INSERT:
    snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Insert");
    break;
  case L_NORMAL:
    snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Normal");
    break;
  case L_VISUAL:
    snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Visual");
    break;

  default:
    snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Undef-%ld", layer_state);
  }

  return layer_state_str;
}

void matrix_render_user(struct CharacterMatrix *matrix) {
  if (is_master) {
    // If you want to change the display of OLED, you need to change here
    matrix_write_ln(matrix, read_layer_state());
    matrix_write_ln(matrix, read_keylog());
    matrix_write_ln(matrix, read_keylogs());
    //matrix_write_ln(matrix, read_mode_icon(keymap_config.swap_lalt_lgui));
    //matrix_write_ln(matrix, read_host_led_state());
    //matrix_write_ln(matrix, read_timelog());
  } else {
    matrix_write(matrix, read_logo());
  }
}

void matrix_update(struct CharacterMatrix *dest, const struct CharacterMatrix *source) {
  if (memcmp(dest->display, source->display, sizeof(dest->display))) {
    memcpy(dest->display, source->display, sizeof(dest->display));
    dest->dirty = true;
  }
}

void iota_gfx_task_user(void) {
  struct CharacterMatrix matrix;
  matrix_clear(&matrix);
  matrix_render_user(&matrix);
  matrix_update(&display, &matrix);
}
#endif//SSD1306OLED

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
#ifdef SSD1306OLED
    set_keylog(keycode, record);
#endif
    // set_timelog();
  }

  switch (keycode) {
    case INSERT:
      if (record->event.pressed) {
        layer_off(_NORMAL);
        layer_off(_VISUAL);
        layer_on(_INSERT);
        persistent_default_layer_set(1UL<<_INSERT);
      }
      return false;
      break;
    case NORMAL:
      if (record->event.pressed) {
        layer_off(_INSERT);
        layer_off(_VISUAL);
        SEND_STRING(SS_UP(X_LSHIFT));
        layer_on(_NORMAL);
      }
      return false;
      break;
    case VISUAL:
      if (record->event.pressed) {
        layer_off(_INSERT);
        layer_off(_NORMAL);
        layer_on(_VISUAL);
        SEND_STRING(SS_DOWN(X_LSHIFT));
      }
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
        layer_off(_INSERT);
        layer_off(_NORMAL);
        layer_off(_VISUAL);
        layer_on(_LOWER);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LOWER);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        layer_off(_INSERT);
        layer_off(_NORMAL);
        layer_off(_VISUAL);
        layer_on(_RAISE);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case ADJUST:
      if (record->event.pressed) {
        layer_off(_INSERT);
        layer_off(_NORMAL);
        layer_off(_VISUAL);
        layer_on(_ADJUST);
      } else {
        layer_off(_ADJUST);
      }
      return false;
      break;
    case RGB_MOD:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          rgblight_mode(RGB_current_mode);
          rgblight_step();
          RGB_current_mode = rgblight_config.mode;
        }
      #endif
      return false;
      break;
    case RGBRST:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          eeconfig_update_rgblight_default();
          rgblight_enable();
          RGB_current_mode = rgblight_config.mode;
        }
      #endif
      break;
    case VIM_A:
        if (record->event.pressed) {
          SEND_STRING(SS_TAP(X_RIGHT));
          layer_off(_NORMAL);
          layer_on(_INSERT);
        }
        return false;
        break;
    case VIM_B:
        if (record->event.pressed) {
          SEND_STRING(SS_LALT(SS_TAP(X_LEFT)));
        }
        return false;
        break;
    case VIM_E:
        if (record->event.pressed) {
          SEND_STRING(SS_LALT(SS_TAP(X_RIGHT)));
        }
        return false;
        break;
    case VIM_O:
        if (record->event.pressed) {
          SEND_STRING(SS_LGUI(SS_TAP(X_RIGHT)));
          SEND_STRING(SS_TAP(X_ENTER));
          layer_off(_NORMAL);
          layer_on(_INSERT);
          return false;
        }
        break;
    case VIM_P:
        if (record->event.pressed) {
          SEND_STRING(SS_LGUI("v"));
          return false;
        }
        break;
    case VIM_U:
        if (record->event.pressed) {
          SEND_STRING(SS_LGUI("z"));
          return false;
        }
        break;
    case VIM_W:
        if (record->event.pressed) {
          SEND_STRING(SS_LALT(SS_TAP(X_RIGHT)));
          SEND_STRING(SS_TAP(X_RIGHT));
        }
        return false;
        break;
    case VISUAL_CUT:
        if (record->event.pressed) {
          SEND_STRING(SS_UP(X_LSHIFT));
          SEND_STRING(SS_LGUI("x"));
          layer_off(_VISUAL);
          layer_on(_NORMAL);
          return false;
        }
        break;
    case VISUAL_YANK:
        if (record->event.pressed) {
          SEND_STRING(SS_UP(X_LSHIFT));
          SEND_STRING(SS_LGUI("c"));
          SEND_STRING(SS_TAP(X_RIGHT));
          layer_off(_VISUAL);
          layer_on(_NORMAL);
          return false;
        }
        break;
  }
  return true;
}

