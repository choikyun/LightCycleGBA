/***************************************************
 Light Cycle for GBA
 ver 1.00
 2015.10.25
 choi kyun
 ***************************************************/

#ifndef _bg_h_
#define _bg_h_

#include <gba.h>

#include "bmp_bg.h"
#include "bmp_title.h"


#ifdef GLOBAL_VALUE_DEFINE
#define GLOBAL
#define GLOBAL_VAL(v) = (v)
#else
#define GLOBAL extern
#define GLOBAL_VAL(v)
#endif

///////////////////////////////////////////////////////////////////// 各種定義

//// BGタイル モード0

// BGコントロールレジスタ
#define DEF_BG_REG REG_BG0CNT

// BGモード
// bitmap
#define DEF_MODE MODE_3

// BGイネーブル
#define DEF_BG_ENABLE BG0_ENABLE

// BGサイズ
#define DEF_BG_SIZE BG_SIZE_0

// BGカラー
#define DEF_BG_COLOR BG_256_COLOR

// BGキャラクタ
#define DEF_CHAR_BASE CHAR_BASE(0)
#define DEF_BG_BASE_ADR CHAR_BASE_ADR(0)
#define MAX_MAP_X (32)
#define MAX_MAP_Y (32)
#define MAP_WIDTH (32)
#define MAP_HEIGHT (32)

// BGマップ
#define DEF_MAP_BASE MAP_BASE(28)
#define DEF_MAP_BASE_ADR MAP_BASE_ADR(28)

// BGビットマップデータ
#define DEF_BG_LEN bmp_bgBitmapLen
#define DEF_BG_BITMAP bmp_bgBitmap

// BGパレット
//#define DEF_BG_PALLET bmp_bgPal
//#define DEF_BG_PALLET_LEN bmp_bgPalLen

// タイトルビットマップデータ
#define DEF_TITLE_LEN bmp_titleBitmapLen
#define DEF_TITLE_BITMAP bmp_titleBitmap

///////////////////////////////////////////////////////////////////// グローバル変数

//////////////////////////////////////////////////////////////////// プロトタイプ
void
init_bg (void);
void
load_bg_bitmap (const u16 *);
void
load_bg_bitmap_lz77 (const u16 *);
void
load_bg_bitmap_pallet (void);
void
load_bg_tile (void);
void
load_title ();

// 16bit bitmap
void
draw_bitmap (int x, int y, int w, int h, const u16 *data, u16 trans);
void
save_bitmap (int x, int y, int w, int h, u16 *data);
void
load_bitmap (int x, int y, int w, int h, u16 *data);





#endif	/* _bg_h_ */
