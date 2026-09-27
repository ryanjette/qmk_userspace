
#include QMK_KEYBOARD_H
#include "oneshot.h"
// 1. this comma separated list defines how many combos will exist in the firmware
// keep this list 1:1 with the consts below
// the enums here will be written in CAPS to differentiate them from the consts in 2
#define U_UND (LCTL(KC_Z))
#define U_CUT (LCTL(KC_X))
#define U_CPY (LCTL(KC_C))
#define U_PST (LCTL(KC_V))
#define U_RDO (LCTL(KC_Y))

enum layers {
  NAV = 1,
  NUM = 4
};

enum custom_keycodes {
    OS_SHFT = SAFE_RANGE,
    OS_CTRL,
    OS_ALT,
    OS_CMD
};

enum combo_events {
    C_DEL,
    C_ENTER,
    C_V,
    C_SCLN,
    C_W,
    C_COMM,
    C_B,
    C_Q,
    COMBO_LENGTH
};
// the point of the list is to define this variable which QMK uses to define how many combos will exist
uint16_t COMBO_LEN = COMBO_LENGTH;

// 2. these consts define the keys that make up the combo - their names matter because you refer to them in 3
// the syntax here will be lower case to differentiate them from the enums in 1
const uint16_t PROGMEM c_del_combo[] = {KC_D, KC_C, COMBO_END}; // Send Delete
const uint16_t PROGMEM c_enter_combo[] = {KC_N, KC_C, COMBO_END}; // Send Enter
const uint16_t PROGMEM c_v_combo[] = {KC_M, KC_P, COMBO_END};
const uint16_t PROGMEM c_scln_combo[] = {KC_DOT, KC_SLSH, COMBO_END};
const uint16_t PROGMEM c_w_combo[] = {KC_N, KC_D, COMBO_END};
const uint16_t PROGMEM c_comm_combo[] = {KC_A, KC_E, COMBO_END};
const uint16_t PROGMEM c_b_combo[] = {KC_L, KC_C, COMBO_END};
const uint16_t PROGMEM c_q_combo[] = {LT(5, KC_U), KC_O, COMBO_END};

// 3. This list maps each combo to the keycode it sends.
combo_t key_combos[] = {
    [C_DEL] = COMBO(c_del_combo, KC_DEL),
    [C_ENTER] = COMBO(c_enter_combo, KC_ENT),
    [C_V] = COMBO(c_v_combo, KC_V),
    [C_SCLN] = COMBO(c_scln_combo, KC_SCLN),
    [C_W] = COMBO(c_w_combo, KC_W),
    [C_COMM] = COMBO(c_comm_combo, KC_COMM),
    [C_B] = COMBO(c_b_combo, KC_B),
    [C_Q] = COMBO(c_q_combo, KC_Q)

};

bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
  (void)combo;
  (void)keycode;
  (void)record;

  return get_highest_layer(layer_state) == 0;
}

static oneshot_state os_shft_state = os_up_unqueued;
static oneshot_state os_ctrl_state = os_up_unqueued;
static oneshot_state os_alt_state = os_up_unqueued;
static oneshot_state os_cmd_state = os_up_unqueued;

bool is_oneshot_cancel_key(uint16_t keycode) {
  return keycode == MO(NAV) || keycode == MO(NUM) || keycode == TO(8);
}

