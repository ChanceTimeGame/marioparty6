#ifndef _GAME_BOARD_GATE_H
#define _GAME_BOARD_GATE_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbGateInit(void);
void mbGateClose(void);
void mbGateCreate(void);
void mbGateBattanCreate(int masuId);
void mbGateBattanKill(int no);
s16 mbev_GateMasu(int playerNo, s16 masuId, s16 masuNext);
int mbev_GateBattanMasuFind(s16 masuId);
int mbev_GateBattan(int playerNo, int masuId);

#endif
