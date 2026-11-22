#ifndef _GAME_BOARD_STATUS_H
#define _GAME_BOARD_STATUS_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbStatusInit(void);
void mbStatusClose(void);
void mbStatusReset(void);
void mbStatusMoveTo(int playerNo, HuVecF *posBegin, HuVecF *posEnd);
void mbStatusMoveSet(int playerNo, HuVecF *posBegin, HuVecF *posEnd, int type, int time);
void mbStatusNoMoveSet(int statusNo, HuVecF *posBegin, HuVecF *posEnd, int type, int time);
void mbStatusPosOnGet(int statusNo, HuVecF *pos);
void mbStatusPosOffGet(int statusNo, HuVecF *pos);
BOOL mbStatusMoveCheck(int playerNo);
BOOL mbStatusOffCheckAll(void);
void mbStatusDispSet(int playerNo, BOOL dispF);
void mbStatusDispFocusSet(int playerNo, BOOL dispF);
void mbStatusDispBackup(void);
void mbStatusDispRestoreForce(void);
void mbStatusDispRestore(void);
BOOL mbStatusDispGet(int playerNo);
int mbStatusNoGet(int playerNo);
void mbStatusDispSetAll(BOOL dispF);
void mbStatusDispForceSet(int playerNo, BOOL dispF);
void mbStatusDispForceSetAll(BOOL dispF);
void mbStatusMasuDispSet(int arg0);
void mbStatusCapsuleDispSet(int statusNo, BOOL dispF);
void mbStatusLayoutSet(int statusNo, u8 layoutNo);
void mbStatusCapsuleFocusSet(int statusNo, s8 index);
void mbStatusPosGet(int playerNo, HuVecF *pos);
void mbStatusNoPosGet(int statusNo, HuVecF *pos);
void mbStatusKaoNoSet(int statusNo, s16 kaoNo);
void mbSprCreate(int arg0, s16 arg1, void *arg2, void *arg3);
void mbSprNumSet(int arg0, int arg1, int arg2);
void mbStatusRainbowSet(int statusNo, float time, int colorEnd);
void mbStatusColorSet(int playerNo, int color);
int mbStatusColorGet(int playerNo);
void mbStatusColorAllSet(int color);

#endif
