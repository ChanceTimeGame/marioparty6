#ifndef _GAME_BOARD_COIN_H
#define _GAME_BOARD_COIN_H

#include "game/board/board.h"

#define MBCOIN_DISP_NONE -1

typedef s16 MBCOINID;

typedef struct MbCoinObj_s {
    HuVecF pos; /* 0x00 */
    HuVecF rot; /* 0x0C */
    HuVecF scale; /* 0x18 */
    float alpha; /* 0x24 */
    u8 work[0x18]; /* 0x28: free for the caller */
} MBCOINOBJ; /* size 0x40 */

void mbCoinInit(void);
void mbCoinClose(void);
void mbCoinEffObjCreate(int objId);
MBCOINID mbCoinCreate(void);
MBCOINID mbCoinCreate2(void);
MBCOINID mbCoinObjCreate(int digit, int motNo);
MBCOINID mbCoinObjCreate2(int sign, int motNo);
MBCOINOBJ *mbCoinObjGet(MBCOINID id);
void mbCoinObjNumDec(MBCOINID id);
void mbCoinObjKill(MBCOINID id);
void mbCoinObjPosSet(MBCOINID id, float x, float y, float z);
void mbCoinObjPosSetV(MBCOINID id, HuVecF *pos);
void mbCoinObjPosGet(MBCOINID id, HuVecF *pos);
void mbCoinObjRotSet(MBCOINID id, float x, float y, float z);
void mbCoinObjRotSetV(MBCOINID id, HuVecF *rot);
void mbCoinObjRotGet(MBCOINID id, HuVecF *rot);
void mbCoinObjScaleSet(MBCOINID id, float x, float y, float z);
void mbCoinObjScaleSetV(MBCOINID id, HuVecF *scale);
void mbCoinObjScaleGet(MBCOINID id, HuVecF *scale);
void mbCoinObjAlphaSet(MBCOINID id, float alpha);
float mbCoinObjAlphaGet(MBCOINID id);
void mbCoinObjDispSet(MBCOINID id, BOOL dispF);
BOOL mbCoinObjDispGet(MBCOINID id);
void mbCoinObjLayerSet(MBCOINID id, u16 layer);
void mbCoinObjMotSet(MBCOINID id, int motNo);
void mbCoinEffCreate(HuVecF *pos);
s16 mbCoinDispCreate(HuVecF *pos, int coinNum, int signNum, BOOL seF);
s16 mbCoinDispMasuCreate(HuVecF *pos, int coinNum, BOOL seF);
s16 mbCoinDispCapsuleCreate(HuVecF *pos, int coinNum);
void mbCoinDispKill(s16 no);
BOOL mbCoinDispKillCheck(s16 no);
int mbCoinAddProcExec(int playerNo, int coinNum, int signNum, BOOL fastF);
int mbCoinAddDispExec(int playerNo, int coinNum, BOOL dispF, BOOL fastF);
int mbCoinAddExec(int playerNo, int coinNum);
int mbStatTeamMinValGet(int teamNo, int base, int max, int *value, int *out);
void mbCoinAddAllProcExecV(int *coinNum, int *signNum, BOOL fastF);
void mbCoinAddAllProcExec(int coinNumP1, int coinNumP2, int coinNumP3, int coinNumP4, BOOL dispF, BOOL fastF);
void mbCoinAddAllExec(int coinNumP1, int coinNumP2, int coinNumP3, int coinNumP4);

#endif
