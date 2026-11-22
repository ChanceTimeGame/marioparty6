#ifndef _GAME_BOARD_COMCHOICE_H
#define _GAME_BOARD_COMCHOICE_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbComChoiceLeftSet(void);
void mbComChoiceRightSet(void);
void mbComChoiceUpSet(void);
void mbComChoiceDownSet(void);
void mbComChoiceListDownSet(int downNum);
void mbComChoiceListRightSet(int rightNum);
void mbComChoiceListSet(int rightNum, int downNum);

#endif
