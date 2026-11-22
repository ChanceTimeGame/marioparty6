#ifndef _GAME_BOARD_SCROLL_H
#define _GAME_BOARD_SCROLL_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbScrollInit(int dataNum, float zoom, float posZ);
void mbScrollClose(void);
void mbev_Scroll(int arg0, int arg1);
void mbev_StarScroll(HuVecF *arg0, HuVecF *arg1, s16 arg2);
void mbScrollStarFindFuncSet(void *arg0);
void mbMapCameraSet(HuVecF *arg0, HuVecF *arg1, float f0);
void mbMapHookSet(void *arg0);
void mbScrollHookSet(void *arg0);
void mbMapSprAdd(int arg0, int arg1);
void mbev_ScrollCapsule(int arg0);

#endif