bool is_oneshot_ignored_key(uint16_t keycode) {
  switch (keycode) {
    case MO(NAV):
    case MO(NUM):
    case TO(8):
    case KC_LSFT:
    case KC_LCTL:
    case KC_LALT:
    case KC_LGUI:
    case OS_SHFT:
    case OS_CTRL:
    case OS_ALT:
    case OS_CMD:
      return true;
    default:
      return false;
  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  update_oneshot(&os_shft_state, KC_LSFT, OS_SHFT, keycode, record);
  update_oneshot(&os_ctrl_state, KC_LCTL, OS_CTRL, keycode, record);
  update_oneshot(&os_alt_state, KC_LALT, OS_ALT, keycode, record);
  update_oneshot(&os_cmd_state, KC_LGUI, OS_CMD, keycode, record);
  return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_split_3x6_3(
    KC_NO,     KC_J,           KC_F,           KC_M,             KC_P,           KC_NO,              KC_NO,           KC_DOT,          KC_SLSH,        KC_QUOTE,       KC_Z,           KC_NO,
    KC_NO,     KC_R,           KC_S,           KC_N,             KC_D,           KC_NO,              KC_NO,           KC_A,            KC_E,           KC_I,           KC_H,           KC_NO,
    KC_NO,     LT(7, KC_X),    KC_G,           KC_L,             KC_C,           KC_NO,              KC_NO,           LT(5, KC_U),     KC_O,           KC_Y,           LT(6, KC_K),    KC_NO,
                                               KC_NO,            KC_T,           MO(NAV),            MO(NUM),         KC_SPC,          TO(8)

  ),

  [NAV] = LAYOUT_split_3x6_3(
    KC_NO,     KC_NO,          U_UND,          U_CPY,            U_PST,          U_CUT,              U_CUT,           U_PST,           U_CPY,          U_UND,          U_RDO,          KC_NO,
    KC_NO,     OS_CMD,        OS_ALT,        OS_CTRL,          OS_SHFT,        KC_NO,              KC_CAPS,         KC_LEFT,         KC_DOWN,        KC_UP,          KC_RGHT,        KC_NO,
    KC_NO,     KC_NO,          KC_NO,          KC_NO,            KC_NO,          KC_NO,              KC_INS,          KC_HOME,         KC_PGDN,        KC_PGUP,        KC_END,         KC_NO,
                                               KC_NO,            KC_NO,          KC_NO,              KC_BSPC,         KC_DEL,          KC_NO
  ),

  // Right hand
  [NUM] = LAYOUT_split_3x6_3(
    KC_NO,     KC_LBRC,        KC_7,           KC_8,             KC_9,           KC_RBRC,            KC_NO,           KC_NO,           KC_NO,          KC_NO,          KC_NO,          KC_NO,
    KC_NO,     KC_SCLN,        KC_4,           KC_5,             KC_6,           KC_EQL,             KC_NO,           OS_SHFT,         OS_CTRL,        OS_ALT,        OS_CMD,        KC_NO,
    KC_NO,     KC_GRV,         KC_1,           KC_2,             KC_3,           KC_BSLS,            KC_NO,           KC_NO,           KC_NO,          KC_ALGR,        KC_NO,          KC_NO,
                                               KC_DOT,           KC_0,           KC_MINS,            KC_NO,           KC_NO,           KC_NO
  ),

  [5] = LAYOUT_split_3x6_3(
    KC_NO,     KC_LCBR,        KC_AMPR,        KC_ASTR,          KC_LPRN,        KC_RCBR,            KC_NO,           KC_NO,           KC_NO,          KC_NO,          KC_NO,          KC_NO,
    KC_NO,     KC_COLN,        KC_DLR,         KC_PERC,          KC_CIRC,        KC_PLUS,            KC_NO,           KC_LSFT,         KC_LCTL,        KC_LALT,        KC_LGUI,        KC_NO,
    KC_NO,     KC_TILD,        KC_EXLM,        KC_AT,            KC_HASH,        KC_PIPE,            KC_NO,           KC_NO,           KC_NO,          KC_ALGR,        KC_NO,          KC_NO,
                                               KC_LPRN,          KC_RPRN,        KC_UNDS,            KC_ENT,          KC_NO,           KC_NO
  ),

  [6] = LAYOUT_split_3x6_3(
    KC_NO,     KC_F12,         KC_F7,          KC_F8,            KC_F9,          KC_PSCR,            KC_NO,           KC_NO,           KC_NO,          KC_NO,          KC_NO,          KC_NO,
    KC_NO,     KC_F11,         KC_F4,          KC_F5,            KC_F6,          KC_NO,              KC_NO,           KC_LSFT,         KC_LCTL,        KC_LALT,        KC_LGUI,        KC_NO,
    KC_NO,     KC_F10,         KC_F1,          KC_F2,            KC_F3,          KC_PAUS,            KC_NO,           KC_NO,           KC_NO,          KC_ALGR,        KC_NO,          KC_NO,
                                               KC_APP,           KC_SPC,         KC_TAB,             KC_NO,           KC_NO,           KC_NO
  ),

  [7] = LAYOUT_split_3x6_3( // Button
    KC_NO,     KC_NO,          U_UND,          U_CPY,            U_PST,          U_CUT,              U_CUT,           U_PST,           U_CPY,          U_UND,          U_RDO,          KC_NO,
    KC_NO,     KC_LGUI,        KC_LALT,        KC_LCTL,          KC_LSFT,        KC_NO,              KC_CAPS,         KC_LEFT,         KC_DOWN,        KC_UP,          KC_RGHT,        KC_NO,
    KC_NO,     KC_NO,          KC_NO,          KC_NO,            KC_ESC,         KC_NO,              KC_INS,          KC_HOME,         KC_PGDN,        KC_PGUP,        KC_END,         KC_NO,
                                               KC_NO,            KC_ESC,         KC_ENT,             KC_BSPC,         KC_DEL,          KC_NO
  ),

  [8] = LAYOUT_split_3x6_3(
    KC_TAB,    KC_Q,           KC_W,           KC_E,             KC_R,           KC_T,               KC_Y,            KC_U,            KC_I,           KC_O,           KC_P,           KC_BSPC,
    KC_LCTL,   KC_A,           KC_S,           KC_D,             KC_F,           KC_G,               KC_H,            KC_J,            KC_K,           KC_L,           KC_SCLN,        KC_QUOT,
    KC_LSFT,   KC_Z,           KC_X,           KC_C,             KC_V,           KC_B,               KC_N,            KC_M,            KC_COMM,        KC_DOT,         KC_SLSH,        LT(6, KC_ESC),
                                               KC_LALT,          KC_SPC,         MO(9),              KC_ENT,          MO(4),           TO(0)
    ),

  [9] = LAYOUT_split_3x6_3(
    KC_TAB,    KC_Q,           KC_W,           KC_E,             KC_R,           KC_ESC,             KC_Y,            KC_U,            KC_I,           KC_O,           KC_P,           KC_BSPC,
    KC_LCTL,   KC_1,           KC_2,           KC_3,             KC_4,           KC_5,               KC_H,            KC_LEFT,         KC_DOWN,        KC_UP,          KC_RGHT,        KC_QUOT,
    KC_LSFT,   KC_6,           KC_7,           KC_8,             KC_9,           KC_0,               KC_N,            KC_M,            KC_COMM,        KC_DOT,         KC_SLSH,        LT(6, KC_ESC),
                                               KC_LALT,          KC_SPC,         KC_NO,              KC_ENT,          KC_NO,           TO(0)
    ),
};