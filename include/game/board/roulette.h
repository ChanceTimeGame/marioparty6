#ifndef _GAME_BOARD_ROULETTE_H
#define _GAME_BOARD_ROULETTE_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
BOOL mbRouletteCreate(int playerNo, int maxPlayer);
BOOL mbRouletteKaneCreate(int playerNo);
BOOL mbRouletteCheck(void);
void mbRouletteWait(void);
int mbRouletteResultGet(void);
void mbRouletteMaxSpeedSet(float speed);
void mbRouletteComValueSet(s16 value);

#endif
