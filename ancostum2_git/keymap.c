#include "crkbd.h"
#include "bootloader.h"
#include "action_layer.h"
#include "action_util.h"
#include "eeconfig.h"
#ifdef PROTOCOL_LUFA
#include "lufa.h"
#include "split_util.h"
#endif
#include "LUFA/Drivers/Peripheral/TWI.h"
#ifdef SSD1306OLED
  #include "ssd1306.h"
#endif

#include "../lib/mode_icon_reader.c"
#include "../lib/layer_state_reader_2.c"
//#include "../lib/host_led_state_reader.c"
#include "../lib/logo_reader.c"
#include "../lib/keylogger.c"
#include "../lib/timelogger.c"

//JIS配列とUS配列の変換定義
#include "keymap_jp.h"

//ダイナミックマクロ関係の諸元を定義
#define KC_DMR1 DYN_REC_START1 //ダイナミックマクロ1の記録を開始
#define KC_DMR2 DYN_REC_START2
#define KC_DMP1 DYN_MACRO_PLAY1//ダイナミックマクロ1を動作
#define KC_DMP2 DYN_MACRO_PLAY2
#define KC_DMRS DYN_REC_STOP //ダイナミックマクロの記録を停止
#define KC_DYNAMIC_MACRO_SIZE 192//ダイナミックマクロのデフォルトサイズは、128

//マウス機能の諸元を定義
#define MOUSEKEY_DELAY             300
#define MOUSEKEY_INTERVAL          0
#define MOUSEKEY_MAX_SPEED         5 
#define MOUSEKEY_TIME_TO_MAX       0
#define MOUSEKEY_WHEEL_MAX_SPEED   8
#define MOUSEKEY_WHEEL_TIME_TO_MAX 40

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
#define _QWERTY 0
#define _OREORE 1
#define _LOWER 3
#define _RAISE 4
#define _ADJUST 16

enum macro_keycodes {
  KC_SAMPLEMACRO,
};

enum{//Define Double tap
	GR_TI = 0,//input ` ~
	F1_EX,    //input ! F1
	F2_AT,    //input @ F2
	F3_HA,    //input # F3
	F4_DL,    //input $ F4
	F5_PE,    //input % F5
	F6_CI,    //input ^ F6
	F7_AM,    //input & F7
	F8_AS,    //input * F8
	F9_LP ,   //input  ( F9
	F10_RP,   //input ) F10
	F11_F12,  //input F12 F11
	LB_RB,    //input [ ] 
	EQ_PL,    //input = +
	MI_UM,    //input - _
	SC_CO,    //input ; :
	QU_DQ,    //input ' "
	CO_LT,    //input , <
	DO_GT,    //input . >
	SL_QU,    //input / ?
	BS_PI,    //input \ |
	IN_PA,    //input Insert Pause
	RA_RC     //input RAlt RCtl
};
//後でライブラリ化
#define KC______  KC_TRNS
#define KC_XXXXX  KC_NO
#define KC_LOWER  LOWER
#define KC_RAISE  RAISE
#define KC_TGOR   TGOR
#define KC_GUIBS  GUI_T(KC_BSPC)
#define KC_GUIDEL GUI_T(KC_DEL)
#define KC_GUISPC GUI_T(KC_SPC)
#define KC_GUIENT GUI_T(KC_ENT)
#define KC_ALTKN  ALT_T(KC_LANG1)
#define KC_TSKMGR LCTL(RSFT(KC_ESC))
#define KC_GRTI TD(GR_TI) 
#define KC_F1EX TD(F1_EX)
#define KC_F2AT TD(F2_AT)
#define KC_F3HA TD(F3_HA)
#define KC_F4DL TD(F4_DL)
#define KC_F5PE TD(F5_PE)
#define KC_F6CI TD(F6_CI)
#define KC_F7AM TD(F7_AM)
#define KC_F8AS TD(F8_AS)
#define KC_F9LP TD(F9_LP)
#define KC_F10RP TD(F10_RP)
#define KC_F11F12 TD(F11_F12)
#define KC_LBRB TD(LB_RB)
#define KC_EQPL TD(EQ_PL)
#define KC_MIUM TD(MI_UM)
#define KC_SCCO TD(SC_CO)
#define KC_QUDQ TD(QU_DQ)
#define KC_COLT TD(CO_LT)
#define KC_DOGT TD(DO_GT)
#define KC_SLQU TD(SL_QU)
#define KC_BSPI TD(BS_PI)
#define KC_INPA TD(IN_PA)
#define KC_RARC TD(RA_RC)
#define KC_KMP0 KMP0
#define KC_KMP1 KMP1
#define KC_KMP2 KMP2
#define KC_KMP3 KMP3
#define KC_FP1  FP1
#define KC_FP2  FP2
#define KC_FP3  FP3
#define KC_FP4  FP4
#define KC_FP5  FP5
#define KC_MACHI MACHI
#define KC_ALENT LALT(KC_ENT)
#define KC_ALDEL RALT(KC_DEL)
#define KC_TG(OR) TG(_OREORE)

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  OREORE,
  LOWER,
  RAISE,
  ADJUST,
  TGOR,
  KMP0,
  KMP1,
  KMP2,
  KMP3,
  FP1,
  FP2,
  FP3,
  FP4,
  FP5,
  MACHI,
  DYNAMIC_MACRO_RANGE,
};

