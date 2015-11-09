/***************************************************
 Light Cycle for GBA
 ver 1.00
 2015.10.25
 choi kyun
 ***************************************************/

#ifndef _game_h_
#define _game_h_

#include <gba.h>
#include "bg.h"

#ifdef GLOBAL_VALUE_DEFINE
#define GLOBAL
#define GLOBAL_VAL(v) = (v)
#else
#define GLOBAL extern
#define GLOBAL_VAL(v)
#endif

///////////////////////////////////////////////////////////////////// 各種定義

/*
 * 数値表示
 */
#define NUM_W (8)
#define NUM_H (8)

/*
 * レベル表示
 */
#define LV_DIGIT (1)
#define LV_X (107)
#define LV_Y (2)

#define MAX_LEVEL (9)

/*
 * ライフ表示
 */
#define LIFE_DIGIT (1)
#define LIFE_X (139)
#define LIFE_Y LV_Y

#define DEF_LIFE (3)

/*
 * スコア表示
 */
#define SCORE_DIGIT (7)
#define SCORE_X (183)
#define SCORE_Y LV_Y

// スコア加算
#define SCORE_TURN (100)
#define SCORE_NOMAL (1)

/*
 * ハイスコア表示
 */
#define HISCORE_DIGIT (7)
#define HISCORE_X (112)
#define HISCORE_Y (122)

/*
 * 矢印
 */
#define ARROW_X (64)
#define ARROW_Y (89)
#define ARROW_Y2 (104-89)

/*
 * キー
 */
#define DEF_KEY_DELAY (1)
#define DEF_KEY_REPEAT (1)

/*
 * READY メッセージ
 */
#define DEF_READY_X (73)
#define DEF_READY_Y (73)
#define DEF_READY_W (94)
#define DEF_READY_H (14)
#define DEF_READY_COUNT (12)

/*
 * YOUメッセージ
 */
#define DEF_YOU_X (6)
#define DEF_YOU_Y (-6)
#define DEF_YOU2_X (-27)
#define DEF_YOU2_Y (-6)


// YOU表示の切り替え座標
#define DISP_SWITCH_YOU (118)

/*
 * メッセージ表示ウェイト
 */
#define DEF_MES_WAIT (15)

/*
 * サイクル
 */
#define MAX_CYCLE (4)

/*
 * 初期座標
 * 画面4分割
 * それぞれ上下左右4pix余白
 */
#define FIX (100)// 固定小数桁

#define INIT_FIELD_X1 (10*FIX)
#define INIT_FIELD_Y1 (20*FIX)
#define INIT_FIELD_X2 (114*FIX)
#define INIT_FIELD_Y2 (80*FIX)

#define INIT_FIELD_W (105)
#define INIT_FIELD_H (60)

/*
 * サイクルのスピード
 * LV * CYCLE_SPEED / 100
 */
#define DEF_CYCLE_SPEED (20)
#define CYCLE_SPEED (10)

#define MAX_SPEED (FIX)

/*
 * サイクルの曲がる間隔
 */
#define DEF_TURN_INTERVAL (60)

/*
 * 点滅ウェイト
 */
#define CYCLE_BLINK_WAIT_NOMAL (16)
#define CYCLE_BLINK_WAIT_OVER (8)

/*
 * デモ開始まで
 */
#define DEF_DEMO_START (60*15)

/*
 * シーンの切り替えウェイト
 */
#define DEF_SCENE_CHANGE (60)

/*
 * アイテム
 */
#define MAX_ITEM (8)

#define ITEM_W (7)
#define ITEM_H (7)
// アイテムの配置間隔
#define ITEM_SPACE (8)

// 生成範囲　8ドット単位
#define ITEM_FIELD_X1 (2)
#define ITEM_FIELD_X2 (28)
#define ITEM_FIELD_Y1 (2)
#define ITEM_FIELD_Y2 (18)

#define ITEM_SPEED_UP (0)
#define ITEM_SPEED_DOWN (1)

/*
 * アイテムの点滅間隔
 */
#define DEF_ITEM_BLINK_WAIT (30)

/*
 * サイクルの大きさ
 */
#define CYCLE_W (6)
#define CYCLE_H (6)

// サイクル中心の相対位置
#define CYCLE_CX (2)
#define CYCLE_CY (2)


