#ifndef _GAME_BOARD_BRANCH_H
#define _GAME_BOARD_BRANCH_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbBranchInit(void);
int mbev_Branch(int arg0, void *arg1);
int mbev_BranchDebug(int arg0, void *arg1);
void mbBranchAttrSet(int arg0);
void mbBranchMAttrSet(int arg0);
void mbBranchAttrReset(int arg0);
void mbBranchMAttrReset(int arg0);
void mbBranchAttrInit(void);
void mbBranchMAttrInit(void);
u16 mbBranchAttrGet(void);
int mbBranchMAttrGet(void);
int mbBranchAttrCheck(s16 arg0);
void mbBranchComStarHookSet(void *arg0);

#endif