#include "dynamic_macro.h"//ダイナミックマクロのヘッダファイルは、enum配列の後に置く必要があるらしい。

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
        TAB,     Q,     W,     E,     R,     T,                      Y,     U,     I,     O,     P,  LBRB,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       LCTL,     A,     S,     D,     F,     G,                      H,     J,     K,     L,  SCCO,  QUDQ,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       LSPO,     Z,     X,     C,     V,     B,                      N,     M,  COLT,  DOGT,  RSPC,  LGUI,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                   LALT, LOWER,  BSPC,      SPC, RAISE,  RARC \
                              //`--------------------'  `--------------------'
  ),

  [_OREORE] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
      _____, _____, _____, _____,     Y,     D,                      J,     S,     R,     H,     L,  QUDQ,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
      _____, _____,     O,     E,     I,     U,                      G,     N,     T,     K, _____,  LBRB,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
      _____, _____, _____, _____, _____, _____,                      P, _____,  COLT,  DOGT,  RSPC, _____,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                  _____, _____, _____,    _____, _____, _____ \
                              //`--------------------'  `--------------------'
  ),

  [_LOWER] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
       GRTI,  F1EX,  F2AT,  F3HA,  F4DL,  F5PE,                   F6CI,  F7AM,  F8AS,  F9LP, F10RP,F11F12,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       EQPL,     1,     2,     3,     4,     5,                      6,     7,     8,     9,     0,  MIUM,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       LSPO,  KMP0, XXXXX, XXXXX, XXXXX, XXXXX,                  XXXXX, XXXXX,  COLT,  BSPI,  SLQU,  LGUI,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                  _____, _____, _____,      ENT, _____, ALENT \
                              //`--------------------'  `--------------------'
  ),

  [_RAISE] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
        ESC,  DMR2,  DMR1,  DMP2,  DMP1,  DMRS,                   CALC,  HOME,  PGUP,   END,  PSCR,  SLCK,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       CAPS,   FP1,   FP3,   FP4,  HENK,TSKMGR,                   LEFT,  DOWN,    UP, RIGHT,   INS,  PAUS,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       LSPO,   FP2, MACHI,   FP5,  MHEN,  TGOR,                   PGDN,   APP,  VOLD,  VOLU,  RSPC,  LGUI,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                  ALDEL, _____,   DEL,    _____, _____,  RARC \
                              //`--------------------'  `--------------------'
  ),

  [_ADJUST] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
        TAB, XXXXX,  BTN1,  WH_U,  BTN2,  BTN3,                  XXXXX,  BTN4,  MS_U,  BTN5, XXXXX, XXXXX,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       LCTL,  ACL2, XXXXX,  WH_D,  ACL0, XXXXX,                  XXXXX,  MS_L,  MS_D,  MS_R, XXXXX, XXXXX,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       LSPO, XXXXX, XXXXX, XXXXX, XXXXX, XXXXX,                  XXXXX, XXXXX, XXXXX, XXXXX,  RSPC,  LGUI,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                  ALDEL, _____,   DEL,      ENT, _____, ALENT \
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
        TWI_Init(TWI_BIT_PRESCALE_1, TWI_BITLENGTH_FROM_FREQ(1, 800000));
        iota_gfx_init(!has_usb());   // turns on the display
    #endif
}

