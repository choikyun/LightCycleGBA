/***************************************************
 Light Cycle for GBA
 ver 1.00
 2015.10.25
 choi kyun
 ***************************************************/

#define GLOBAL_VALUE_DEFINE

#include <stdio.h>
#include <stdlib.h>
#include <gba.h>

#include "game.h"
#include "bg.h"
#include "sprite.h"
#include "score.h"
#include "music.h"
#include "graph.h"
#include "bmp_num.h"
#include "bmp_ready.h"
#include "bmp_crash.h"
#include "bmp_over.h"
#include "bmp_win.h"


static void
disp_title (void);
static void
disp_ready (void);
static void
disp_over (void);
static void
disp_clear (void);
static void
disp_crash (void);
static void
restart (void);
static void
init_stage (void);
static void
init_cycles (void);
static void
init_items (void);
static void
disp_you ();
static void
move_cycles (void);
static void
move_mycycle (void);
static void
disp_cycles (void);
static void
pause (void);
static void
add_score (u32);
static void
update_life (void);
static void
update_lv (void);
static void
update_score (void);
static void
disp_num (int, int, u16);
static void
shuffle (int[], int);
static bool
is_deadend (int, int);
static void
change_direction (SpriteChrType *);
static void
demo_end (void);
static void
check_over (void);
static void
reset_message (void);
static void
disp_items (void);
static void
get_item (SpriteChrType *);
static void
effect_cycles (int, int);
static void
check_clear (void);

//debug
void
vbaPrint (char *s);

/*
 * サイクルの曲がる間隔
 */
int turn_interval[] = { 200, 200, 300, 400 };


/***************************************************
 メインループ
 ***************************************************/
void
game (void)
{
  game_state.key = keysDown ();
  game_state.keyr = keysDownRepeat ();

  switch (game_state.scene)
  {
    case GAME_TITLE:
      seed++;
      disp_title ();
      break;

    case GAME_DEMO:
      seed++;
      move_cycles ();
      disp_cycles ();
      demo_end ();
      break;

    case GAME_MAIN:
      move_mycycle ();
      move_cycles ();
      disp_cycles ();
      disp_items ();
      check_over ();
      check_clear ();
      pause ();
      break;

    case GAME_READY:
      disp_you ();
      disp_cycles ();
      disp_items ();
      disp_ready ();
      break;

    case GAME_PAUSE:
      pause ();
      break;

    case GAME_OVER:
      disp_over ();
      break;

    case GAME_CRASH:
      disp_crash ();
      break;

    case GAME_CLEAR:
      disp_clear ();
      break;
  }
}

/***************************************************
 タイトル
 ***************************************************/
static void
disp_title (void)
{
  int i;
  u16 key = game_state.key;

  if (key & KEY_START)
  {
    restart();
    stage.demo = false;
    StopMusic();
  }
  else if ((key & KEY_R) && (key & KEY_B))
  {
    clear_hiscore ();
  }
  else if(!--game_state.demo_start)
  {
    // デモ開始
    restart ();
    stage.demo = true;
    game_state.scene = GAME_DEMO;
    StopMusic();
    // デモ用サイクルパラメータ
    for (i = 0; i < MAX_CYCLE; i++)
    {
      cycles[i].speed = CYCLE_SPEED * 4;
    }
  }
}

/***************************************************
 ready表示
 ***************************************************/
static void
disp_ready (void)
{
  if (--mes.wait)
    return;

  mes.wait = mes.wait_rel;
  mes.chr ^= 1;

  if (mes.chr)
    draw_bitmap ( DEF_READY_X, DEF_READY_Y, DEF_READY_W, DEF_READY_H,
                 bmp_readyBitmap, COL_TRANS);
  else
    load_bitmap ( DEF_READY_X, DEF_READY_Y, DEF_READY_W, DEF_READY_H, mes.save);

  if (!--mes.count)
  {
    game_state.scene = GAME_MAIN;

    reset_message();
    // you消去
    erase_sprite (SPRITE_YOU);
    erase_sprite (SPRITE_YOU2);

    PlayMusic (MUSIC_STAGE, PLAY_LOOP_ON);
  }
}

