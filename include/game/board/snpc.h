#ifndef _GAME_BOARD_SNPC_H
#define _GAME_BOARD_SNPC_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbSNpcInit(void);
void mbSNpcCreate(int arg0, int arg1);
void mbSNpcKill(void);
u8 mbSNpcMasuGet(void);
void mbSNpcMotWinSet(void);
void mbSNpcMotIdleSet(void);
void mbSNpcDispSet(int arg0);
void mbSNpcMotReset(void);
void mbSNpcPlayerWalkSet(int arg0, int arg1);
void mbSNpcStarExec(int arg0, int arg1);
int mbMasuChanceCreate(int arg0, int arg1, int arg2);
void mbMasuChanceKill(void *arg0);
void mbMasuChanceTypeSet(int arg0, u8 arg1, int arg2, int arg3);
void mbMasuChanceFlagSet(int arg0, u8 arg1, int arg2, int arg3, int arg4);
void mbMasuChancePlayerSet(int arg0, u8 arg1);
int mbMasuChanceSet(int arg0, s16 arg1);
int mbSNpcMasuStarNextGet(int arg0, int arg1, void *arg2, int arg3);
void mbObjStarTevStageSet(void *arg0, void *arg1, void *arg2, void *arg3);
void mbObjFadeCreate(s16 arg0, HuVecF *arg1);
void mbObjFadeKill(s16 arg0);
void mbObjFadeTexRotSet(s16 arg0, HuVecF *arg1, HuVecF *arg2);
void mbObjFadeTexColorSet(s16 arg0, u8 arg1, u8 arg2, u8 arg3, float f0);
void mbObjMetalCreate(s16 arg0);
int mbObjMetalKill(s16 arg0);
void mbObjMetalTPLvlSet(s16 arg0, float f0);
void mbObjMetalColorSet(s16 arg0, void *arg1, void *arg2);
void mbObjBiriQCreate(s16 arg0);
int mbObjBiriQKill(s16 arg0);
void mbObjBiriQColorSet(s16 arg0, int arg1, void *arg2, float f0);

#endif
