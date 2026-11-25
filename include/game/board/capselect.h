#ifndef _GAME_BOARD_CAPSELECT_H
#define _GAME_BOARD_CAPSELECT_H

#include "game/board/board.h"

int mbCapSelect(void);
int mbCapDelete(int extraCapsule, BOOL loopF);
float mbCapSelectGrow(HuVecF *posStart, HuVecF *posEnd, s16 objId, float weight, float scaleBase);
void mbCapSelectResultSet(int playerNo, int objId, int selNo);
void mbCapSelectResultGet(int playerNo, int *objId, int *selNo);
void mbCapSelectResultReset(int playerNo);
BOOL mbCapSelectShrinkCheck(int playerNo);
void fn_8019A618(void);
void fn_8019A61C(void);
void fn_8019A620(void);
void fn_8019A624(void);
void fn_8019A628(void);
void fn_8019A62C(void);
void mbCapMasuExec(int playerNo, int masuId);
void mbCapMasuObjInit(void);
void mbCapMasuObjClose(void);
void mbCapMasuObjCreateAll(void);
void mbCapMasuObjCreate(int masuId);
void mbCapCapsuleGet(int playerNo, int capsuleNo);
void mbCapMasuDispSet(BOOL dispF);

#endif
