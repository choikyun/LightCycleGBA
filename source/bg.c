/***************************************************
 Light Cycle for GBA
 ver 1.00
 2015.10.25
 choi kyun
 ***************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <gba.h>

#include "bg.h"
#include "game.h"

/***************************************************
 BG初期化
 ***************************************************/
void
init_bg (void)
{
  // モード設定 BG2 使用
  SetMode ( DEF_MODE | (BG2_ENABLE) | OBJ_ENABLE | OBJ_1D_MAP);
}

/***************************************************
 BG ビットマップ描画 32768色
 ***************************************************/
void
draw_bitmap (int x, int y, int w, int h, const u16 *data, u16 trans)
{
  int i, j;
  u16 *screen = (u16 *) VRAM + y * SCREEN_WIDTH + x;

  for (j = 0; j < h; j++)
  {
    for (i = 0; i < w; i++)
    {
      if (*data != trans)
        *screen++ = *data++;
      else
      {
        screen++;
        data++;
      }
    }
    screen += SCREEN_WIDTH - w;
  }
}


/***************************************************
 BG ビットマップ退避
 ***************************************************/
void
save_bitmap (int x, int y, int w, int h, u16 *data)
{
  int i, j;
  u16 *screen = (u16 *) VRAM + y * SCREEN_WIDTH + x;

  for (j = 0; j < h; j++)
  {
    for (i = 0; i < w; i++)
    {
      *data++ = *screen++;
    }
    screen += SCREEN_WIDTH - w;
  }
}

/***************************************************
 BG ビットマップ復帰
 ***************************************************/
void
load_bitmap (int x, int y, int w, int h, u16 *data)
{
  int i, j;
  u16 *screen = (u16 *) VRAM + y * SCREEN_WIDTH + x;

  for (j = 0; j < h; j++)
  {
    for (i = 0; i < w; i++)
    {
      *screen++ = *data++;
    }
    screen += SCREEN_WIDTH - w;
  }
}

/***************************************************
 BG ビットマップ転送
 ***************************************************/
void
lord_bg_bitmap (u16 *data, int len)
{
  u16 *screen = (u16 *) VRAM;

  CpuSet ( data, screen, (COPY16 | len));
}

/***************************************************
 BG ビットマップ転送 圧縮LZ77
 ***************************************************/
void
load_bg_bitmap_lz77 (const u16 *data)
{
  void *screen = (void *) VRAM;

  LZ77UnCompVram ((void *) data, screen);
}

/***************************************************
 BG ビットマップモード用パレット転送
 ***************************************************/
void
load_bg_bitmap_pallet (void)
{
  //CpuSet ( DEF_BG_PALLET, BG_PALETTE, (COPY16 | DEF_BG_PALLET_LEN));
}

/***************************************************
 BG タイル・パレット転送
 ***************************************************/
void
load_bg_tile (void)
{
}

/***************************************************
 タイトル ビットマップ転送
 ***************************************************/
void
load_title (void)
{
  load_bg_bitmap_lz77(DEF_TITLE_BITMAP);
}

