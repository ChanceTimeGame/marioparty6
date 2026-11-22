#ifndef _GAME_BOARD_DICE_H
#define _GAME_BOARD_DICE_H

#include "game/board/board.h"

#define DICE_PLAYERNO_NULL GW_PLAYER_MAX
#define DICE_MAX (DICE_PLAYERNO_NULL+1)
#define DICE_VALUENUM_MAX 12
#define DICE_NUM_MAX 3

#define DICETYPE_PATAPATA_COIN 7
#define DICETYPE_CHANCE_TRADE 8
#define DICETYPE_CHANCE_CHAR 9
#define DICETYPE_BLOCK 10
#define DICETYPE_KURIBO 13
#define DICETYPE_ORDER 14
#define DICETYPE_MAX 21

#define DICE_RESULT_FAIL1 -1
#define DICE_RESULT_FAIL2 -2
#define DICE_RESULT_X -3
#define DICE_RESULT_Y -4
#define DICE_RESULT_FAIL5 -5
#define DICE_RESULT_CAPSULESEL -6

typedef void (*MBDICEHITHOOK)(int result);
typedef u16 (*MBDICEPADBTNHOOK)(int playerNo);
typedef void (*MBDICEMOTHOOK)(int playerNo);

void mbDiceInit(void);
int mbDiceProcExec(int playerNo, int diceType, s8 *valueTbl, int *tutorialNum, BOOL padWinF, BOOL waitF, HuVecF *pos, int color);
int mbDiceExec(int playerNo, int diceType, s8 *valueTbl, int tutorialNum, BOOL padWinF, BOOL waitF, HuVecF *pos, int color);
int mbDicePlayerExec(int playerNo, int diceType);
int mbDiceChanceTradeExec(int playerNo);
int mbDiceChanceCharExec(int playerNo, int *playerNoTbl);
void mbDiceFadeSet(int playerNo);
void mbDiceKill(int playerNo);
void mbDiceClose(void);
BOOL mbDiceKillCheck(int playerNo);
BOOL mbDiceKillCheckAll(void);
int mbDiceResultGet(int playerNo);
void mbDiceHitHookSet(int playerNo, MBDICEHITHOOK hook);
void mbDiceObjHit(int playerNo);
void mbDicePadBtnHookSet(int playerNo, MBDICEPADBTNHOOK hook);
void mbDiceMotHookSet(int playerNo, MBDICEMOTHOOK hook);
int mbDiceTypeGet(int playerNo);
void mbDiceTutorialNumSet(int playerNo, int num);
int mbDiceTutorialNumGet(int playerNo);
int mbDiceValueNoGet(int playerNo);
int mbDiceMaxGet(int diceType);
int mbDiceValueMaxGet(int diceType);
OMOBJ *mbDiceNumObjCreate(int playerNo, HuVecF *pos1, HuVecF *pos2, int value, int color, BOOL followF);
void mbDiceNumObjKill(OMOBJ *obj);
BOOL mbDiceNumStopCheck(int playerNo);
void mbDiceNumKill(int playerNo);
void mbDiceNumShrinkSet(int playerNo);
void mbDiceStub(void);
OMOBJ *mbDiceSNpcNumCreate(int playerNo, HuVecF *pos);
OMOBJ *mbDiceSNpcNumObjCreate(HuVecF *pos, HuVecF *ofs, int value, int flag, int color);
void mbDiceSNpcNumKill(OMOBJ *obj);
void mbDiceSNpcNumDispSet(OMOBJ *obj, int dispF);
void mbDiceSNpcNumSet(OMOBJ *obj, int value);
void mbDiceSNpcNumPosSet(OMOBJ *obj, HuVecF *pos);
void mbDiceSNpcNumOfsSet(OMOBJ *obj, HuVecF *ofs);
void mbDiceSNpcNumOfsGet(OMOBJ *obj, HuVecF *ofs);

#endif
