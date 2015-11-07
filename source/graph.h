/***************************************************
 Light Cycle for GBA
 ver 1.00
 2015.10.25
 choi kyun
 ***************************************************/

#ifndef _graph_h_
#define _graph_h_

#include <gba.h>
#include "game.h"

#ifdef GLOBAL_VALUE_DEFINE
#define GLOBAL
#define GLOBAL_VAL(v) = (v)
#else
#define GLOBAL extern
#define GLOBAL_VAL(v)
#endif


///////////////////////////////////////////////////////////////////// グローバル関数
u16
point (int x, int y);
void
pset (int x, int y, u16 col);
void
pset2x (int x, int y, u16 col);

#endif	/* _graph_h_ */
