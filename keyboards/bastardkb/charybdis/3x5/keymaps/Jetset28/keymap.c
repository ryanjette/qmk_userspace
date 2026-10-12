
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
#define U_LOCK (LCTL(LGUI(KC_Q)))

enum layers {
  NAV = 2,
  NUM = 4
};

enum combo_events {
    C_DEL,
    C_ENTER,
  C_ESC,
  C_V,
  C_W,
  C_B,
  C_SCLN,
  C_COMM,
  C_Z,
    COMBO_LENGTH
};
// the point of the list is to define this variable which QMK uses to define how many combos will exist
uint16_t COMBO_LEN = COMBO_LENGTH;

// 2. these consts define the keys that make up the combo - their names matter because you refer to them in 3
// the syntax here will be lower case to differentiate them from the enums in 1
const uint16_t PROGMEM c_del_combo[] = {KC_D, KC_C, COMBO_END}; // Send Delete
const uint16_t PROGMEM c_enter_combo[] = {KC_N, KC_C, COMBO_END}; // Send Enter
const uint16_t PROGMEM c_esc_combo[] = {KC_M, KC_D, COMBO_END};
const uint16_t PROGMEM c_v_combo[] = {KC_F, KC_P, COMBO_END};
const uint16_t PROGMEM c_w_combo[] = {KC_D, KC_S, COMBO_END};
const uint16_t PROGMEM c_b_combo[] = {KC_G, KC_C, COMBO_END};
const uint16_t PROGMEM c_scln_combo[] = {KC_DOT, KC_QUOTE, COMBO_END};
const uint16_t PROGMEM c_comm_combo[] = {KC_A, KC_I, COMBO_END};
const uint16_t PROGMEM c_z_combo[] = {KC_U, KC_Y, COMBO_END};

// 3. this list tells QMK which combos, defined by your consts, will be added to the list of combos
// [UPPERCASE_ENUM] = COMBO_ACTION(lowercase_const),
combo_t key_combos[] = {
    [C_DEL] = COMBO_ACTION(c_del_combo),
    [C_ENTER] = COMBO_ACTION(c_enter_combo),
    [C_ESC] = COMBO(c_esc_combo, KC_ESC),
    [C_V] = COMBO(c_v_combo, KC_V),
    [C_W] = COMBO(c_w_combo, KC_W),
    [C_B] = COMBO(c_b_combo, KC_B),
    [C_SCLN] = COMBO(c_scln_combo, KC_SCLN),
    [C_COMM] = COMBO(c_comm_combo, KC_COMM),
    [C_Z] = COMBO(c_z_combo, KC_Z)

};

bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
  (void)combo_index;
  (void)combo;
  (void)keycode;
  (void)record;

  return get_highest_layer(layer_state) == 0;
}

// 4. this switch/case block tells QMK what each combo does when it is pressed
void process_combo_event(uint16_t combo_index, bool pressed) {
  switch(combo_index) {
    case C_DEL:
      if (pressed){
       tap_code16(KC_DEL);
      }
      break;
    case C_ENTER:
      if (pressed){
       tap_code16(KC_ENT);
      }
      break;
  }
}
//Drag Scroll//
enum custom_keycodes {
  OS_SHFT = SAFE_RANGE,
  OS_CTRL,
  OS_ALT,
  OS_CMD,
  DRAG_SCROLL,
};

static oneshot_state os_shft_state = os_up_unqueued;
static oneshot_state os_ctrl_state = os_up_unqueued;
static oneshot_state os_alt_state = os_up_unqueued;
static oneshot_state os_cmd_state = os_up_unqueued;

bool is_oneshot_cancel_key(uint16_t keycode) {
  return keycode == MO(NAV) || keycode == MO(NUM) || keycode == LT(1, KC_T) || keycode == LT(6, KC_K);
}

