#ifndef _GAME_BOARD_CAPSULE_H
#define _GAME_BOARD_CAPSULE_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
int mbCapUse(int arg0, s16 arg1);
void MBCapsuleStub1(void);
int MBCapsuleStub2(void);
int mbCapEffUseCreate(int arg0, s16 arg1);
int mbCapEffUseModeGet(int arg0);
int mbCapEffUsePosGet(int arg0, HuVecF *arg1);
int mbCapEffUseWanWanCreate(int arg0, s16 arg1, HuVecF *arg2);
void mbCapPlayerThrow(int arg0, int arg1, s16 arg2);
int mbCapPlayerThrowCheck(void);
int mbCapThrowColCreate(int arg0);
int mbCapThrowColCheck(int arg0, int arg1, int arg2);
void mbCapThrowHookSet(void *arg0);
void mbCapEffThrowCreate(HuVecF *arg0, HuVecF *arg1);
int mbCapEffThrowCheck(HuVecF *arg0, void *arg1);
void mbCapAutoThrow(HuVecF *arg0, HuVecF *arg1, HuVecF *arg2, int arg3, int arg4, int arg5, int arg6, float f0);
void mbCapAutoThrowEnd(void);
int mbCapSelectMasu(int arg0, u8 arg1);
void mbCapSelectMasuInit(void);
void mbCapListInit(struct CapsuleList_s *list);
void mbCapListRead(void);
int mbCapListCopy(struct CapsuleList_s *list);
void mbCapListDebug(void);
void mbCapNumInc(int capsuleNo, int type);
void mbCapNumDebug(void);
s16 mbCapValueTypeGet(s16 value);
s16 mbCapMasuTypeGet(s16 masuId);
s16 mbCapValuePlayerGet(s16 value);
s16 mbCapMasuPlayerGet2(s16 masuId);
void mbCapMasuPlayerSet(s16 masuId, s16 playerNo);
void mbCapMasuPlayerTypeSet(s16 masuId, s16 type, s16 playerNo);
s16 mbCapMasuPlayerGet(s16 masuId);
s16 mbCapUseModeGet(s16 capsuleNo);
BOOL mbCapUseTrapCheck(s16 capsuleNo);
int mbCapBuyCostGet(s16 capsuleNo, s16 playerNo);
int mbCapSellCostGet(s16 capsuleNo, s16 playerNo);
int mbCapCostGet(s16 capsuleNo);
int mbCapListExcludeCheck(s16 capsuleNo);
BOOL mbCapThrowMasuCheck(int masuId);
int mbCapComChanceGet(int capsuleNo, int playerNo, int mode);
int mbCapSelectComGet(int playerNo, int *capsuleTbl, int num);
int mbCapSelectDeleteComGet(int playerNo, int *capsuleTbl, int num);
void mbCapRandomThrowAdd(int arg0, int arg1, int arg2);
void mbCapRandomTrapAdd(int arg0, int arg1, int arg2);
void mbCapMasuCapsuleSet(s16 arg0, s16 arg1, s16 arg2);
int mbCapFileGet(int capsuleNo);
int mbCapDescMesGet(s16 arg0);
int mbCapUseMesGet(int capsuleNo);
int mbCapDebugNameGet(s16 arg0);
int mbCapMasuPatGet(s16 arg0);
int mbCapColorGet(int arg0);
int mbCapUseCostGet(void);
int mbCapUseCheck(s16 arg0);
int mbCapValidCheck(s16 arg0);
s16 mbCapMasuDispTypeGet(s16 arg0);
int mbCapSelectMasuNum(int arg0);
s16 mbCapSelectMasuFrontNum(s16 arg0);
int mbCapSelectMasuBackNum(s16 masuId);
int mbCapValidListGet(int arg0);
int mbCapNextGet(int arg0);
int mbCapMasuNextGet(int arg0);
int mbCapShopListGet(int arg0, void *arg1);
int mbCapRandomListGet(void *arg0, int arg1);
int mbCapBonusCoinNumGet(int arg0, s16 arg1);
int mbCapDescWinCreate(s16 arg0);
void mbCapInit(void);
int mbCapObjCreate(int capsuleNo, int arg1);
s16 mbCapObjBorderCreate(s16 arg0, int arg1);
void mbCapObjKill(int arg0);
void mbCapObjBorderKill(int arg0);
s16 mbCapObjColorCreate(s16 arg0, int arg1);
void mbCapObjColorKill(int arg0);
void mbCapObjColorPosSet(int arg0, float f0, float f1, float f2);
void mbCapObjColorRotSet(int arg0, float f0, float f1, float f2);
void mbCapObjColorScaleSet(int arg0, float f0, float f1, float f2);
void mbCapObjColorPosSetV(int arg0, HuVecF *arg1);
void mbCapObjColorRotSetV(int arg0, HuVecF *arg1);
void mbCapObjColorScaleSetV(int arg0, HuVecF *arg1);
void mbCapObjColorPosGet(int arg0, HuVecF *arg1);
void mbCapObjColorRotGet(int arg0, HuVecF *arg1);
void mbCapObjColorScaleGet(int arg0, HuVecF *arg1);
void mbCapObjColorLayerSet(int arg0, int arg1);
u8 mbCapObjColorLayerGet(int arg0);
void mbCapObjColorAttrSet(int arg0, int arg1);
void mbCapObjColorAttrReset(int arg0, int arg1);
void mbCapObjColorDispSet(int arg0, int arg1);
void mbCapObjColorAlphaSet(int arg0, int arg1);
void mbCapObjColorMtxSet(int arg0, void *arg1);
void mbCapObjColorMtxGet(int arg0, void *arg1);

#endif