//SSD1306 OLED update loop, make sure to add #define SSD1306OLED in config.h
#ifdef SSD1306OLED

void matrix_scan_user(void) {
   iota_gfx_task();
}

void matrix_render_user(struct CharacterMatrix *matrix) {
  if (is_master) {
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

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
    set_keylog(keycode, record);
    set_timelog();
  }

   if (!process_record_dynamic_macro (keycode, record)) {//ダイナミックマクロを使用するには、ここからの3行を入れるらしい
			return false;
		}

  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        persistent_default_layer_set(1UL<<_QWERTY);
      }
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
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
          layer_on(_ADJUST);
        } else {
          layer_off(_ADJUST);
        }
        return false;
        break;
    case TGOR:
        if (record->event.pressed) {
          layer_invert(_OREORE);
        }
        return false;
        break;
    case KMP0://Print Layer 0
	if (record->event.pressed) {
		SEND_STRING("/\"OREORE layer\n");
		SEND_STRING(" \" :-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:\n");
		SEND_STRING(" \" :  Tab  :   q   :   w   :   y   :   d   :   f   :   j   :   s   :   r   :   h   :   l   :  &/@  :\n");
		SEND_STRING(" \" :-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:\n");
	  	SEND_STRING(" \" :  LCtl :   a   :   o   :   e   :   i   :   u   :   g   :   n   :   t   :   k   :  ;/\'  :  ]/\\  :\n");
	  	SEND_STRING(" \" :-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:\n");
	  	SEND_STRING(" \" :  LSPO :   z   :   x   :   c   :   v   :   b   :   p   :   m   :  ,/<  :  ./>  :  RSPC :  LGUI :\n");
	  	SEND_STRING(" \" :-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:\n");
	  	SEND_STRING(" \"                         :  LAlt : Lower : GUIBS : GUISPC: Raise : RAlCt :\n");
	  	SEND_STRING(" \"                         :-------:-------:-------:-------:-------:-------:\n");
	  }
	return false;
        break;