// ゲーム管理
typedef struct
{
    int scene;
    u16 key;
    u16 keyr;
    bool blink_cycles;
    int demo_start;
    int scene_change;
} GameStateType;

// ステージ管理
typedef struct
{
    int lv;
    int mode;
    int life;
    bool demo;
} StageType;

// キャラクタ
typedef struct
{
    int direc;
    int prev_direc;
    int speed;// スピード
    int chr;
    bool show;// 表示/非表示
    bool over;// 行動不能
    int x;
    int y;
    int turn_interval;// 曲がるまでのインターバル
    int turn_interval_rel;
    bool blink_on;// 点滅
    int blink_wait;
    int blink_wait_rel;
    bool use;
} SpriteChrType;

// 点滅メッセージ
typedef struct
{
    int x;
    int y;
    int wait;
    int wait_rel;
    int count;
    u16 save[100 * 14];
    int chr;
} BlinkMessageType;

// ウェイト
typedef struct
{
    int wait;
} WaitType;

// 座標
typedef struct
{
    int direc;
    int x;
    int y;
} PointType;

// クラッシュ
typedef struct
{
    int x;
    int y;
    int time;
    int wait;
    int wait_rel;
} CrashType;

// クリアレベル
typedef struct
{
    int clear_area_nomal;
    int clear_area_special;
    int clear_area_great;
    int beam_speed;
} ClearLevel;

// シーン
enum
{
  GAME_MAIN = 0,
  GAME_TITLE = 1,
  GAME_PAUSE = 2,
  GAME_OVER = 4,
  GAME_READY = 8,
  GAME_OVER_PRE = 16,
  GAME_CRASH = 32,
  GAME_CLEAR = 64,
  GAME_DEMO = 128,
};

// スプライトキャラクタ
enum
{
  SPRITE_POINT1,//0-3 固定
  SPRITE_POINT2,
  SPRITE_POINT3,
  SPRITE_POINT4,
  SPRITE_ARROW,
  SPRITE_ITEM,// 5-12
  SPRITE_YOU = 13,
  SPRITE_YOU2,
};

////////// タイル番号

#define TILE_POINT1 (512)
#define TILE_POINT2 (514)
#define TILE_POINT3 (516)
#define TILE_POINT4 (518)
#define TILE_POINT_END (520)
#define TILE_UP (522)
#define TILE_DOWN (524)
#define TILE_ARROW (526)

#define TILE_YOU (528)
#define TILE_YOU2 (536)

////////// SRAM

#define SRAM_CHECK_HISC (0)
#define SRAM_HISCORE (4)
/*
 * 乱数
 */
#define SRAM_SEED (8)
/*
 * ゲームモード
 */
#define SRAM_MODE (12)

#define SRAM_ON (1)

////////// COLOR

// 壁
#define COL_WALL RGB5(3,3,31)

// サイクルの軌跡
#define COL_POINT1 RGB5(0,31,31)
#define COL_POINT2 RGB5(0,31,0)
#define COL_POINT3 RGB5(31,0,31)
#define COL_POINT4 RGB5(31,31,0)

// 透過
#define COL_TRANS RGB5(0,0,0)
#define COL_TRANS_NUM RGB5(0,0,1)



////////// timer

#define DEF_TIMER_RELOAD (0)

///////////////////////////////////////////////////////////////////// マクロ

#define RND(MIN,MAX) ((MIN)+(int)(rand()/(float)RAND_MAX*((MAX)-(MIN)+1)))

///////////////////////////////////////////////////////////////////// グローバル変数

// ゲーム管理
GLOBAL GameStateType game_state;
GLOBAL StageType stage;

// キャラクタ
GLOBAL SpriteChrType cycles[MAX_CYCLE];

// マイサイクル
GLOBAL SpriteChrType *mycycle;

// アイテム
GLOBAL SpriteChrType items[MAX_ITEM];

// メッセージ表示
GLOBAL BlinkMessageType mes;

// スコア
GLOBAL u32 score;
GLOBAL u32 hiscore;

// 乱数の種
GLOBAL u32 seed;


///////////////////////////////////////////////////////////////////// プロトタイプ
void
init_ship (void);
void
game (void);
void
init_game (void);
void
update_hiscore (void);
void
disp_game_mode (void);

#endif	/* _game_h_ */
