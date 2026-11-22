#ifndef _GAME_BOARD_BRANCH_H
#define _GAME_BOARD_BRANCH_H

#include "game/board/board.h"

typedef int (*MBBRANCHCOMHOOK)(int playerNo, int linkNum, s16 *linkTbl, BOOL partyF);

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbBranchInit(void);
BOOL mbev_Branch(int playerNo, s16 *masuId);
BOOL mbev_BranchDebug(int playerNo, s16 *masuId);
void mbBranchAttrSet(u16 attr);
void mbBranchMAttrSet(u32 mAttr);
void mbBranchAttrReset(u16 attr);
void mbBranchMAttrReset(u32 mAttr);
void mbBranchAttrInit(void);
void mbBranchMAttrInit(void);
u16 mbBranchAttrGet(void);
u32 mbBranchMAttrGet(void);
BOOL mbBranchAttrCheck(int masuId);
void mbBranchComStarHookSet(MBBRANCHCOMHOOK hook);

#endif
