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
    s8 rank; //0x1C
    u16 useMode; //0x1E
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
int mbCapSelectComGet(int arg0, int *arg1, int arg2);
int mbCapSelectDeleteComGet(int arg0, int *arg1, int arg2);
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
s16 mbCapMasuDispTypeGet(s16 masuId);
int mbCapSelectMasuNum(int masuId);
int mbCapSelectMasuFrontNum(int masuId);
int mbCapSelectMasuBackNum(int masuId);
int mbCapValidListGet(int *list);
int mbCapNextGet(int chanceNo);
int mbCapMasuNextGet(int playerNo);
int mbCapShopListGet(int playerNo, struct CapsuleList_s *out);
int mbCapRandomListGet(int *out, int max);
int mbCapBonusCoinNumGet(int playerNo, int capsuleNo);
int mbCapDescWinCreate(int capsuleNo);
void mbCapInit(void);
int mbCapObjCreate(int capsuleNo, int arg1);
s16 mbCapObjBorderCreate(int objId, int capsuleNo);
void mbCapObjKill(int objId);
void mbCapObjBorderKill(int objId);
int mbCapObjColorCreate(int capsuleNo, BOOL linkF);
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