/***************************************************
 デモ終了
 ***************************************************/
static void
demo_end (void)
{
  int i;
  u16 key = game_state.key;

  // デモ繰り返し
  if (cycles[0].over && cycles[1].over && cycles[2].over && cycles[3].over)
  {
    if (!--game_state.scene_change)
    {
      restart();
      game_state.scene = GAME_DEMO;
      for (i = 0; i < MAX_CYCLE; i++)
      {
        cycles[i].speed = CYCLE_SPEED * 5;
      }
    }
  }


  if (key & KEY_START)
  {
    // タイトルのロード
    load_title ();
    game_state.scene = GAME_TITLE;
    game_state.scene_change = DEF_SCENE_CHANGE;

    // デモ開始まで
    game_state.demo_start = DEF_DEMO_START;

    // スプライト消去
    for (i = 0; i < MAX_CYCLE; i++)
    {
      erase_sprite(i);
    }
    PlayMusic (MUSIC_TITLE, PLAY_LOOP_ON);
  }
}

/***************************************************
 Crash
 ***************************************************/
static void
disp_crash (void)
{
  if (--mes.wait)
    return;

  mes.wait = mes.wait_rel;
  mes.chr ^= 1;

  if (mes.chr)
    draw_bitmap ( DEF_READY_X, DEF_READY_Y, DEF_READY_W, DEF_READY_H,
                 bmp_crashBitmap, COL_TRANS);
  else
    load_bitmap ( DEF_READY_X, DEF_READY_Y, DEF_READY_W, DEF_READY_H, mes.save);

  if (!--mes.count)
  {
    if (!--stage.life)
    {
      // ゲームオーバー
      game_state.scene = GAME_OVER;
      save_hiscore (score);
      StopMusic ();
      PlaySound(SOUND_OVER);
      return;
    }
    // リスタート
    restart();
    PlayMusic (MUSIC_STAGE, PLAY_LOOP_ON);
  }
}

/***************************************************
 game over
 ***************************************************/
static void
disp_over (void)
{
  u16 key = game_state.key;

  draw_bitmap ( DEF_READY_X, DEF_READY_Y, DEF_READY_W, DEF_READY_H,
                 bmp_overBitmap, COL_TRANS);

  if (key & KEY_START)
  {
    init_hiscore ();
    init_game ();
    load_title ();
    init_sprite ();
    PlayMusic (MUSIC_TITLE, PLAY_LOOP_ON);
  }
}

/***************************************************
 clear
 ***************************************************/
static void
disp_clear (void)
{
  u16 key = game_state.key;

  draw_bitmap ( DEF_READY_X, DEF_READY_Y, DEF_READY_W, DEF_READY_H,
                 bmp_winBitmap, COL_TRANS);

  if (key & KEY_START)
  {
    // レベルアップ
    if (++stage.lv > MAX_LEVEL)
      stage.lv = MAX_LEVEL;
    // リスタート
    restart();
    StopMusic();
  }
}

/***************************************************
 ゲーム初期化
 ***************************************************/
void
init_game (void)
{
  // ゲームステート初期化
  game_state.scene = GAME_TITLE;
  game_state.scene_change = DEF_SCENE_CHANGE;

  // パラメータ初期化
  score = 0;
  stage.life = DEF_LIFE;
  stage.lv = 0;

  // デモ開始まで
  game_state.demo_start = DEF_DEMO_START;
}

/***************************************************
 リスタート
 ***************************************************/
static void
restart (void)
{
  game_state.scene = GAME_READY;
  game_state.scene_change = DEF_SCENE_CHANGE;

  // ランダマイズ
  srand (REG_TM0CNT + seed);
  // seedをSRAMに保存
  SRAMWrite32(SRAM_SEED, seed);

  // 背景初期化
  load_bg_bitmap_lz77 (DEF_BG_BITMAP);
  // Cycle初期化
  init_cycles ();
  // ステージ初期化
  init_stage ();
  // score
  update_score ();
  // life
  update_life ();
  // lv
  update_lv ();

  // readyメッセージをセット
  reset_message();
}

