#ifndef _GAME_BOARD_SHOPEVENT_H
#define _GAME_BOARD_SHOPEVENT_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbev_ShopEnableSet(int arg0);
void mbev_ShopInit(int arg0);
void mbev_ShopExInit(int arg0, void *arg1);
void mbev_ShopCreate(int arg0, int arg1);
void mbev_ShopExObjHookSet(void *arg0);
void mbev_ShopBackCreate(int arg0, int arg1, int arg2, int arg3);
void mbev_ShopBackMotCreate(int arg0, int arg1, int arg2, int arg3, void *arg4);
int mbev_Shop(int arg0, int arg1);

#endif
