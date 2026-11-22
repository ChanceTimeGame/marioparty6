#ifndef _GAME_BOARD_PAUSE_H
#define _GAME_BOARD_PAUSE_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbPauseInit(void);
void mbPauseCreate(int arg0);
int mbPauseStartCheck(void);
BOOL mbPauseProcCheck(void);
void mbPauseDisableSet(BOOL disableF);
BOOL mbPauseDisableGet(void);
void mbPauseHookPush(void *arg0);
void mbPauseHookPop(void *arg0);
void mbPauseSet(BOOL pauseF);
int mbPauseDataDirGet(int arg0);
int mbPauseDataNumGet(int arg0);

#endif
