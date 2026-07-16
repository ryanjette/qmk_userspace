/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include QMK_KEYBOARD_H
#define U_UND (LCTL(KC_Z))
#define U_CUT (LCTL(KC_X))
#define U_CPY (LCTL(KC_C))
#define U_PST (LCTL(KC_V))
#define U_RDO (LCTL(KC_Y))
#define MODS_SHIFT (get_mods() & MOD_BIT(KC_LSFT) || get_mods() & MOD_BIT(KC_RSFT))
#define SEND_CAP_STRING(str, capitalized) if (MODS_SHIFT) { \
                                            clear_mods(); \
                                            SEND_STRING(capitalized); \
                                          } else { \
                                            SEND_STRING(str); \
                                          }

// 1. this comma separated list defines how many combos will exist in the firmware
// keep this list 1:1 with the consts below
// the enums here will be written in CAPS to differentiate them from the consts in 2
enum combo_events {
    H_WHG,
    H_PH,
    H_GH,
    H_SH,
    H_TH,
    H_CH,
    C_DEL,
    C_ENTER,
    COMBO_LENGTH
};
// the point of the list is to define this variable which QMK uses to define how many combos will exist
#define COMBO_LEN = COMBO_LENGTH;

// 2. these consts define the keys that make up the combo - their names matter because you refer to them in 3
// the syntax here will be lower case to differentiate them from the enums in 1
#define PROGMEM h_whg_combo[] = {KC_O, KC_Y, COMBO_END}; // TYPE "wh" // for HD Gold/neu/Ti
#define PROGMEM h_ph_combo[] = {KC_M, KC_P, COMBO_END}; // TYPE "ph"
#define PROGMEM h_gh_combo[] = {KC_G, KC_L, COMBO_END}; // TYPE "gh"
#define PROGMEM h_sh_combo[] = {LALT_T(KC_S), LCTL_T(KC_N), COMBO_END}; // "sh"
#define PROGMEM h_th_combo[] = {LCTL_T(KC_N), LSFT_T(KC_D), COMBO_END}; // "th"
#define PROGMEM h_ch_combo[] = {KC_C, KC_L, COMBO_END}; // "ch"
#define PROGMEM c_del_combo[] = {LSFT_T(KC_D), KC_C, COMBO_END}; // Send Delete
#define PROGMEM c_enter_combo[] = {LCTL_T(KC_N), KC_C, COMBO_END}; // Send Enter

// 3. this list tells QMK which combos, defined by your consts, will be added to the list of combos
// [UPPERCASE_ENUM] = COMBO_ACTION(lowercase_const),
combo_t key_combos[] = {
    [H_WHG] = COMBO_ACTION(h_whg_combo),
    [H_PH] = COMBO_ACTION(h_ph_combo),
    [H_GH] = COMBO_ACTION(h_gh_combo),
    [H_SH] = COMBO_ACTION(h_sh_combo),
    [H_TH] = COMBO_ACTION(h_th_combo),
    [H_CH] = COMBO_ACTION(h_ch_combo),
    [C_DEL] = COMBO_ACTION(c_del_combo),
    [C_ENTER] = COMBO_ACTION(c_enter_combo),

};

