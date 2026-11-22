#ifndef _GAME_BOARD_MALLOC_H
#define _GAME_BOARD_MALLOC_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
int mbMalloc(int arg0);
int mbMallocNum(int arg0, int arg1);
int mbMallocFlush(int arg0);
int mbMallocFlushModel(int arg0);
int mbMallocFlushModelNum(int arg0, int arg1);

#endif