bool is_oneshot_ignored_key(uint16_t keycode) {
  switch (keycode) {
    case MO(NAV):
    case MO(NUM):
    case LT(1, KC_T):
    case LT(6, KC_K):
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

bool set_scrolling = false;

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (set_scrolling) {
        mouse_report.h = mouse_report.x;
        mouse_report.v = mouse_report.y;
        mouse_report.x = 0;
        mouse_report.y = 0;
    }
    return mouse_report;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  update_oneshot(&os_shft_state, KC_LSFT, OS_SHFT, keycode, record);
  update_oneshot(&os_ctrl_state, KC_LCTL, OS_CTRL, keycode, record);
  update_oneshot(&os_alt_state, KC_LALT, OS_ALT, keycode, record);
  update_oneshot(&os_cmd_state, KC_LGUI, OS_CMD, keycode, record);

  if (keycode == DRAG_SCROLL) {
    if (record->event.pressed) {
      set_scrolling = !set_scrolling;
    }
    return false;
    }
    return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(
    
  KC_J,           KC_F,           KC_M,             KC_P,           KC_NO,              KC_NO,           KC_DOT,          KC_SLSH,        KC_QUOTE,       KC_Q,           
  KC_R,           KC_S,           KC_N,             KC_D,           KC_NO,              KC_NO,           KC_A,            KC_E,           KC_I,           KC_H,           
  KC_X,           KC_G,           KC_L,             KC_C,           KC_NO,              KC_NO,           KC_U,           KC_O,           KC_Y,           LT(6, KC_K),   
                            KC_NO,            LT(1, KC_T),           MO(NAV),         MO(NUM),         MT(MOD_LSFT, KC_SPC)
  ),

//NAV//
  [1] = LAYOUT(
  U_LOCK,   U_UND,   U_CPY,                         U_PST,                        U_CUT,                              U_CUT,   U_PST,   U_CPY,   U_UND,   U_RDO,
  KC_NO,    KC_NO,  KC_BTN2,                       KC_BTN1,                      KC_NO,                              KC_CAPS, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT,
  KC_NO,   POINTER_DEFAULT_DPI_REVERSE,   POINTER_DEFAULT_DPI_FORWARD,  DRAGSCROLL_MODE,KC_NO,                       KC_INS,  KC_HOME, KC_PGDN, KC_PGUP, KC_END,
                     KC_NO,                         KC_NO,                        KC_ENT,                            KC_BSPC, KC_DEL  
  ),

  [NAV] = LAYOUT(
     U_CUT,  U_UND,  U_CPY,  U_PST, KC_NO,             KC_NO,  KC_NO,  KC_NO, KC_NO,  KC_NO,
     OS_CMD, OS_ALT, OS_CTRL, OS_SHFT,KC_NO,          KC_CAPS, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT,
     KC_NO,  KC_TAB, KC_BSPC, KC_NO, KC_NO,           KC_INS,  KC_HOME, KC_PGDN, KC_PGUP, KC_END,
                    KC_NO,  KC_NO,  KC_NO,            KC_BSPC, KC_DEL
  ),

//RIGHT HAND
//Num//
  [NUM] = LAYOUT(
    KC_LBRC, KC_7,    KC_8,    KC_9,    KC_RBRC,           KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,
    KC_SCLN, KC_4,    KC_5,    KC_6,    KC_EQL,            KC_NO,    OS_SHFT, OS_CTRL, OS_ALT,  OS_CMD,
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_BSLS,           KC_NO,    KC_NO,    KC_NO,    KC_ALGR, KC_NO,
                      KC_DOT,  KC_0,    KC_MINS,           KC_NO,    KC_NO    
  ),
  
  //Function//
  [6] = LAYOUT(
    KC_F12,  KC_F7,   KC_F8,   KC_F9,   KC_PSCR, KC_NO,    QK_BOOT,    KC_NO,    KC_NO,    KC_NO,
    KC_F11,  KC_F4,   KC_F5,   KC_F6,   KC_NO,   KC_NO,    KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI,
    KC_F10,  KC_F1,   KC_F2,   KC_F3,   KC_PAUS, KC_NO,    KC_NO,    KC_NO,    KC_ALGR, KC_NO,
                      KC_APP,  KC_SPC,  KC_TAB,  KC_NO,    KC_NO    
  ),
  


};