/***************************************************
 メッセージのリセット
 ***************************************************/
static void
reset_message (void)
{
  mes.count = DEF_READY_COUNT;
  mes.chr = 0;
  mes.wait = mes.wait_rel = DEF_MES_WAIT;
  save_bitmap ( DEF_READY_X, DEF_READY_Y, DEF_READY_W, DEF_READY_H, mes.save);
}

/***************************************************
 ステージ初期化
 ***************************************************/
static void
init_stage (void)
{
  int i;
  // キャラクタ番号
  int c[] = { 0, 1, 2, 3 };
  // XY初期値
  int pos[][2] = { { INIT_FIELD_X1, INIT_FIELD_Y1 }, { INIT_FIELD_X2,
      INIT_FIELD_Y1 }, { INIT_FIELD_X1, INIT_FIELD_Y2 }, { INIT_FIELD_X2,
      INIT_FIELD_Y2 } };
  // ターン可能なインターバル
  int turn[] = {120, 60, 120, 120};

  // キャラクタのシャッフル
  shuffle (c, 4);

  for (i = 0; i < MAX_CYCLE; i++)
  {
    cycles[i].direc = RND(0, 3);
    cycles[i].prev_direc = -1;
    cycles[i].speed = DEF_CYCLE_SPEED  + CYCLE_SPEED * stage.lv;
    cycles[i].x = pos[i][0] + RND(0, INIT_FIELD_W) * FIX;
    cycles[i].y = pos[i][1] + RND(0, INIT_FIELD_H) * FIX;
    cycles[i].chr = c[i];
    cycles[i].show = 1;
    cycles[i].over = false;
    cycles[i].turn_interval = cycles[i].turn_interval_rel = turn[c[i]];
    cycles[i].blink_on = false;
    cycles[i].blink_wait = cycles[i].blink_wait_rel = CYCLE_BLINK_WAIT_NOMAL;

    // 自機取得
    if (cycles[i].chr == 0)
      mycycle = &cycles[i];
  }
  // アイテム初期化
  init_items();

  // サイクルの先頭点滅するか
  game_state.blink_cycles = false;
}

/***************************************************
 サイクル初期化
 ***************************************************/
static void
init_cycles (void)
{
  int i;

  // 8*8
  // サイクル
  set_sprite_form (SPRITE_POINT1, OBJ_SIZE(0), OBJ_SQUARE, OBJ_256_COLOR);
  set_sprite_form (SPRITE_POINT2, OBJ_SIZE(0), OBJ_SQUARE, OBJ_256_COLOR);
  set_sprite_form (SPRITE_POINT3, OBJ_SIZE(0), OBJ_SQUARE, OBJ_256_COLOR);
  set_sprite_form (SPRITE_POINT4, OBJ_SIZE(0), OBJ_SQUARE, OBJ_256_COLOR);

  // アイテム
  for (i = 0; i < MAX_ITEM; i++)
  {
    set_sprite_form (SPRITE_ITEM + i, OBJ_SIZE(0), OBJ_SQUARE, OBJ_256_COLOR);
  }

  // 32*8
  // YOU
  set_sprite_form (SPRITE_YOU, OBJ_SIZE(1), OBJ_SHAPE(1), OBJ_256_COLOR);
  set_sprite_form (SPRITE_YOU2, OBJ_SIZE(1), OBJ_SHAPE(1), OBJ_256_COLOR);


  // タイル
  set_sprite_tile (SPRITE_POINT1, TILE_POINT1);
  set_sprite_tile (SPRITE_POINT2, TILE_POINT2);
  set_sprite_tile (SPRITE_POINT3, TILE_POINT3);
  set_sprite_tile (SPRITE_POINT4, TILE_POINT4);

  set_sprite_tile (SPRITE_YOU, TILE_YOU);
  set_sprite_tile (SPRITE_YOU2, TILE_YOU2);
}

/***************************************************
 アイテム初期化
 ***************************************************/

