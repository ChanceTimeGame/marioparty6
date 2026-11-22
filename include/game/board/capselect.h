#ifndef _GAME_BOARD_CAPSELECT_H
#define _GAME_BOARD_CAPSELECT_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
s8 mbCapSelect(void);
s8 mbCapDelete(int arg0, int arg1);
float mbCapSelectGrow(HuVecF *arg0, HuVecF *arg1, s16 arg2, float f0, float f1);
void mbCapSelectResultSet(int arg0, int arg1, int arg2);
void mbCapSelectResultGet(int arg0, void *arg1, void *arg2);
void mbCapSelectResultReset(int arg0);
int mbCapSelectShrinkCheck(int arg0);
void fn_8019A618(void);
void fn_8019A61C(void);
void fn_8019A620(void);
void fn_8019A624(void);
void fn_8019A628(void);
void fn_8019A62C(void);
void mbCapMasuExec(int arg0, int arg1);
void mbCapMasuObjInit(void);
void mbCapMasuObjClose(void);
void mbCapMasuObjCreateAll(void);
void mbCapMasuObjCreate(int arg0);
void mbCapCapsuleGet(int arg0, int arg1);
void mbCapMasuDispSet(int arg0);

#endif
