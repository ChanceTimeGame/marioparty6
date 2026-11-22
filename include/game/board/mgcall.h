#ifndef _GAME_BOARD_MGCALL_H
#define _GAME_BOARD_MGCALL_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbMgCallInit(void);
int mbev_MgCall(void);
void mbMgRouletteFocusKill(u8 arg0);
void mbev_MgCallKettou(void);
void mbev_MgCallDonkey(void);
void mbev_MgCallKoopa(void);
void mbev_MgCallSingle(int arg0);
void mbev_MgCallSingleKoopa(int arg0, int arg1);
void mbMgCallDataClose(void);
void mbev_MgCallTutorial(void);
int mbMgCallVsEffCreate(void);
int mbMgCallSingleOnCheck(void);
int mbMgRouletteNumGet(int arg0);

#endif