/*  case KMP1://Print Layer 1
	if (record->event.pressed) {
		SEND_STRING("/\"ADJUST layer\n"); 
		SEND_STRING(" \" :-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:\n");
	  	SEND_STRING(" \" :  Tab  : xxxxx :  LCL  :  WUP  :  RCL  :  MBT3 : xxxxx :  MBT4 :  MUP  :  MBT5 : xxxxx : xxxxx :\n");
		SEND_STRING(" \" :-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:\n");
	  	SEND_STRING(" \" :  LCtl :  ACL2 : xxxxx :  WDW  :  ACL0 : xxxxx : xxxxx :  MLF  :  MDW  :  MRI  : xxxx  : xxxx  :\n");
		SEND_STRING(" \" :-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:\n");
		SEND_STRING(" \" :  LSPO : xxxxx : xxxxx : xxxxx : xxxxx : xxxxx : xxxxx : xxxxx : xxxxx : xxxxx :  RSPC :  LGUI :\n");
		SEND_STRING(" \" :-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:\n");
		SEND_STRING(" \"                         : ALDEL : _____ : GUIDEL: GUIENT: _____ : ALENT :\n");
		SEND_STRING(" \"                         :-------:-------:-------:-------:-------:-------:\n");
	}
	return false;
      break;
    case KMP2://Print Layer 2
	if (record->event.pressed) {
		SEND_STRING("/\"Lower Layer\n");
		SEND_STRING(" \" :-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:\n");
		SEND_STRING(" \" :   +   :   !   :   [   :   #   :   $   :   %   :   =   :   ^   :   \"   :  *    :   (   :  F11  :\n");
		SEND_STRING(" \" :   {   :   F1  :   F2  :   F3  :   F4  :   F5  :   F6  :   F7  :   F8  :  F9   :  F10  :  F12  :\n");
		SEND_STRING(" \" :-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:\n");
  		SEND_STRING(" \" :   :   :       :       :       :       :       :       :       :       :       :       : UNDS  :\n");
	  	SEND_STRING(" \" :   _   :   1   :   2   :   3   :   4   :   5   :   6   :   7   :   8   :   9   :   0   :   -   :\n");
		SEND_STRING(" \" :-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:\n");
		SEND_STRING(" \" :       :       :       :       :       :       :       :       :   <   :  Pipe :   \?   :       :\n");
		SEND_STRING(" \" :  LSPO :  KMP0 :  KMP1 :  KMP2 :  KMP3 :   b   :   n   :   m   :   ,   :  Yen  :   /   :  LGUI :\n");
		SEND_STRING(" \" :-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:\n");
		SEND_STRING(" \"                         :  LAlt : _____ : GUIBS : GUIENT: Raise : ALENT :\n");
		SEND_STRING(" \"                         :-------:-------:-------:-------:-------:-------:\n");
	}
	return false;
	break;
    case KMP3://Print Layer 3
	if (record->event.pressed) {
		SEND_STRING("/\"Raise Layer\n");
	 	SEND_STRING(" \" :-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:\n");
	 	SEND_STRING(" \" :  Esc  :  DMR2 :  DMR1 :  DMP2 :  DMP1 :  DMRS :  calc :  Home : PgUp  :  END  : Prtsc : Scrlk :\n");
	  	SEND_STRING(" \" :-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:\n");
	  	SEND_STRING(" \" :  Cap  :  FP1  :  FP3  :  FP4  :  HENK : TSKMGR:  Left :   Up  : Down  : Right :  Ins  : Pause :\n");
	  	SEND_STRING(" \" :-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:\n");
		SEND_STRING(" \" :  LSPO :  FP2  : MACHI :  FP5  :  MHEN :TG(ORE):  PgDn :  Menu : VolDw : VolUp : RSPC  : LGUI  :\n");
		SEND_STRING(" \" :-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:-------:\n");
		SEND_STRING(" \"                         :       :       :       :       :       :  RCtl :\n");
		SEND_STRING(" \"                         : ALDEL : Lower : GUIDEL: GUISPC: _____ :  RAlt :\n");
		SEND_STRING(" \"                         :-------:-------:-------:-------:-------:-------:\n");
	}
 	return false;
        break;
*/
    case MACHI://ダイナミックマクロにて、OS側の動作待ちを行うために挿入するためのもの。 
	if (record->event.pressed) {
		_delay_ms(300);//QMKから適当なものを見つけてきた
//	  	wait_ms(500);	
        }
  	return false;	
        break;
    case FP1://社外向け挨拶定型文
	if (record->event.pressed) {
		SEND_STRING(SS_TAP(X_INT4));
	 	SEND_STRING("fugafuga0.");
		SEND_STRING(SS_TAP(X_SPACE)SS_TAP(X_ENTER)"\n");
//		SEND_STRING(SS_TAP(X_SPACE));
	}
	return false;
        break;
    case FP2://メールパスワード送信通知
	if (record->event.pressed) {
		SEND_STRING(SS_TAP(X_INT4));
		SEND_STRING("fugafuga0.");
		SEND_STRING(SS_TAP(X_SPACE)SS_TAP(X_ENTER)"\n");
		SEND_STRING("fugafuga1.");
		SEND_STRING(SS_TAP(X_SPACE));
	}
	return false;
	break;
    case FP3://社内向け挨拶定型文
	if (record->event.pressed) {
		SEND_STRING(SS_TAP(X_INT4));
	 	SEND_STRING("fugafuga2.");
		SEND_STRING(SS_TAP(X_SPACE));
	}
  	return false;
        break;
    case FP4://input ID ハードコーディングあり！！
	if (record->event.pressed) {
		SEND_STRING("hoge");
	 	SEND_STRING(SS_TAP(X_TAB));
		SEND_STRING("hogehoge");
		SEND_STRING(SS_TAP(X_TAB)SS_TAP(X_ENTER));
	}
	return false;
        break;
    case FP5://input PASSWORD ハードコーディングあり！！
	if (record->event.pressed) {
		SEND_STRING("hogehoge");
  		SEND_STRING(SS_TAP(X_TAB)SS_TAP(X_ENTER));
	}
	return false;
        break;

  }
  return true;
}

