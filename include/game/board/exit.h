#ifndef _GAME_BOARD_EXIT_H
#define _GAME_BOARD_EXIT_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbExitWatchCreate(void);
void mbExitReq(void);
BOOL mbExitCheck(void);
HUPROCESS *mbExitWatchProcGet(void);

#endif
