/* Copied from via/keymap.c */

#include QMK_KEYBOARD_H

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  /* Keymap _BL: Base Layer (Default Layer)
   */
[0] = LAYOUT(
  QK_GESC,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,  KC_DEL,
  KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_PGUP,
  KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,  KC_ENT,             KC_PGDN,
  KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_RSFT,  KC_UP,    KC_END,
  KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 KC_RALT,  MO(1),    KC_LEFT,  KC_DOWN,  KC_RGHT),

  /* Keymap _FL: Function Layer
   */
[1] = LAYOUT(
  KC_GRV,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,    _______,  KC_INS,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_PSCR,  KC_SCRL,  KC_PAUS,   _______,  _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,             _______,
  _______,  _______,  RGB_HUI,  RGB_HUD,  RGB_SPD,  RGB_SPI,  KC_MUTE,  KC_VOLU,  KC_VOLD,  KC_MPRV,  KC_MPLY,  KC_MNXT,  _______,   RGB_VAI,  KC_HOME,
  _______,  _______,  _______,                                QK_BOOT,                                _______,  _______,  RGB_RMOD,  RGB_VAD,  RGB_MOD),

[2] = LAYOUT(
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,             _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,
  _______,  _______,  _______,                                _______,                                _______,  _______,  _______,   _______,  _______),
[3] = LAYOUT(
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,             _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,
  _______,  _______,  _______,                                _______,                                _______,  _______,  _______,   _______,  _______)
};

#define CAPS_LOCK_INDEX 30
#define SCROLL_LOCK_INDEX 26
#define NUM_LOCK_INDEX 26
static bool blinker_on = false;

void process_blink_timer(void);
void indicate_caps_lock(void);
void indicate_scroll_lock(void);
void indicate_num_lock(void);
void set_rgb_matrix_lock_indicator(uint8_t index);
void indicate_layers_active(uint8_t led_min, uint8_t led_max);
void indicate_set_layer_color(int index, uint8_t layer);

void housekeeping_task_user(void) {
  process_blink_timer();
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
  indicate_num_lock();
  indicate_scroll_lock();
  indicate_caps_lock();
  indicate_layers_active(led_min, led_max);

  return false;
}

void process_blink_timer(void) {
  uint16_t time = timer_read();

  if (((time >> 10) & 1) == 1) {
    blinker_on = true;
  } else {
    blinker_on = false;
  }
}

void indicate_caps_lock(void) {
  if (host_keyboard_led_state().caps_lock
  && g_led_config.flags[CAPS_LOCK_INDEX] & LED_FLAG_KEYLIGHT) {
    set_rgb_matrix_lock_indicator(CAPS_LOCK_INDEX);
  }
}

void indicate_scroll_lock(void) {
  if (host_keyboard_led_state().scroll_lock
  && g_led_config.flags[SCROLL_LOCK_INDEX] & LED_FLAG_KEYLIGHT
  && get_highest_layer(layer_state) == 1) {
    set_rgb_matrix_lock_indicator(SCROLL_LOCK_INDEX);
  }
}

void indicate_num_lock(void) {
  if (host_keyboard_led_state().num_lock
  && g_led_config.flags[NUM_LOCK_INDEX] & LED_FLAG_KEYLIGHT
  && get_highest_layer(layer_state) == 2) {
    set_rgb_matrix_lock_indicator(NUM_LOCK_INDEX);
  }
}

void set_rgb_matrix_lock_indicator(uint8_t index) {
  if (blinker_on) {
    rgb_matrix_set_color(index, RGB_GREEN);
  }
}

void indicate_layers_active(uint8_t led_min, uint8_t led_max) {
  uint8_t layer = get_highest_layer(layer_state);

  for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
    for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
      uint8_t index = g_led_config.matrix_co[row][col];

      if (index >= led_min && index < led_max
      && index != NO_LED
      && keymap_key_to_keycode(layer, (keypos_t){col,row}) > KC_TRNS) {
        indicate_set_layer_color(index, layer);
      }
    }
  }
}

void indicate_set_layer_color(int index, uint8_t layer) {
  HSV color = {
    rgb_matrix_get_hue() + 0x7f,
    rgb_matrix_get_sat(),
    rgb_matrix_get_val()
  };

  switch (layer) {
    case 0:
      return;
    case 1:
      color.s = 255;
      break;
    case 2:
      color.h -= 10;
      color.s = 255;
      break;
    case 3:
      color.h -= 20;
      color.s = 255;
      break;
    default:
      return;
  }

  uint8_t current_val = rgb_matrix_get_val();
  color.v = MIN(color.v, current_val);

  RGB rgb_color = hsv_to_rgb(color);
  rgb_matrix_set_color(index, rgb_color.r, rgb_color.g, rgb_color.b);
}
