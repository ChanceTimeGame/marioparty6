#ifndef _GAME_BOARD_DICE_H
#define _GAME_BOARD_DICE_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbDiceInit(void);
int mbDiceProcExec(int arg0, int arg1, void *arg2, void *arg3, int arg4, int arg5, void *arg6, int arg7);
int mbDiceExec(int arg0, int arg1, void *arg2, int arg3, int arg4, int arg5, void *arg6, int arg7);
void mbDicePlayerExec(int arg0, int arg1);
void mbDiceChanceTradeExec(int arg0);
void mbDiceChanceCharExec(int arg0, void *arg1);
void mbDiceFadeSet(int arg0);
void mbDiceKill(int arg0);
void mbDiceClose(void);
int mbDiceKillCheck(int arg0);
int mbDiceKillCheckAll(void);
int mbDiceResultGet(int arg0);
void mbDiceHitHookSet(int arg0, void *arg1);
void mbDiceObjHit(int arg0);
void mbDicePadBtnHookSet(int arg0, void *arg1);
void mbDiceMotHookSet(int arg0, void *arg1);
int mbDiceTypeGet(int arg0);
void mbDiceTutorialNumSet(int arg0, int arg1);
int mbDiceTutorialNumGet(int arg0);
s16 mbDiceValueNoGet(int arg0);
s8 mbDiceMaxGet(int arg0);
s8 mbDiceValueMaxGet(int arg0);
int mbDiceNumObjCreate(u8 arg0, HuVecF *arg1, HuVecF *arg2, int arg3, u8 arg4, int arg5);
void mbDiceNumObjKill(void *arg0);
int mbDiceNumStopCheck(int arg0);
void mbDiceNumKill(int arg0);
void mbDiceNumShrinkSet(int arg0);
void mbDiceStub(void);
int mbDiceSNpcNumCreate(int arg0, HuVecF *arg1);
int mbDiceSNpcNumObjCreate(HuVecF *arg0, HuVecF *arg1, u8 arg2, u8 arg3, int arg4);
void mbDiceSNpcNumKill(void *arg0);
void mbDiceSNpcNumDispSet(void *arg0, u8 arg1);
void mbDiceSNpcNumSet(void *arg0, u8 arg1);
void mbDiceSNpcNumPosSet(void *arg0, HuVecF *arg1);
void mbDiceSNpcNumOfsSet(void *arg0, HuVecF *arg1);
void mbDiceSNpcNumOfsGet(void *arg0, HuVecF *arg1);

#endif
