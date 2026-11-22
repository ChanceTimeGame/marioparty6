#ifndef _GAME_BOARD_COIN_H
#define _GAME_BOARD_COIN_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbCoinInit(void);
void mbCoinClose(void);
void mbCoinEffObjCreate(s16 arg0);
s16 mbCoinCreate(void);
s16 mbCoinCreate2(void);
s16 mbCoinObjCreate(int arg0, int arg1);
s16 mbCoinObjCreate2(int arg0, int arg1);
int mbCoinObjGet(int arg0);
void mbCoinObjNumDec(int arg0);
void mbCoinObjKill(int arg0);
void mbCoinObjPosSet(int arg0, float f0, float f1, float f2);
void mbCoinObjPosSetV(int arg0, HuVecF *arg1);
void mbCoinObjPosGet(int arg0, HuVecF *arg1);
void mbCoinObjRotSet(int arg0, float f0, float f1, float f2);
void mbCoinObjRotSetV(int arg0, HuVecF *arg1);
void mbCoinObjRotGet(int arg0, HuVecF *arg1);
void mbCoinObjScaleSet(int arg0, float f0, float f1, float f2);
void mbCoinObjScaleSetV(int arg0, HuVecF *arg1);
void mbCoinObjScaleGet(int arg0, HuVecF *arg1);
void mbCoinObjAlphaSet(int arg0, float f0);
float mbCoinObjAlphaGet(int arg0);
void mbCoinObjDispSet(int arg0, int arg1);
int mbCoinObjDispGet(int arg0);
void mbCoinObjLayerSet(int arg0, u16 arg1);
void mbCoinObjMotSet(int arg0, s8 arg1);
void mbCoinEffCreate(HuVecF *arg0);
s16 mbCoinDispCreate(HuVecF *arg0, int arg1, int arg2, int arg3);
int mbCoinDispMasuCreate(HuVecF *arg0, int arg1, int arg2);
int mbCoinDispCapsuleCreate(HuVecF *arg0, int arg1);
void mbCoinDispKill(s16 no);
BOOL mbCoinDispKillCheck(s16 no);
int mbCoinAddProcExec(int playerNo, int coinNum, BOOL dispF, BOOL fastF);
int mbCoinAddDispExec(int arg0, int arg1, int arg2, int arg3);
int mbCoinAddExec(int playerNo, int coinNum);
int mbStatTeamMinValGet(int arg0, int arg1, int arg2, void *arg3, int arg4);
void mbCoinAddAllProcExecV(void *arg0, void *arg1, int arg2);
void mbCoinAddAllProcExec(int coinNumP1, int coinNumP2, int coinNumP3, int coinNumP4, BOOL dispF, BOOL fastF);
void mbCoinAddAllExec(int coinNumP1, int coinNumP2, int coinNumP3, int coinNumP4);

#endif
