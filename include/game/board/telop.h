#ifndef _GAME_BOARD_TELOP_H
#define _GAME_BOARD_TELOP_H

#include "game/board/board.h"
#include "game/sprite.h"

void mbTelopCreate(int playerNo, int telopNo, BOOL waitF);
void mbTelopPlayerCreate(int playerNo);
void mbTelopPlayerSkipCreate(int telopNo);
BOOL mbTelopCheck(void);
void mbTelopLastTurnCreate(void);
void mbTelopTimeCreate(void);
HUSPR_GROUPID mbTelopTimeSprCreate(void);
void mbTelopTimeSprKill(HUSPR_GROUPID grpId);
void mbTelopTimeSprRotSet(HUSPR_GROUPID grpId, float t);
void mbTelopTimeStarSet(HUSPR_GROUPID grpId, int num);
void mbTelopTimeTPLvlSet(HUSPR_GROUPID grpId, float tpLvl);
void mbTelopTimeDispSet(HUSPR_GROUPID grpId, BOOL dispF);
s8 mbPadStkXGet(int padNo);
s8 mbPadStkYGet(int padNo);
void mbTauntInit(void);
void mbTauntClose(void);
int mbLanguageGet(void);
void mbLanguageSet(int lang);
int mbBoardDataNumGet(int dataNum);
void mbBoardDataDirRead(void);
void mbTelopTimeChangeCreate(void);
void mbTelopTimeChangeKill(void);
BOOL mbTelopTimeChangeCheck(void);

#endif
