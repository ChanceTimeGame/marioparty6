#ifndef _GAME_BOARD_TELOP_H
#define _GAME_BOARD_TELOP_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbTelopCreate(s8 playerNo, s8 telopNo, BOOL waitF);
void mbTelopPlayerCreate(int playerNo);
void mbTelopPlayerSkipCreate(int arg0);
BOOL mbTelopCheck(void);
void mbTelopLastTurnCreate(void);
void mbTelopTimeCreate(void);
int mbTelopTimeSprCreate(void);
void mbTelopTimeSprKill(int arg0);
void mbTelopTimeSprRotSet(s16 arg0, float f0);
void mbTelopTimeStarSet(int arg0, int arg1);
void mbTelopTimeTPLvlSet(int arg0, float f0);
void mbTelopTimeDispSet(int arg0, int arg1);
s8 mbPadStkXGet(int padNo);
s8 mbPadStkYGet(int padNo);
void mbTauntInit(void);
void mbTauntClose(void);
int mbLanguageGet(void);
void mbLanguageSet(int arg0);
int mbBoardDataNumGet(int arg0);
void mbBoardDataDirRead(void);
void mbTelopTimeChangeCreate(void);
void mbTelopTimeChangeKill(void);
int mbTelopTimeChangeCheck(void);

#endif
