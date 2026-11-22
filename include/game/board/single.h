#ifndef _GAME_BOARD_SINGLE_H
#define _GAME_BOARD_SINGLE_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbSingleInit(void);
void mbSingleClose(void);
void mbSingleSaveInit(int arg0, int arg1, int arg2);
void mbSingleMgUnlockInit(void);
void mbSingleMgUnlockWrite(void);
void mbSingleMgUnlockSet(int arg0);
void mbSingleMgUnlockReset(int arg0);
int mbSingleMgUnlockGet(int arg0);
int mbSingleMgUnlockCheckAny(void);
int mbSingleMgUnlockNumGet(void);
void mbev_SingleMg(int arg0, int arg1);
int mbev_SingleMgEnd(int arg0);
void mbSinglePrizeFlagReset(int arg0);
int mbSingleCall(int arg0, u8 arg1);
void mbSingleReturn(void);
void mbSingleReturnWrite(void);
void mbSingleGameEnd(void);
void mbSingleSaveFlush(int arg0);
int mbSingleStepGet(void);
int mbSingleOppCharGet(void);
void mbSingleTeamCharSet(int arg0);
int mbSingleTeamCharGet(void);
int mbSingleMgUnlockCheck(void);

#endif