static void
init_items (void)
{
  int i;

  for(i = 0; i < MAX_ITEM/2; i++)
  {
    items[i].chr = ITEM_SPEED_UP;
  }
  for(i = MAX_ITEM/2; i < MAX_ITEM; i++)
  {
    items[i].chr = ITEM_SPEED_DOWN;
  }
  for(i = 0; i < MAX_ITEM; i++)
  {
    items[i].x = RND(ITEM_FIELD_X1, ITEM_FIELD_X2) * ITEM_W;
    items[i].y = RND(ITEM_FIELD_Y1, ITEM_FIELD_Y2) * ITEM_H;
    items[i].show = true;
    items[i].blink_on = true;
    items[i].blink_wait = items[i].blink_wait_rel = DEF_ITEM_BLINK_WAIT;
    items[i].use = false;

    // タイル
    set_sprite_tile (SPRITE_ITEM + i, TILE_UP + items[i].chr * 2);
  }
}

/***************************************************
 サイクル移動
 ***************************************************/
static void
move_cycles (void)
{
  static int dir[][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
  int i;

  for (i = 0; i < MAX_CYCLE; i++)
  {
    if (cycles[i].over)
      continue;

    // 自機以外は自動ターン
    if (cycles[i].chr || stage.demo)
      change_direction (&cycles[i]);

    // 行動不能でなければ移動
    if (!cycles[i].over)
    {
      cycles[i].x += dir[cycles[i].direc][0] * cycles[i].speed;
      cycles[i].y += dir[cycles[i].direc][1] * cycles[i].speed;

      // アイテム判定
      if (!stage.demo)
        get_item (&cycles[i]);
    }
  }
}

/***************************************************
 方向転換
 ***************************************************/
static void
change_direction (SpriteChrType *c)
{
  static int coord[][2] = { { 2, 0 }, { 0, 2 }, { -2, 0 }, { 0, -2 } };
  // サイクルの気まぐれ度
  static int turn_prob[] = { 128, 64, 128, 256 };
  int prev;
  bool turn = false;

  // ターンのインターバル
  // 0以下でターン可能
  c->turn_interval--;

  if (!RND(0, turn_prob[c->chr]) && c->turn_interval <= 0)
  {
    turn = true;
  }

  // 行き止まりか 気まぐれ
  if (is_deadend (c->x / FIX + coord[c->direc][0],
                  c->y / FIX + coord[c->direc][1]) || turn)
  {
    // インターバルリセット
    c->turn_interval = c->turn_interval_rel;

    // 初回
    if (c->prev_direc == -1)
    {
      prev = c->direc;
      c->direc = RND(0, 1) ? (c->direc - 1) & 3 : (c->direc + 1) & 3;  // 左右どちらか
    }
    else
    {
      prev = c->direc;
      c->direc = c->prev_direc;  // 前回と同じ　ジグザクに移動
    }
    c->prev_direc = prev;

    // もう一度調べる
    if (is_deadend (c->x / FIX + coord[c->direc][0],
                    c->y / FIX + coord[c->direc][1]))
    {
      // 行き止まりなので折り返す
      c->direc = (c->direc + 2) & 3;

      // 最後に調べる　もう移動出来ない
      if (is_deadend (c->x / FIX + coord[c->direc][0],
                      c->y / FIX + coord[c->direc][1]))
      {
        c->over = true;
        c->blink_on = true;
        c->blink_wait_rel = CYCLE_BLINK_WAIT_OVER;
        // スプライトの張替え
        set_sprite_tile (c->chr, TILE_POINT_END);
        update_life ();
        PlaySound(SOUND_CRASH);
      }
    }
  }
}

/***************************************************
 行き止まり判定
 ***************************************************/
static bool
is_deadend (int x, int y)
{
  int i;
  // 座標補正
  int fix[][2] = { { 1, 1 }, { 2, 1 }, { 1, 2 }, { 2, 2 } };

  for (i = 0; i < 4; i++)
  {
    // 壁もしくはサイクル
    int col = point (x + fix[i][0], y + fix[i][1]);
    if (col == COL_WALL || col == COL_POINT1 || col == COL_POINT2
        || col == COL_POINT3 || col == COL_POINT4)
      return true;
  }
  return false;
}

/***************************************************
 自機移動
 ***************************************************/
static void
move_mycycle (void)
{
  u16 key = game_state.key;


  if (stage.demo)
    return;

  if (key & KEY_LEFT)
  {
    mycycle->direc = (mycycle->direc - 1) & 3;
    add_score (SCORE_TURN);
    PlaySound(SOUND_TURN);
    return;
  }
  else if (key & KEY_RIGHT)
  {
    mycycle->direc = (mycycle->direc + 1) & 3;
    add_score (SCORE_TURN);
    PlaySound(SOUND_TURN);
    return;
  }
  add_score(SCORE_NOMAL);
}

/***************************************************
 サイクル表示
 ***************************************************/
static void
disp_cycles (void)
{
  int i;
  u16 col[] = { COL_POINT1, COL_POINT2, COL_POINT3, COL_POINT4 };

  for (i = 0; i < MAX_CYCLE; i++)
  {
    // 点滅するか
    if (cycles[i].blink_on && !--cycles[i].blink_wait)
    {
      cycles[i].blink_wait = cycles[i].blink_wait_rel;
      cycles[i].show ^= 1;
    }

    if (cycles[i].show)
    {
      move_sprite (cycles[i].chr, cycles[i].x / FIX, cycles[i].y / FIX);
      pset2x (cycles[i].x / FIX + 1, cycles[i].y / FIX + 1, col[cycles[i].chr]);
    }
    else
      erase_sprite (cycles[i].chr);
  }
}

/***************************************************
 アイテム効果
 ***************************************************/
static void
effect_cycles(int item, int chr)
{
  int i;

  for (i = 0; i < MAX_CYCLE; i++)
  {
    switch (item)
    {
      // スピードダウン　自分が遅くなる
      case ITEM_SPEED_DOWN:
        if (cycles[i].chr != chr) continue;
        cycles[i].speed -= (cycles[i].speed / 8);
        break;

      // スピードアップ　相手を速くする
      case ITEM_SPEED_UP:
        if (cycles[i].chr == chr) continue;
        cycles[i].speed += (cycles[i].speed / 4);
        if (cycles[i].speed > MAX_SPEED)
          cycles[i].speed = MAX_SPEED;
        break;
    }
  }
}

/***************************************************
 アイテム取得
 ***************************************************/
static void
get_item (SpriteChrType *c)
{
  int i;
  u32 x, y;

  x = (u32)c->x / FIX;
  y = (u32)c->y / FIX;

  for (i = 0; i < MAX_ITEM; i++)
  {
    // 当たり判定　m - e + a < a + b + 1
    if (x - (u32)items[i].x + (u32)(CYCLE_W-1) < (u32)(CYCLE_W-1 + ITEM_W-1 + 1)
        && y - (u32)items[i].y + (u32)(CYCLE_H-1) < (u32)(CYCLE_H-1 + ITEM_H-1 + 1)
        && !items[i].use)
    {
      items[i].use = true;
      erase_sprite (i + 4);
      // アイテムの効果
      effect_cycles(items[i].chr, c->chr);
      PlaySound(SOUND_ITEM);
    }
  }
}

/***************************************************
 アイテム表示
 ***************************************************/
static void
disp_items (void)
{
  int i;

  for (i = 0; i < MAX_ITEM; i++)
  {
    if (!items[i].use)
    {
      // 点滅するか
      if (items[i].blink_on && !--items[i].blink_wait)
      {
        items[i].blink_wait = items[i].blink_wait_rel;
        items[i].show ^= 1;
      }

      if (items[i].show)
        move_sprite (i + 4, items[i].x, items[i].y);
      else
        erase_sprite (i + 4);
    }
  }
}


/***************************************************
 ゲームオーバー判定
 ***************************************************/
static void
check_over (void)
{
  static int coord[][2] = { { 2, 0 }, { 0, 2 }, { -2, 0 }, { 0, -2 } };

  if (is_deadend (mycycle->x / FIX + coord[mycycle->direc][0],
                  mycycle->y / FIX + coord[mycycle->direc][1]))
  {
    // メッセージ初期化
    reset_message();

    mycycle->over = true;
    mycycle->blink_on = true;
    mycycle->blink_wait_rel = CYCLE_BLINK_WAIT_OVER;
    // スプライトの張替え
    set_sprite_tile (mycycle->chr, TILE_POINT_END);

    // クラッシュ
    game_state.scene = GAME_CRASH;
    PlaySound(SOUND_CRASH);
  }
}

/***************************************************
 クリア判定
 ***************************************************/
static void
check_clear (void)
{
  int i;
  int over = 0;

  for (i = 0; i < MAX_CYCLE; i++)
  {
    if ( cycles[i].over && cycles[i].chr != mycycle->chr)
    {
      over++;
    }
  }

  if (over == 3)
  {
    // メッセージ初期化
    reset_message();

    // クリア
    game_state.scene = GAME_CLEAR;
    PlayMusic(MUSIC_TITLE, PLAY_LOOP_ON);
  }
}


/***************************************************
 YOU表示
 ***************************************************/
static void
disp_you (void)
{
  int i;

  for (i = 0; i < MAX_CYCLE; i++)
  {
    if (cycles[i].chr == 0 && cycles[i].x / FIX <= DISP_SWITCH_YOU)
      move_sprite (SPRITE_YOU, cycles[i].x / FIX + DEF_YOU_X,
                   cycles[i].y / FIX + DEF_YOU_Y);
    else if (cycles[i].chr == 0 && cycles[i].x / FIX > DISP_SWITCH_YOU)
      move_sprite (SPRITE_YOU2, cycles[i].x / FIX + DEF_YOU2_X,
                   cycles[i].y / FIX + DEF_YOU2_Y);
  }
}

/***************************************************
 ポーズ
 ***************************************************/
static void
pause (void)
{
  u16 key = game_state.key;

  if (key & KEY_SELECT)
  {
    game_state.scene ^= GAME_PAUSE;
    if (game_state.scene & GAME_PAUSE)
    {
      StopMusic ();
    }
    else
    {
      PlayMusic (MUSIC_STAGE, PLAY_LOOP_ON);
    }
  }
}

/***************************************************
 スコア加算
 ***************************************************/
static void
add_score (u32 num)
{
  score += num;
  update_score ();
}

/***************************************************
 LV表示
 ***************************************************/
static void
update_lv (void)
{
  u32 lv = stage.lv + 1;

  disp_num ( LV_X, LV_Y, lv % 10);
}

/***************************************************
 ライフ表示
 ***************************************************/
static void
update_life (void)
{
  disp_num ( LIFE_X, LIFE_Y, stage.life);
}

/***************************************************
 スコア表示
 ***************************************************/
static void
update_score (void)
{
  int i;
  int pos = SCORE_DIGIT * NUM_W - NUM_W;
  u32 sc = score;

  for (i = 0; i < SCORE_DIGIT; i++)
  {
    disp_num ( SCORE_X + pos, SCORE_Y, sc % 10);
    sc /= 10;
    pos -= NUM_W;
  }
}

/***************************************************
 ハイスコア表示
 ***************************************************/
void
update_hiscore (void)
{
  int i;
  int pos = SCORE_DIGIT * NUM_W - NUM_W;
  u32 sc = hiscore;

  for (i = 0; i < SCORE_DIGIT; i++)
  {
    disp_num ( HISCORE_X + pos, HISCORE_Y, sc % 10);
    sc /= 10;
    pos -= NUM_W;
  }
}

/***************************************************
 数字表示　BG
 ***************************************************/
static void
disp_num (int x, int y, u16 num)
{
  draw_bitmap (x, y, NUM_W, NUM_H, bmp_numBitmap + NUM_W * NUM_H * num,
               COL_TRANS_NUM);
}

//// デバグ用
//THUMB code
void
vbaPrint (char *s)
{
  asm volatile("mov r0, %0;"
      "swi 0xff;"
      : // no ouput
      : "r" (s)
      : "r0");
}

/***************************************************
 配列のシャッフル
 ***************************************************/
void
shuffle (int ary[], int size)
{
  int i, j, t;

  for (i = 0; i < size; i++)
  {
    j = rand () % size;
    t = ary[i];
    ary[i] = ary[j];
    ary[j] = t;
  }
}
