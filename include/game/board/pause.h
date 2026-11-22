#ifndef _GAME_BOARD_PAUSE_H
#define _GAME_BOARD_PAUSE_H

#include "game/board/board.h"

typedef void (*MBPAUSEHOOK)(BOOL endF);

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbPauseInit(void);
void mbPauseCreate(int playerNo);
int mbPauseStartCheck(void);
BOOL mbPauseProcCheck(void);
void mbPauseDisableSet(BOOL disableF);
BOOL mbPauseDisableGet(void);
void mbPauseHookPush(MBPAUSEHOOK hook);
void mbPauseHookPop(MBPAUSEHOOK hook);
void mbPauseSet(BOOL pauseF);
unsigned int mbPauseDataDirGet(int lang);
unsigned int mbPauseDataNumGet(unsigned int dataNum);

#endif
