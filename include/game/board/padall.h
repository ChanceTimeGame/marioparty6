#ifndef _GAME_BOARD_PADALL_H
#define _GAME_BOARD_PADALL_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
u16 mbPadDStkRepGetAll(void);
u16 mbPadBtnDownGetAll(void);

#endif
