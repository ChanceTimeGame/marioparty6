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
void mbCapListInit(int arg0);
void mbCapListRead(void);
int mbCapListCopy(int arg0);
void mbCapListDebug(void);
void mbCapNumInc(int arg0, int arg1);
void mbCapNumDebug(void);
s16 mbCapValueTypeGet(s16 value);
s16 mbCapMasuTypeGet(s16 masuId);
s16 mbCapValuePlayerGet(s16 arg0);
s16 mbCapMasuPlayerGet2(int arg0);
void mbCapMasuPlayerSet(int arg0, u8 arg1);
void mbCapMasuPlayerTypeSet(s16 masuId, s16 capsuleNo, s16 playerNo);
s16 mbCapMasuPlayerGet(s16 arg0);
s16 mbCapUseModeGet(s16 capsuleNo);
int mbCapUseTrapCheck(u8 arg0);
int mbCapBuyCostGet(s16 arg0, int arg1);
int mbCapSellCostGet(s16 arg0, int arg1);
int mbCapCostGet(u8 arg0);
u8 mbCapListExcludeCheck(u8 arg0);
int mbCapThrowMasuCheck(int arg0);
int mbCapComChanceGet(int arg0, int arg1, int arg2);
int mbCapSelectComGet(int arg0, void *arg1, int arg2);
int mbCapSelectDeleteComGet(int arg0, void *arg1, int arg2);
void mbCapRandomThrowAdd(int capsuleNo, int playerNo, BOOL overwriteF);
void mbCapRandomTrapAdd(int capsuleNo, int playerNo, BOOL overwriteF);
void mbCapMasuCapsuleSet(int masuId, int capsuleNo, int playerNo);
int mbCapFileGet(int capsuleNo);
u32 mbCapDescMesGet(int capsuleNo);
u32 mbCapUseMesGet(int capsuleNo);
char *mbCapDebugNameGet(int capsuleNo);
int mbCapMasuPatGet(int capsuleNo);
int mbCapColorGet(int capsuleNo);
int mbCapUseCostGet(void);
BOOL mbCapUseCheck(int capsuleNo);
BOOL mbCapValidCheck(int capsuleNo);
int mbCapMasuDispTypeGet(s16 masuId);
int mbCapSelectMasuNum(s16 masuId);
s16 mbCapSelectMasuFrontNum(s16 masuId);
s16 mbCapSelectMasuBackNum(s16 masuId);
int mbCapValidListGet(int *list);
int mbCapNextGet(int chanceNo);
int mbCapMasuNextGet(int playerNo);
int mbCapShopListGet(int playerNo, struct CapsuleList_s *out);
int mbCapRandomListGet(int *out, int max);
int mbCapBonusCoinNumGet(int playerNo, s16 capsuleNo);
int mbCapDescWinCreate(int capsuleNo);
void mbCapInit(void);
int mbCapObjCreate(int capsuleNo);
s16 mbCapObjBorderCreate(s16 objId, int capsuleNo);
void mbCapObjKill(int objId);
void mbCapObjBorderKill(int objId);
s16 mbCapObjColorCreate(int capsuleNo, BOOL linkF);
void mbCapObjColorKill(int objId);
void mbCapObjColorPosSet(int objId, float x, float y, float z);
void mbCapObjColorRotSet(int objId, float x, float y, float z);
void mbCapObjColorScaleSet(int objId, float x, float y, float z);
void mbCapObjColorPosSetV(int objId, HuVecF *pos);
void mbCapObjColorRotSetV(int objId, HuVecF *rot);
void mbCapObjColorScaleSetV(int objId, HuVecF *scale);
void mbCapObjColorPosGet(int objId, HuVecF *pos);
void mbCapObjColorRotGet(int objId, HuVecF *rot);
void mbCapObjColorScaleGet(int objId, HuVecF *scale);
void mbCapObjColorLayerSet(int objId, u8 layer);
u8 mbCapObjColorLayerGet(int objId);
void mbCapObjColorAttrSet(int objId, u32 attr);
void mbCapObjColorAttrReset(int objId, u32 attr);
void mbCapObjColorDispSet(int objId, BOOL dispF);
void mbCapObjColorAlphaSet(int objId, u8 alpha);
void mbCapObjColorMtxSet(int objId, Mtx mtx);
void mbCapObjColorMtxGet(int objId, Mtx mtx);

#endif
