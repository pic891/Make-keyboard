/*USレイヤの追加(ダブルタップ方式)
 * Copyright 2018 ENDO Katsuhiro <ka2hiro@curlybracket.co.jp>
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

//JIS配列とUS配列の変換定義
#include “keymap_jp.h” 

//ダイナミックマクロ関係の諸元を定義
#define DMR1 DYN_REC_START1 //ダイナミックマクロ1の記録を開始
#define DMR2 DYN_REC_START2
#define DMP1 DYN_MACRO_PLAY1//ダイナミックマクロ1を動作
#define DMP2 DYN_MACRO_PLAY2
#define DMRS DYN_REC_STOP //ダイナミックマクロの記録を停止
#define DYNAMIC_MACRO_SIZE 192//ダイナミックマクロのデフォルトサイズは、128

//マウス機能(加速モード)の諸元を定義
//#define MOUSEKEY_DELAY             300
//#define MOUSEKEY_INTERVAL          0
//#define MOUSEKEY_MAX_SPEED         5 
//#define MOUSEKEY_TIME_TO_MAX       0
//#define MOUSEKEY_WHEEL_MAX_SPEED   8
//#define MOUSEKEY_WHEEL_TIME_TO_MAX 40

//マウス機能(一定モード)の諸元を定義
#define MK_C_OFFSET_UNMOD   16
#define MK_C_INETRVAL_UNMOD 16
#define MK_C_OFFSET_0        1
#define MK_C_INTERVAL_0     32
#define MK_C_OFFSET_1        4
#define MK_C_INTERVAL_1     16
#define MK_C_OFFSET_2       32
#define MK_C_INTERVAL_2     16
#define MK_W_OFFSET_UNMOD    1
#define MK_W_INETRVAL_UNMOD 40
#define MK_W_OFFSET_0    0.125 //1
#define MK_W_INTERVAL_0    720 //360
#define MK_W_OFFSET_1        1
#define MK_W_INTERVAL_1    360 //120
#define MK_W_OFFSET_2        1
#define MK_W_INTERVAL_2     20

// Defines the keycodes used by our macros in process_record_user
#define _JIS     0
#define _US      1  //US配列を追加
#define _Eucalyn 2
#define _LOWER   3
#define _RAISE   4
#define _ADJUST  5

enum tapdances{
	GR_TI = 0,//input ` ~
	F1_EX,    //input ! F1
	F2_AT,    //input @ F2
	F3_HA,    //input # F3
	F4_DL,    //input $ F4
	F5_PE,    //input % F5
	F6_CI,    //input ^ F6
	F7_AM,    //input & F7
	F8_AS,    //input * F8
	F11_F12,  //input F12 F11
	LP_RP,    //input ( ) 
	LB_RB,    //input [ ] 
	EQ_PL,    //input = +
	MI_UM,    //input - _
	SC_CO,    //input ; :
	QU_DQ,    //input ‘ “
	CO_LT,    //input , <
	DO_GT,    //input . >
	SL_QU,    //input / ?
	BS_PI,    //input \ |
	LA_ES,    //input LAlt ESC
	SF_TA,    //input LSft TAB
	LA_TA,    //input LAlt TAB
	F9_LRP,   //input F9 ()
	F10_LR,   //input F10 []
	FP_13,    //input FP1～FP3
	FP_45     //input FP4～5
};

#define _____  KC_TRNS
#define XXXXX  KC_NO

enum custom_keycodes {
  LOWER = SAFE_RANGE,
  RAISE,
  ADJUST,
  EUCHEN, //Eucalyn_HENK
  EUCMHE, //Eucalyn_MHEN
  QWEHEN, //Qwerty_HENK
  QWEMHE, //Qwerty_MHEN
  MACHI,
  DYNAMIC_MACRO_RANGE,
};

#include “dynamic_macro.h”//ダイナミックマクロのヘッダファイルは、enum配列の後に置く必要があるらしい。

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

 [_JIS] = LAYOUT( \

       KC_Q,              KC_W,  KC_E,  KC_R,  KC_T,      KC_Y,  KC_U,  KC_I,       KC_O,       KC_P,\
       LT(_ADJUST,KC_A),  KC_S,  KC_D,  KC_F,  KC_G,      KC_H,  KC_J,  KC_K,       KC_L,       TD(SC_CO),\
       KC_Z,              KC_X,  KC_C,  KC_V,  KC_B,      KC_N,  KC_M,  TD(CO_LT),  TD(DO_GT),  OSM(MOD_LSFT),\
     TD(LA_TA),  LT(_LOWER,KC_BSPC),  CTL_T(KC_ESC),      LGUI_T(KC_TAB),  LT(_RAISE,KC_SPC),  TD(LA_ES) \
  ),

  [_US] = LAYOUT( \

       KC_Q,              KC_W,  KC_E,  KC_R,  KC_T,      KC_Y,  KC_U,  KC_I,       KC_O,       KC_P,\
       LT(_ADJUST,KC_A),  KC_S,  KC_D,  KC_F,  KC_G       KC_H,  KC_J,  KC_K,       KC_L,       TD(SC_CO),\
       KC_Z,              KC_X,  KC_C,  KC_V,  KC_B,      KC_N,  KC_M,  TD(CO_LT),  TD(DO_GT),  OSM(MOD_LSFT),\
     TD(LA_TA),  LT(_LOWER,KC_BSPC),  CTL_T(KC_ESC),      LGUI_T(KC_TAB),  LT(_RAISE,KC_SPC),  TD(LA_ES) \
  ),

  [_Eucalyn] = LAYOUT( \
      _____, _____,  TD(CO_LT),  TD(DO_GT),  TD(SC_CO),      KC_M,  KC_R,  KC_D,  KC_Y, _____,\
      _____,  KC_O,  KC_E,       KC_I,       KC_U,           KC_G,  KC_T, _____,  KC_S,  KC_N,\
      _____, _____,  _____,      _____,      KC_F,           KC_B,  KC_H,  KC_J,  KC_L, _____,\
                                   _____, _____, _____,      _____, _____, _____ \
  ),

  [_LOWER] = LAYOUT( \
       TD(F1_EX),    TD(F2_AT),  TD(F3_HA),  TD(F4_DL),   TD(F5_PE),       KC_CALC,    KC_7,  KC_8,  KC_9,  TD(QU_DQ),\
       TD(F6_CI),    TD(F7_AM),  TD(F8_AS),  TD(F9_LRP),  TD(F10_LR),      TD(EQ_PL),  KC_4,  KC_5,  KC_6,  TD(MI_UM),\
       TD(F11_F12),  TD(GR_TI),  TD(BS_PI),  TD(LP_RP),   TD(LB_RB),       KC_0,       KC_1,  KC_2,  KC_3,  TD(SL_QU),\
                                             TD(SF_TA), _____, _____,      _____,KC_ENT, _____ \
  ),

  [_RAISE] = LAYOUT( \
      DMR2,     DMR1,     DMP2,       DMP1,    DMRS,        XXXXX,    KC_HOME,  KC_PGUP,  KC_END,   XXXXX,\
      KC_ZKHK,  KC_CAPS,  TD(FP_13),  QWEHEN,  EUCHEN,      KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  KC_INS,\
     KC_APP,    MACHI,    TD(FP_45),  QWEMHE,  EUCMHE,      KC_PGDN,  KC_PSCR,  KC_SLCK,  KC_PAUS,  OSM(MOD_LSFT),\
                                  _____,KC_DEL, _____,      _____, _____, _____ \
  ),

  [_ADJUST] = LAYOUT( \
      XXXXX,             XXXXX,    KC_WH_U,  KC_BTN3,    XXXXX,      XXXXX,  KC_BTN4,  KC_MS_U,  KC_BTN5,  KC_VOLU,\
      LT(_ADJUST,KC_A),  KC_BTN1,  KC_WH_D,  KC_BTN2,  KC_ACL0,      XXXXX,  KC_MS_L,  KC_MS_D,  KC_MS_R,  KC_VOLD,\
      XXXXX,             XXXXX,      XXXXX,  KC_ACL2,  KC_ACL1,      XXXXX,  TO(_JIS), TO(_US),    XXXXX,    _____,\
                                          _____,KC_BSPC, _____,      _____,KC_SPC, _____ \
  )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

   if (!process_record_dynamic_macro (keycode, record)) {//ダイナミックマクロを使用するには、ここからの3行を入れるらしい
			return false;
		}

  switch (keycode) {
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
      } else {
        layer_off(_LOWER);
      }
      return false;
      break;

    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
      } else {
        layer_off(_RAISE);
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

    case EUCHEN:
        if (record->event.pressed) {
          layer_on(_Eucalyn);
          SEND_STRING(SS_TAP(X_INT4));
        }
        return false;
        break;

    case EUCMHE:
        if (record->event.pressed) {
          layer_on(_Eucalyn);
          SEND_STRING(SS_TAP(X_INT5));
        }
        return false;
        break;

    case QWEHEN:
        if (record->event.pressed) {
          layer_off(_Eucalyn);
          SEND_STRING(SS_TAP(X_INT4));
        }
        return false;
        break;

    case QWEMHE:
        if (record->event.pressed) {
          layer_off(_Eucalyn);
          SEND_STRING(SS_TAP(X_INT5));
        }
        return false;
        break;

    case MACHI://ダイナミックマクロにて、OS側の動作待ちを行うために挿入するためのもの。 
	if (record->event.pressed) {
		_delay_ms(300);//QMKから適当なものを見つけてきた
//	  	wait_ms(500);	
        }
  	return false;	
        break;

    default:

      break;
  }
  return true;
}

void matrix_init_user(void) {
}

void matrix_scan_user(void) {
}

void led_set_user(uint8_t usb_led) {
}

void keyboard_post_init_user(void) {
	layer_state_set(1);//_キーボード接続時にlayer_stateに1を設定する。これをやらないとlayer_stateが0となり、タップダンスの出力に影響する。
}

//Shiftを押さないものは”register_code(hoge);”、Shiftを押すものは”register_code16(hoge);”となる。
//ただ容量に影響はないため、区別せず両方とも”code16”を使用している。
//Define Tap dance -> GR_TI
void dance_grti_finished (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 1) {
		if (layer_state_is(_JIS)) register_code16(JP_GRV);
		if (layer_state_is(_US))  register_code16(KC_GRV);
	} else  if (state->count == 2) {
		if (layer_state_is(_JIS)) register_code16(JP_TILD);
		if (layer_state_is(_US))  register_code16(KC_TILD);
		}
	}
void dance_grti_reset (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 1) {
		if (layer_state_is(_JIS)) unregister_code16(JP_GRV);
		if (layer_state_is(_US))  unregister_code16(KC_GRV);
	} else  if (state->count == 2) {                                               
		if (layer_state_is(_JIS)) unregister_code16(JP_TILD);
		if (layer_state_is(_US))  unregister_code16(KC_TILD);
		}
	}

//Define Tap dance -> F2_AT
void dance_f2at_finished (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 1) {
		if (layer_state_is(_JIS)) register_code16(KC_F2);
		if (layer_state_is(_US))  register_code16(KC_F2);
	} else  if (state->count == 2) {
		if (layer_state_is(_JIS)) register_code16(JP_AT);
		if (layer_state_is(_US))  register_code16(KC_AT);
		}
	}
void dance_f2at_reset (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 1) {
		if (layer_state_is(_JIS)) unregister_code16(KC_F2);
		if (layer_state_is(_US))  unregister_code16(KC_F2);
	} else  if (state->count == 2) {                                               
		if (layer_state_is(_JIS)) unregister_code16(JP_AT);
		if (layer_state_is(_US))  unregister_code16(KC_AT);
		}
	}

//Define Tap dance -> F6_CI
void dance_f6ci_finished (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 1) {
		register_code16(KC_F6);
	} else  if (state->count == 2) {
		if (layer_state_is(_JIS)) register_code16(JP_CIRC);
		if (layer_state_is(_US))  register_code16(KC_CIRC);
		}
	}
void dance_f6ci_reset (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 1) {
		unregister_code16(KC_F6);
	} else  if (state->count == 2) {                                               
		if (layer_state_is(_JIS)) unregister_code16(JP_CIRC);
		if (layer_state_is(_US))  unregister_code16(KC_CIRC);
		}
	}

//Define Tap dance -> F7_AM
void dance_f7am_finished (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 1) {
		register_code16(KC_F7);
	} else  if (state->count == 2) {
		if (layer_state_is(_JIS)) register_code16(JP_AMPR);
		if (layer_state_is(_US))  register_code16(KC_AMPR);
		}
	}
void dance_f7am_reset (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 1) {
		unregister_code16(KC_F7);
	} else  if (state->count == 2) {                                               
		if (layer_state_is(_JIS)) unregister_code16(JP_AMPR);
		if (layer_state_is(_US))  unregister_code16(KC_AMPR);
		}
	}

//Define Tap dance -> F8_AS
void dance_f8as_finished (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 1) {
		register_code16(KC_F8);
	} else  if (state->count == 2) {
		if (layer_state_is(_JIS)) register_code16(JP_ASTR);
		if (layer_state_is(_US))  register_code16(KC_ASTR);
		}
	}
void dance_f8as_reset (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 1) {
		unregister_code16(KC_F8);
	} else  if (state->count == 2) {                                               
		if (layer_state_is(_JIS)) unregister_code16(JP_ASTR);
		if (layer_state_is(_US))  unregister_code16(KC_ASTR);
		}
	}

//Define Tap dance -> LP_RP
void dance_lprp_finished (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 1) {
		if (layer_state_is(_JIS)) register_code16(JP_LPRN);
		if (layer_state_is(_US))  register_code16(KC_LPRN);
	} else  if (state->count == 2) {
		if (layer_state_is(_JIS)) register_code16(JP_RPRN);
		if (layer_state_is(_US))  register_code16(KC_RPRN);
		}
	}
void dance_lprp_reset (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 1) {
		if (layer_state_is(_JIS)) unregister_code16(JP_LPRN);
		if (layer_state_is(_US))  unregister_code16(KC_LPRN);
	} else  if (state->count == 2) {                                               
		if (layer_state_is(_JIS)) unregister_code16(JP_RPRN);
		if (layer_state_is(_US))  unregister_code16(KC_RPRN);
		}
	}

//Define Tap dance -> LB_RB
void dance_lbrb_finished (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 1) {
		if (layer_state_is(_JIS)) register_code16(JP_LBRC);
		if (layer_state_is(_US))  register_code16(KC_LBRC);
	} else  if (state->count == 2) {
		if (layer_state_is(_JIS)) register_code16(JP_RBRC);
		if (layer_state_is(_US))  register_code16(KC_RBRC);
		}
	}
void dance_lbrb_reset (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 1) {
		if (layer_state_is(_JIS)) unregister_code16(JP_LBRC);
		if (layer_state_is(_US))  unregister_code16(KC_LBRC);
	} else  if (state->count == 2) {                                               
		if (layer_state_is(_JIS)) unregister_code16(JP_RBRC);
		if (layer_state_is(_US))  unregister_code16(KC_RBRC);
		}
	}

//Define Tap dance -> EQ_PL
void dance_eqpl_finished (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 1) {
		if (layer_state_is(_JIS)) register_code16(JP_EQL);
		if (layer_state_is(_US))  register_code16(KC_EQL);
	} else  if (state->count == 2) {
		if (layer_state_is(_JIS)) register_code16(JP_PLUS);
		if (layer_state_is(_US))  register_code16(KC_PLUS);
		}
	}
void dance_eqpl_reset (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 1) {
		if (layer_state_is(_JIS)) unregister_code16(JP_EQL);
		if (layer_state_is(_US))  unregister_code16(KC_EQL);
	} else  if (state->count == 2) {                                               
		if (layer_state_is(_JIS)) unregister_code16(JP_PLUS);
		if (layer_state_is(_US))  unregister_code16(KC_PLUS);
		}
	}

//Define Tap dance -> MI_UM
void dance_mium_finished (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 1) {
		if (layer_state_is(_JIS)) register_code16(JP_MINS);
		if (layer_state_is(_US))  register_code16(KC_MINS);
	} else  if (state->count == 2) {
		if (layer_state_is(_JIS)) register_code16(JP_UNDS);
		if (layer_state_is(_US))  register_code16(KC_UNDS);
		}
	}
void dance_mium_reset (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 1) {
		if (layer_state_is(_JIS)) unregister_code16(JP_MINS);
		if (layer_state_is(_US))  unregister_code16(KC_MINS);
	} else  if (state->count == 2) {                                               
		if (layer_state_is(_JIS)) unregister_code16(JP_UNDS);
		if (layer_state_is(_US))  unregister_code16(KC_UNDS);
		}
	}

//Define Tap dance -> SC_CO
void dance_scco_finished (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 1) {
		if (layer_state_is(_JIS)) register_code16(JP_SCLN);
		if (layer_state_is(_US))  register_code16(KC_SCLN);
	} else  if (state->count == 2) {
		if (layer_state_is(_JIS)) register_code16(JP_COLN);
		if (layer_state_is(_US))  register_code16(KC_COLN);//modを追加するためには、code16
		}
	}
void dance_scco_reset (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 1) {
		if (layer_state_is(_JIS)) unregister_code16(JP_SCLN);
		if (layer_state_is(_US))  unregister_code16(KC_SCLN);
	} else  if (state->count == 2) {                                               
		if (layer_state_is(_JIS)) unregister_code16(JP_COLN);
		if (layer_state_is(_US))  unregister_code16(KC_COLN);
		}
	}

//Define Tap dance -> F9_LRP
void dance_f9lrp_finished (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 1) {
		register_code16(KC_F9);
	} else  if (state->count == 2) {
		if (layer_state_is(_JIS)) { 
			register_code16(JP_LPRN);
			register_code16(JP_RPRN);
			}
		if (layer_state_is(_US)) { 
			register_code16(KC_LPRN);
			register_code16(KC_RPRN);
			}
		}
	}
void dance_f9lrp_reset (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 1) {
		unregister_code16(KC_F9);
	} else  if (state->count == 2) {
		if (layer_state_is(_JIS)) { 
			unregister_code16(JP_LPRN);
			unregister_code16(JP_RPRN);
			}
		if (layer_state_is(_US)) { 
			unregister_code16(KC_LPRN);
			unregister_code16(KC_RPRN);
			}
		}
	}

//Define Tap dance -> F10_LR
void dance_f10lr_finished (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 1) {
		register_code16(KC_F10);
	} else  if (state->count == 2) {
		if (layer_state_is(_JIS)) { 
			register_code16(JP_LBRC);
			register_code16(JP_RBRC);
			}
		if (layer_state_is(_US)) { 
			register_code16(KC_LBRC);
			register_code16(KC_RBRC);
			}
		}
	}
void dance_f10lr_reset (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 1) {
		unregister_code16(KC_F10);
	} else  if (state->count == 2) {
		if (layer_state_is(_JIS)) { 
			unregister_code16(JP_LBRC);
			unregister_code16(JP_RBRC);
			}
		if (layer_state_is(_US)) { 
			unregister_code16(KC_LBRC);
			unregister_code16(KC_RBRC);
			}
		}
	}

//Define Tap dance -> QU_DQ
void dance_qudq_finished (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 1) {
		if (layer_state_is(_JIS)) register_code16(JP_QUOT);
		if (layer_state_is(_US))  register_code16(KC_QUOT);
	} else  if (state->count == 2) {
		if (layer_state_is(_JIS)) register_code16(JP_DQT);
		if (layer_state_is(_US))  register_code16(KC_DQT);
		}
	}
void dance_qudq_reset (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 1) {
		if (layer_state_is(_JIS)) unregister_code16(JP_QUOT);
		if (layer_state_is(_US))  unregister_code16(KC_QUOT);
	} else  if (state->count == 2) {                                               
		if (layer_state_is(_JIS)) unregister_code16(JP_DQT);
		if (layer_state_is(_US))  unregister_code16(KC_DQT);
		}
	}

//Define Tap dance -> BS_PI
void dance_bspi_finished (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 1) {
		if (layer_state_is(_JIS)) register_code16(JP_BSLS);
		if (layer_state_is(_US))  register_code16(KC_BSLS);
	} else  if (state->count == 2) {
		if (layer_state_is(_JIS)) register_code16(JP_PIPE);
		if (layer_state_is(_US))  register_code16(KC_PIPE);
		}
	}
void dance_bspi_reset (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 1) {
		if (layer_state_is(_JIS)) unregister_code16(JP_BSLS);
		if (layer_state_is(_US))  unregister_code16(KC_BSLS);
	} else  if (state->count == 2) {                                               
		if (layer_state_is(_JIS)) unregister_code16(JP_PIPE);
		if (layer_state_is(_US))  unregister_code16(KC_PIPE);
		}
	}

//Define Tap dance ->FP_13
void dance_FP13_finished (qk_tap_dance_state_t *state, void *user_data) {
		if (state->count == 1) {
			SEND_STRING(SS_TAP(X_INT4));
		 	SEND_STRING(“hoge”);
			SEND_STRING(SS_TAP(X_SPACE)SS_TAP(X_ENTER)”\n”);
		} else if (state->count == 2) {	
			SEND_STRING(SS_TAP(X_INT4));
			SEND_STRING(“hoge”);
			SEND_STRING(SS_TAP(X_SPACE)SS_TAP(X_ENTER)”\n”);
			SEND_STRING(“hoge”);
			SEND_STRING(SS_TAP(X_SPACE));

		} else if (state->count == 3) {
			SEND_STRING(SS_TAP(X_INT4));
		 	SEND_STRING(“hogek”);
			SEND_STRING(SS_TAP(X_SPACE));
		}
	}

//Define Tap dcance ->FP_45
void dance_FP45_finished (qk_tap_dance_state_t *state, void *user_data) {
		if (state->count ==1) {		
			SEND_STRING(“hoge”);
		 	SEND_STRING(SS_TAP(X_TAB));
			SEND_STRING(“hoge”);
			SEND_STRING(SS_TAP(X_TAB)SS_TAP(X_ENTER));
		} else if (state->count == 2) {	
			SEND_STRING(“hoge”);
	  		SEND_STRING(SS_TAP(X_TAB)SS_TAP(X_ENTER));
		}
	}

//Define Double Tap
//ACTION_TAP_DANCE_FN_ADVANCED (NULL, dance__finished, dance__reset),
qk_tap_dance_action_t tap_dance_actions[] = {
	[GR_TI]   = ACTION_TAP_DANCE_FN_ADVANCED (NULL, dance_grti_finished, dance_grti_reset), //input ` ~
	[F1_EX]   = ACTION_TAP_DANCE_DOUBLE(KC_F1,   JP_EXLM), //input F1 !
	[F2_AT]   = ACTION_TAP_DANCE_FN_ADVANCED (NULL, dance_f2at_finished, dance_f2at_reset), //input F2 @
	[F3_HA]   = ACTION_TAP_DANCE_DOUBLE(KC_F3,   JP_HASH), //input F3 #
	[F4_DL]   = ACTION_TAP_DANCE_DOUBLE(KC_F4,   JP_DLR),  //input F4 $
	[F5_PE]   = ACTION_TAP_DANCE_DOUBLE(KC_F5,   JP_PERC), //input F5 %
	[F6_CI]   = ACTION_TAP_DANCE_FN_ADVANCED (NULL, dance_f6ci_finished, dance_f6ci_reset), //input F6 ^
	[F7_AM]   = ACTION_TAP_DANCE_FN_ADVANCED (NULL, dance_f7am_finished, dance_f7am_reset), //input F7 &
	[F8_AS]   = ACTION_TAP_DANCE_FN_ADVANCED (NULL, dance_f8as_finished, dance_f8as_reset), //input F8 *
	[F11_F12] = ACTION_TAP_DANCE_DOUBLE(KC_F11,  KC_F12),  //input F11 F12
	[LP_RP]   = ACTION_TAP_DANCE_FN_ADVANCED (NULL, dance_lprp_finished, dance_lprp_reset), //input ( )
	[LB_RB]   = ACTION_TAP_DANCE_FN_ADVANCED (NULL, dance_lbrb_finished, dance_lbrb_reset), //input [ ]
	[EQ_PL]   = ACTION_TAP_DANCE_FN_ADVANCED (NULL, dance_eqpl_finished, dance_eqpl_reset), //input = +
	[MI_UM]   = ACTION_TAP_DANCE_FN_ADVANCED (NULL, dance_mium_finished, dance_mium_reset), //input - _
	[SC_CO]  = ACTION_TAP_DANCE_FN_ADVANCED (NULL, dance_scco_finished, dance_scco_reset),//input ; :
	[QU_DQ]   = ACTION_TAP_DANCE_FN_ADVANCED (NULL, dance_qudq_finished, dance_qudq_reset),  //input ‘ “
	[CO_LT]   = ACTION_TAP_DANCE_DOUBLE(JP_COMM, JP_LT ),   //input , <
	[DO_GT]   = ACTION_TAP_DANCE_DOUBLE(JP_DOT,  JP_GT),    //input . >
	[SL_QU]   = ACTION_TAP_DANCE_DOUBLE(JP_SLSH, JP_QUES),  //input / ?
	[BS_PI]   = ACTION_TAP_DANCE_FN_ADVANCED (NULL, dance_bspi_finished, dance_bspi_reset), //input \ |
	[LA_ES]   = ACTION_TAP_DANCE_DOUBLE(KC_LALT, KC_ESC), //input LAlt ESC
	[SF_TA]   = ACTION_TAP_DANCE_DOUBLE(KC_LSFT, KC_TAB), //input LSft TAB
	[LA_TA]   = ACTION_TAP_DANCE_DOUBLE(KC_LALT, KC_TAB), //input LAlt TAB
	[F9_LRP]  = ACTION_TAP_DANCE_FN_ADVANCED (NULL, dance_f9lrp_finished, dance_f9lrp_reset),//input F9 []
	[F10_LR]  = ACTION_TAP_DANCE_FN_ADVANCED (NULL, dance_f10lr_finished, dance_f10lr_reset),//input F10 []
	[FP_13]   = ACTION_TAP_DANCE_FN_ADVANCED (NULL, dance_FP13_finished,NULL), //input FP1～3
	[FP_45]   = ACTION_TAP_DANCE_FN_ADVANCED (NULL, dance_FP45_finished,NULL)  //input FP4～5
};
