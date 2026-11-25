#ifndef _GAME_BOARD_CAPSULE_H
#define _GAME_BOARD_CAPSULE_H

#include "game/board/board.h"

typedef struct CapsuleData_s {
    u32 file; //0x00
    u32 iconFile; //0x04
    u32 descMes; //0x08
    u32 useMes; //0x0C
    s32 masuPat; //0x10
    s32 color; //0x14
    s32 cost; //0x18
    char rank; //0x1C
    s16 useMode; //0x1E
    char *debugName; //0x20
    u8 listF; //0x24
} CAPSULE_DATA; //size=0x28

typedef struct CapsuleChance_s {
    s16 chance;
    char mark[2];
} CAPSULE_CHANCE;

typedef struct CapsuleChanceTbl_s {
    s16 capsuleNo;
    CAPSULE_CHANCE chance[11];
} CAPSULE_CHANCE_TBL; //size=0x2E

typedef struct CapsuleList_s {
    s8 capsuleNo;
    s8 rate[3];
    s8 boardRate[8];
    s8 turnRate[4];
} CAPSULE_LIST; //size=0x10

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
int mbCapUse(int playerNo, int capsuleNo);
void MBCapsuleStub1(void);
int MBCapsuleStub2(void);
int mbCapEffUseCreate(int playerNo, int capsuleNo);
int mbCapEffUseModeGet(int playerNo);
int mbCapEffUsePosGet(int playerNo, HuVecF *pos);
int mbCapEffUseWanWanCreate(int playerNo, int capsuleNo, HuVecF *pos);
void mbCapPlayerThrow(int playerNo, int masuId, int capsuleNo);
int mbCapPlayerThrowCheck(void);
int mbCapThrowColCreate(int dataNum);
int mbCapThrowColCheck(HuVecF *pos, HuVecF *next, HuVecF *out);
void mbCapThrowHookSet(void (*hook)(BOOL startF));
void mbCapEffThrowCreate(HuVecF *start, HuVecF *end);
int mbCapEffThrowCheck(HuVecF *pos, int *maxTime);
void mbCapAutoThrow(HuVecF *start, HuVecF *mid, HuVecF *end, int playerNo, int masuId, int capsuleNo, int maxTime, float speed);
void mbCapAutoThrowEnd(void *work);
int mbCapSelectMasu(int playerNo, int capsuleNo);
void mbCapSelectMasuInit(void);
void mbCapListInit(CAPSULE_LIST *list);
void mbCapListRead(void);
int mbCapListCopy(CAPSULE_LIST *list);
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
void mbCapMasuCapsuleSet(int masuId, int capsuleNo, int playerNo);
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
int mbCapBonusCoinNumGet(int playerNo, int capsuleNo);
int mbCapDescWinCreate(s16 capsuleNo);
void mbCapInit(void);
int mbCapObjCreate(int capsuleNo, int arg1);
s16 mbCapObjBorderCreate(s16 arg0, int arg1);
void mbCapObjKill(int arg0);
void mbCapObjBorderKill(int arg0);
int mbCapObjColorCreate(int capsuleNo, int arg1);
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
