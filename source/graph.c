/***************************************************
 Light Cycle for GBA
 ver 1.00
 2015.10.25
 choi kyun
 ***************************************************/

//#define GLOBAL_VALUE_DEFINE
#include <stdio.h>
#include <stdlib.h>
#include <gba.h>
#include "game.h"
#include "graph.h"

/***************************************************
 ポイント
 ***************************************************/
u16
point (int x, int y)
{
  u16 *screen = (u16 *) VRAM;

  return *(screen + y * SCREEN_WIDTH + x);
}

/***************************************************
 ポイントセット
 ***************************************************/
void
pset (int x, int y, u16 col)
{
  u16 *screen = (u16 *) VRAM;

  screen += y * SCREEN_WIDTH + x;
  *screen = col;
}

/***************************************************
 ポイントセット2pix
 ***************************************************/
void
pset2x (int x, int y, u16 col)
{
  u16 *screen = (u16 *) VRAM;

  screen += y * SCREEN_WIDTH + x;

  *screen = col;
  *(screen + 1) = col;
  *(screen + SCREEN_WIDTH) = col;
  *(screen + SCREEN_WIDTH + 1) = col;
}