#endif

//Define Double Tap
qk_tap_dance_action_t tap_dance_actions[] = {
	[GR_TI]   = ACTION_TAP_DANCE_DOUBLE(JP_GRV,  JP_TILD), //input ` ~
	[F1_EX]   = ACTION_TAP_DANCE_DOUBLE(KC_F1,   JP_EXLM), //input F1 !
	[F2_AT]   = ACTION_TAP_DANCE_DOUBLE(KC_F2,   JP_AT),   //input F2 @
	[F3_HA]   = ACTION_TAP_DANCE_DOUBLE(KC_F3,   JP_HASH), //input F3 #
	[F4_DL]   = ACTION_TAP_DANCE_DOUBLE(KC_F4,   JP_DLR),  //input F4 $
	[F5_PE]   = ACTION_TAP_DANCE_DOUBLE(KC_F5,   JP_PERC), //input F5 %
	[F6_CI]   = ACTION_TAP_DANCE_DOUBLE(KC_F6,   JP_CIRC), //input F6 ^
	[F7_AM]   = ACTION_TAP_DANCE_DOUBLE(KC_F7,   JP_AMPR), //input F7 &
	[F8_AS]   = ACTION_TAP_DANCE_DOUBLE(KC_F8,   JP_ASTR), //input F8 *
	[F9_LP]   = ACTION_TAP_DANCE_DOUBLE(KC_F9,   JP_LPRN), //input F9 (
	[F10_RP]  = ACTION_TAP_DANCE_DOUBLE(KC_F10,  JP_RPRN), //input F10 )
	[F11_F12] = ACTION_TAP_DANCE_DOUBLE(KC_F11,  KC_F12),  //input F11 F12
	[LB_RB]   = ACTION_TAP_DANCE_DOUBLE(JP_LBRC, JP_RBRC), //input [ ] 
	[EQ_PL]   = ACTION_TAP_DANCE_DOUBLE(JP_EQL,  JP_PLUS), //input = +
	[MI_UM]   = ACTION_TAP_DANCE_DOUBLE(JP_MINS, JP_UNDS), //input - _
	[SC_CO]   = ACTION_TAP_DANCE_DOUBLE(JP_SCLN, JP_COLN), //input ; :
	[QU_DQ]   = ACTION_TAP_DANCE_DOUBLE(JP_QUOT, JP_DQT),  //input ' "
	[CO_LT]   = ACTION_TAP_DANCE_DOUBLE(JP_COMM, JP_LT ),  //input , <
	[DO_GT]   = ACTION_TAP_DANCE_DOUBLE(JP_DOT,  JP_GT),   //input . >
	[SL_QU]   = ACTION_TAP_DANCE_DOUBLE(JP_SLSH, JP_QUES), //input / ?
	[BS_PI]   = ACTION_TAP_DANCE_DOUBLE(JP_BSLS, JP_PIPE), //input \ |
	[IN_PA]   = ACTION_TAP_DANCE_DOUBLE(KC_INS,  KC_PAUS), //input Inser Pause
	[RA_RC]   = ACTION_TAP_DANCE_DOUBLE(KC_RALT, KC_RCTL), //input RAlt RCtl
};
