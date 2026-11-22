#ifndef _GAME_BOARD_GATE_H
#define _GAME_BOARD_GATE_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbGateInit(void);
void mbGateClose(void);
void mbGateCreate(void);
void mbGateBattanCreate(s16 masuId);
void mbGateBattanKill(int no);
int mbev_GateMasu(int arg0, s16 arg1, s16 arg2);
int mbev_GateBattanMasuFind(s16 arg0);
int mbev_GateBattan(int arg0, int arg1);

#endif
