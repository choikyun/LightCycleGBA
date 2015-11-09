/***************************************************
 Light Cycle for GBA
 ver 1.00
 2015.10.25
 choi kyun
 ***************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <gba.h>

#include "sprite.h"
#include "bg.h"

/***************************************************
 スプライト初期化
 ***************************************************/
void
init_sprite (void)
{
  int i;

  //すべて画面外へ
  for (i = 0; i < MAX_SPRITE; i++)
  {
    move_sprite (i, SCREEN_WIDTH, SCREEN_HEIGHT);
  }

}

/***************************************************
 スプライト非表示
 ***************************************************/
void
erase_sprite (u16 num)
{
  move_sprite (num, SCREEN_WIDTH, SCREEN_HEIGHT);
}

/***************************************************
 スプライト移動
 ***************************************************/
void
move_sprite (u16 num, s16 x, s16 y)
{
  OBJATTR *sp = (OBJATTR*) OAM + num;

  sp->attr1 &= 0xfe00;
  sp->attr0 &= 0xff00;
  sp->attr1 |= OBJ_X(x);
  sp->attr0 |= OBJ_Y(y);
}

/***************************************************
 スプライト形状　カラー サイズ
 ***************************************************/
void
set_sprite_form (u16 num, u16 size, u16 form, u16 col)
{
  OBJATTR *sp = (OBJATTR*) OAM + num;

  sp->attr0 &= 0x1fff;
  sp->attr1 &= 0x3fff;
  sp->attr0 |= col | form;
  sp->attr1 |= size;
}

/***************************************************
 タイルキャラクタ設定
 **************************************************/
void
set_sprite_tile (u16 num, u16 tile)
{
  OBJATTR *sp = (OBJATTR*) OAM + num;

  sp->attr2 &= 0xfc00;
  sp->attr2 |= tile;
}

/***************************************************
 スプライトX座標読み取り
 ***************************************************/
s16
read_sprite_x (u16 num)
{
  OBJATTR *sp = (OBJATTR*) OAM + num;

  return sp->attr1 & ~0xfe00;
}

/***************************************************
 スプライトY座標読み取り
 ***************************************************/
s16
read_sprite_y (u16 num)
{
  OBJATTR *sp = (OBJATTR*) OAM + num;

  return sp->attr0 & ~0xff00;
}

/***************************************************
 キャラクタ初期化
 ***************************************************/
void
init_sprite_chr (void)
{
  u16 *oam = DEF_MODE < MODE_3 ? OBJ_BASE_ADR : BITMAP_OBJ_BASE_ADR;// スプライトデータ
  u16 *pal = OBJ_COLORS;		// スプライトパレット

  // 共通パレット転送
  CpuSet (sprite_point1Pal, pal, (COPY16 | sprite_point1PalLen / 2));

  // データ転送
  CpuSet ( sprite_point1Tiles, oam, (COPY16 | sprite_point1TilesLen / 2));
  CpuSet ( sprite_point2Tiles, oam + sprite_point1TilesLen / 2, (COPY16 | sprite_point1TilesLen / 2));
  CpuSet ( sprite_point3Tiles, oam + sprite_point1TilesLen / 2 * 2, (COPY16 | sprite_point1TilesLen / 2));
  CpuSet ( sprite_point4Tiles, oam + sprite_point1TilesLen / 2 * 3, (COPY16 | sprite_point1TilesLen / 2));
  CpuSet ( sprite_point_endTiles, oam + sprite_point1TilesLen / 2 * 4, (COPY16 | sprite_point1TilesLen / 2));
  CpuSet ( sprite_upTiles, oam + sprite_point1TilesLen / 2 * 5, (COPY16 | sprite_point1TilesLen / 2));
  CpuSet ( sprite_downTiles, oam + sprite_point1TilesLen / 2 * 6, (COPY16 | sprite_point1TilesLen / 2));
  CpuSet ( sprite_arrowTiles, oam + sprite_point1TilesLen / 2 * 7, (COPY16 | sprite_point1TilesLen / 2));

  CpuSet ( sprite_youTiles, oam + sprite_point1TilesLen / 2 * 8, (COPY16 | sprite_youTilesLen / 2));
  CpuSet ( sprite_you2Tiles, oam + sprite_point1TilesLen / 2 * 8 + sprite_youTilesLen/2, (COPY16 | sprite_youTilesLen / 2));

}
