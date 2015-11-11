/***************************************************
 Light Cycle for GBA
 ver 1.00
 2015.10.25
 choi kyun
 ***************************************************/

#ifndef _sprite_h_
#define _sprite_h_

#include <gba.h>
#include "sprite_point1.h"
#include "sprite_point2.h"
#include "sprite_point3.h"
#include "sprite_point4.h"
#include "sprite_point_end.h"
#include "sprite_up.h"
#include "sprite_down.h"
#include "sprite_you.h"
#include "sprite_you2.h"
#include "sprite_arrow.h"
#include "sprite_drill.h"



#ifdef GLOBAL_VALUE_DEFINE
#define GLOBAL
#define GLOBAL_VAL(v) = (v)
#else
#define GLOBAL extern
#define GLOBAL_VAL(v)
#endif

///////////////////////////////////////////////////////////////////// 各種定義

// スプライト最大数
#define MAX_SPRITE 128


///////////////////////////////////////////////////////////////////// グローバル変数

//////////////////////////////////////////////////////////////////// プロトタイプ
void
init_sprite (void);
void
move_sprite (u16 num, s16 x, s16 y);
void
set_sprite_form (u16 num, u16 size, u16 form, u16 col);
void
set_sprite_tile (u16 num, u16 tile);
void
init_sprite_chr (void);
void
erase_sprite (u16 num);
s16
read_sprite_x (u16 num);
s16
read_sprite_y (u16 num);

#endif	/* _sprite_h_ */