// 4. this switch/case block tells QMK what each combo does when it is pressed
void process_combo_event(uint16_t combo_index, bool pressed) {
  switch(combo_index) {
    case H_WHG:
      if (pressed) {
        SEND_CAP_STRING("wh", "Wh");
      }
      break;
    case H_PH:
      if (pressed){
        SEND_CAP_STRING("ph", "Ph");
      }
      break;
    case H_GH:
      if (pressed){
        SEND_CAP_STRING("gh", "Gh");
      }
      break;
    case H_SH:
      if (pressed){
       SEND_CAP_STRING("sh", "Sh");
      }
      break;
    case H_TH:
      if (pressed){
        SEND_CAP_STRING("th", "Th");
      }
      break;
    case H_CH:
      if (pressed){
       SEND_CAP_STRING("ch", "Ch");
      }
      break;
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
    DRAG_SCROLL = SAFE_RANGE,
};

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
    if (keycode == DRAG_SCROLL && record->event.pressed) {
        set_scrolling = !set_scrolling;
    }
    return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_charybdis_3x5(
    KC_J,         KC_F,              KC_M,              KC_P,              KC_V,             KC_SCLN,           KC_DOT,           KC_SLSH,          KC_QUOTE,          KC_Z,
    LGUI_T(KC_R), LALT_T(KC_S),      LCTL_T(KC_N),      LSFT_T(KC_D),      KC_W,             KC_COMM,           LSFT_T(KC_A),     LCTL_T(KC_E),     LALT_T(KC_I),      LGUI_T(KC_H),
   LT(7, KC_X),         KC_G,              KC_L,              KC_C,              KC_B,             KC_Q,         LT(5,KC_U),             KC_O,             KC_Y,      LT(6,KC_K),
                                     LT(3, KC_BSPC),    LT(1, KC_T),       LT(2, KC_TAB),    OSM(MOD_LSFT),     LT(4, KC_SPC)    

  ),

//NAV//
  [1] = LAYOUT_charybdis_3x5(
   KC_NO,   U_UND,   U_CPY,             U_PST,           U_CUT,                     U_CUT,   U_PST,   U_CPY,   U_UND,   U_RDO,
    KC_LGUI, KC_LALT,  LCTL_T(KC_BTN2),   KC_BTN1,    DRAGSCROLL_MODE,                     KC_CAPS, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT,
    KC_NO,    KC_NO,   POINTER_DEFAULT_DPI_REVERSE,             POINTER_DEFAULT_DPI_FORWARD,           KC_NO,                     KC_INS,  KC_HOME, KC_PGDN, KC_PGUP, KC_END,
                       KC_NO,             KC_NO,            KC_ENT,                   KC_BSPC, KC_DEL  
  ),

//mouse REMOVE//
  [2] = LAYOUT_charybdis_3x5(
   KC_NO,   KC_NO,    KC_NO,    KC_NO,    KC_NO,    U_RDO,   U_PST,   U_CPY,   U_CUT,   U_UND,
   LCTL(LGUI(KC_LEFT)), LSG(KC_LEFT),  LSG(KC_RGHT), LCTL(LGUI(KC_RGHT)), KC_NO,    KC_NO,    KC_BTN1, KC_BTN3, KC_MS_U, KC_MS_R,
    KC_NO,    KC_ALGR, KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_WH_L, KC_WH_D, KC_WH_U, KC_WH_R,
                      KC_NO,    KC_NO,    KC_NO,    KC_BTN1, KC_BTN3
  ),

//Media//
  [3] = LAYOUT_charybdis_3x5(
   KC_NO,   KC_NO,    KC_NO,    KC_NO,    KC_NO,    RGB_TOG, RGB_MOD, RGB_HUI, RGB_SAI, RGB_VAI,
    KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, KC_NO,    KC_NO,    KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT,
    KC_NO,    KC_ALGR, KC_NO,  KC_NO,   KC_NO,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
                      KC_NO,   KC_NO,  KC_NO,    KC_MSTP, KC_MPLY 
  ),
//RIGHT HAND
//Num//
  [4] = LAYOUT_charybdis_3x5(
    KC_LBRC, KC_7,    KC_8,    KC_9,    KC_RBRC, KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,
    KC_SCLN, KC_4,    KC_5,    KC_6,    KC_EQL,  KC_NO,    KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI,
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_BSLS, KC_NO,    KC_NO,    KC_NO,    KC_ALGR, KC_NO,
                      KC_DOT,  KC_0,    KC_MINS, KC_NO,    KC_NO    
  ),
//Right Button//
  [5] = LAYOUT_charybdis_3x5(
    KC_LCBR, KC_AMPR, KC_ASTR, KC_LPRN, KC_RCBR, KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,
    KC_COLN, KC_DLR,  KC_PERC, KC_CIRC, KC_PLUS, KC_NO,    KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI,
    KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_PIPE, KC_NO,    KC_NO,    KC_NO,    KC_ALGR, KC_NO,
                       KC_LPRN, KC_RPRN, KC_UNDS, KC_ENT,    KC_NO   
  ),
  
  //Function//
  [6] = LAYOUT_charybdis_3x5(
    KC_F12,  KC_F7,   KC_F8,   KC_F9,   KC_PSCR, KC_NO,    QK_BOOT,    KC_NO,    KC_NO,    KC_NO,
    KC_F11,  KC_F4,   KC_F5,   KC_F6,   KC_NO,   KC_NO,    KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI,
    KC_F10,  KC_F1,   KC_F2,   KC_F3,   KC_PAUS, KC_NO,    KC_NO,    KC_NO,    KC_ALGR, KC_NO,
                      KC_APP,  KC_SPC,  KC_TAB,  KC_NO,    KC_NO    
  ),
  
  //Left Button//
   [7] = LAYOUT_charybdis_3x5( 
    KC_NO,   U_UND,    U_CPY,   U_PST,  U_CUT,     U_CUT,   U_PST,   U_CPY,   U_UND,   U_RDO,
    KC_LGUI, KC_LALT,  KC_LCTL, KC_LSFT, KC_NO,    KC_CAPS, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT,
    KC_NO,    KC_NO,   KC_NO,   KC_ESC,   KC_NO,     KC_INS,  KC_HOME, KC_PGDN, KC_PGUP, KC_END,
                        KC_NO,  KC_ESC, KC_ENT,    KC_BSPC, KC_DEL 
  ),


};