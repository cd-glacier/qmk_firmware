#include QMK_KEYBOARD_H
#include "bootloader.h"
#ifdef PROTOCOL_LUFA
#include "lufa.h"
#include "split_util.h"
#endif
#ifdef AUDIO_ENABLE
  #include "audio.h"
#endif
#ifdef SSD1306OLED
  #include "ssd1306.h"
#endif

#include "../../config.h"
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
    _BASE = 0,
    _NORMAL,
    _INSERT,
    _LOWER,
    _RAISE,
    _ADJUST,
};

enum custom_keycodes {
  BASE = SAFE_RANGE,
  NORMAL,
  INSERT,
  LOWER,
  RAISE,
  ADJUST,
  VIM_A,
  VIM_O,
};

enum tap_dance_keycodes {
  VIM_DD,
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

qk_tap_dance_action_t tap_dance_actions[] = {
  [VIM_DD]  =  ACTION_TAP_DANCE_FN (vim_dd)
};

enum macro_keycodes {
  KC_SAMPLEMACRO,
};

// Fillers to make layering more clear
#define _______ KC_TRNS
#define XXXXXXX KC_NO
//Macros
#define M_SAMPLE M(KC_SAMPLEMACRO)

#if HELIX_ROWS == 5
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
};

#elif HELIX_ROWS == 4

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  /* Normal
   * ,-----------------------------------------.             ,-----------------------------------------.
   * | Tab  |      |      |      |      |      |             |      |      |Insert|VIM_O |      | Bksp |
   * |------+------+------+------+------+------|             |------+------+------+-----+------+------|
   * | Ctrl |VIM_A |      |VIM_DD|      |      |             | Left | Down |  Up  |Right |      |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | Shift|      |      |      |      |      |             |      |      |      |      |      |      |
   * |------+------+------+------+------+------+-------------+------+------+------+------+------+------|
   * |      |      |  Alt |  GUI |      |Space |      |      |Enter |Raise | GUI  | Alt  |      |      |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_NORMAL] = LAYOUT( \
      KC_TAB,  _______, _______, _______, _______, _______,                    _______, _______, INSERT,  VIM_O,    _______, KC_BSPC,
      KC_LCTL, VIM_A,   _______, TD(VIM_DD),  _______, _______,                    KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT, _______, _______, \
      KC_LSFT, _______, _______, _______, _______, _______,                    _______, _______, _______, _______,  _______, _______, \
      _______, _______, KC_LALT, KC_LGUI, _______, KC_SPACE, _______, _______, KC_ENT,  RAISE,   KC_RGUI, KC_RALT,  _______, _______ \
      ),

  /* Insert
   * ,-----------------------------------------.             ,-----------------------------------------.
   * | Tab  |   Q  |   W  |   E  |   R  |   T  |             |   Y  |   U  |   I  |   O  |   P  | Bksp |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | Ctrl |   A  |   S  |   D  |   F  |   G  |             |   H  |   J  |   K  |   L  |   ;  |  '   |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | Shift|   Z  |   X  |   C  |   V  |   B  |             |   N  |   M  |   ,  |   .  |   /  |  -   |
   * |------+------+------+------+------+------+-------------+------+------+------+------+------+------|
   * |Normal|   _  |  ALt |  GUI | Lower| Space|      |      | Enter| Raise|  GUI |  Alt | xxxx |  =   |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_INSERT] = LAYOUT( \
      KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                        KC_Y,    KC_U,  KC_I,    KC_O,    KC_P,    KC_BSPC, \
      KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                        KC_H,    KC_J,  KC_K,    KC_L,    KC_SCLN, KC_QUOT, \
      KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                        KC_N,    KC_M,  KC_COMM, KC_DOT,  KC_SLSH, KC_MINS , \
      NORMAL,  KC_UNDS, KC_LALT, KC_LGUI, LOWER,   KC_SPACE, _______, _______,  KC_ENT,  RAISE, KC_RGUI, KC_RALT, _______, KC_EQL\
      ),

  /* Lower
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |      |      |   1  |   2  |   3  |      |             |   (  |  )   |  [   |   ]  |   {  |  }   |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |      |   4  |   5  |   6  |      |             | Left | Down |  Up  |Right |      |  |   |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |      |   7  |   8  |   9  |   0  |             |      |      |      |      |      |      |
   * |------+------+------+------+------+------+-------------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_LOWER] = LAYOUT( \
      _______, _______, KC_1,    KC_2,    KC_3,    _______,                   KC_LPRN, KC_RPRN, KC_LBRC, KC_RBRC,  KC_LCBR, KC_RCBR, \
      _______, _______, KC_4,    KC_5,    KC_6,    _______,                   KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT, _______, KC_PIPE, \
      _______, _______, KC_7,    KC_8,    KC_9,    KC_0,                      _______, _______, _______, _______,  _______, _______, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______, _______ \
      ),

  /* Raise
   * ,-----------------------------------------.             ,-----------------------------------------.
   * | Ecs  |      |      |      |      |      |             |   ~  |   !  |   @  |   #  |   $  | Bksp |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |      | COPY | CUT  |PASTE |      |             |   %  |   ^  |   &  |   *  |      |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |      |      |      |      |      |             |   `  |   \  |      |      |      |      |
   * |------+------+------+------+------+------+-------------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_RAISE] = LAYOUT( \
      KC_ESC,  _______, _______, _______, _______, _______,                   KC_TILD, KC_EXLM, KC_AT,    KC_HASH, KC_DLR,  KC_BSPC, \
      _______, _______, M(0),    M(1),    M(2),    _______,                   KC_PERC, KC_CIRC, KC_AMPR,  KC_ASTR, _______, _______, \
      _______, _______, _______, _______, _______, _______,                   KC_GRV,  KC_BSLS, _______,  _______, _______, _______, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______, _______, _______ \
      ),

  /* Adjust (Lower + Raise)
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |      | Reset|      |      |      |      |             |      |      |      |      |      |  Del |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |      |      |Aud on|Audoff| Mac  |             | Win  |      |      |      |      |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |      |      |      |      |      |             |      |      |RGB ON| HUE+ | SAT+ | VAL+ |
   * |------+------+------+------+------+------+-------------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |      |      |      |      | MODE | HUE- | SAT- | VAL- |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_ADJUST] =  LAYOUT( \
      _______, RESET,   _______, _______, _______, _______,                   _______, _______, _______, _______, _______, KC_DEL, \
      _______, _______, _______, AU_ON,   AU_OFF,  AG_NORM,                   AG_SWAP, _______,  _______, _______,  _______, _______, \
      _______, _______, _______, _______, _______, _______,                   _______, _______, RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RGB_SMOD,RGB_HUD, RGB_SAD, RGB_VAD \
      )
};

#else
#error "undefined keymaps"
#endif

// define variables for reactive RGB
bool TOG_STATUS = false;
int RGB_current_mode;

void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
}

// Setting ADJUST layer RGB back to default
void update_tri_layer_RGB(uint8_t layer1, uint8_t layer2, uint8_t layer3) {
  if (IS_LAYER_ON(layer1) && IS_LAYER_ON(layer2)) {
    #ifdef RGBLIGHT_ENABLE
      //rgblight_mode(RGB_current_mode);
    #endif
    layer_on(layer3);
  } else {
    layer_off(layer3);
  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case NORMAL:
      if (record->event.pressed) {
        layer_off(_INSERT);
        layer_on(_NORMAL);
      }
      return false;
      break;
    case INSERT:
      if (record->event.pressed) {
        layer_off(_NORMAL);
        layer_on(_INSERT);
      }
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
        layer_off(_INSERT);
        layer_on(_LOWER);
      } else {
        layer_off(_LOWER);
        layer_on(_INSERT);
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        layer_off(_INSERT);
        layer_on(_RAISE);
      } else {
        layer_off(_RAISE);
        layer_on(_INSERT);
      }
      return false;
      break;
    case ADJUST:
        if (record->event.pressed) {
          layer_on(_ADJUST);
        } else {
          layer_off(_ADJUST);
        }
        return false;
        break;
    case VIM_A:
        if (record->event.pressed) {
          SEND_STRING (SS_TAP(X_RIGHT));
          layer_off(_NORMAL);
          layer_on(_INSERT);
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
  }
  return true;
}

void matrix_init_user(void) {
    #ifdef AUDIO_ENABLE
        startup_user();
    #endif
    #ifdef RGBLIGHT_ENABLE
      RGB_current_mode = rgblight_config.mode;
    #endif
    //SSD1306 OLED init, make sure to add #define SSD1306OLED in config.h
    #ifdef SSD1306OLED
        iota_gfx_init(!has_usb());   // turns on the display
    #endif
}


#ifdef AUDIO_ENABLE

void startup_user()
{
    _delay_ms(20); // gets rid of tick
}

void shutdown_user()
{
    _delay_ms(150);
    stop_all_notes();
}

void music_on_user(void)
{
    music_scale_user();
}

void music_scale_user(void)
{
    PLAY_SONG(music_scale);
}

#endif


//SSD1306 OLED update loop, make sure to add #define SSD1306OLED in config.h
#ifdef SSD1306OLED

// hook point for 'led_test' keymap
//   'default' keymap's led_test_init() is empty function, do nothing
//   'led_test' keymap's led_test_init() force rgblight_mode_noeeprom(RGBLIGHT_MODE_RGB_TEST);
__attribute__ ((weak))
void led_test_init(void) {}

void matrix_scan_user(void) {
     led_test_init();
     iota_gfx_task();  // this is what updates the display continuously
}

void matrix_update(struct CharacterMatrix *dest,
                          const struct CharacterMatrix *source) {
  if (memcmp(dest->display, source->display, sizeof(dest->display))) {
    memcpy(dest->display, source->display, sizeof(dest->display));
    dest->dirty = true;
  }
}

//assign the right code to your layers for OLED display
#define L_BASE 0
#define L_NORMAL (1<<_NORMAL)
#define L_INSERT (1<<_INSERT)
#define L_LOWER (1<<_LOWER)
#define L_RAISE (1<<_RAISE)
#define L_ADJUST (1<<_ADJUST)
#define L_ADJUST_TRI (L_ADJUST|L_RAISE|L_INSERT)

static void render_logo(struct CharacterMatrix *matrix) {

  static char logo[]={
    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
    0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
    0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,
    0};
  matrix_write(matrix, logo);
  //matrix_write_P(&matrix, PSTR(" Split keyboard kit"));
}



void render_status(struct CharacterMatrix *matrix) {

  // Render to mode icon
  static char logo[][2][3]={{{0x95,0x96,0},{0xb5,0xb6,0}},{{0x97,0x98,0},{0xb7,0xb8,0}}};
  if(keymap_config.swap_lalt_lgui==false){
    matrix_write(matrix, logo[0][0]);
    matrix_write_P(matrix, PSTR("\n"));
    matrix_write(matrix, logo[0][1]);
  }else{
    matrix_write(matrix, logo[1][0]);
    matrix_write_P(matrix, PSTR("\n"));
    matrix_write(matrix, logo[1][1]);
  }

  // Define layers here, Have not worked out how to have text displayed for each layer. Copy down the number you see and add a case for it below
  char buf[40];
  snprintf(buf,sizeof(buf), "Undef-%ld", layer_state);
  matrix_write_P(matrix, PSTR("\nLayer: "));
    switch (layer_state) {
        case L_BASE:
           matrix_write_P(matrix, PSTR("Normal"));
           break;
        case L_NORMAL:
           matrix_write_P(matrix, PSTR("Normal"));
           break;
        case L_INSERT:
           matrix_write_P(matrix, PSTR("Insert"));
           break;
        case L_LOWER:
           matrix_write_P(matrix, PSTR("Lower"));
           break;
        case L_RAISE:
           matrix_write_P(matrix, PSTR("Raise"));
           break;
        case L_ADJUST:
        case L_ADJUST_TRI:
           matrix_write_P(matrix, PSTR("Adjust"));
           break;
        default:
           matrix_write(matrix, buf);
    }

  // Host Keyboard LED Status
  char led[40];
    snprintf(led, sizeof(led), "\n%s  %s  %s",
            (host_keyboard_leds() & (1<<USB_LED_NUM_LOCK)) ? "NUMLOCK" : "       ",
            (host_keyboard_leds() & (1<<USB_LED_CAPS_LOCK)) ? "CAPS" : "    ",
            (host_keyboard_leds() & (1<<USB_LED_SCROLL_LOCK)) ? "SCLK" : "    ");
  matrix_write(matrix, led);
}


void iota_gfx_task_user(void) {
  struct CharacterMatrix matrix;

#if DEBUG_TO_SCREEN
  if (debug_enable) {
    return;
  }
#endif

  matrix_clear(&matrix);
  if(is_master){
    render_status(&matrix);
  }else{
    render_logo(&matrix);
  }
  matrix_update(&display, &matrix);
}

#endif
