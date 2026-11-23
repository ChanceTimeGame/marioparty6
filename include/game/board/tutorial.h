#ifndef _GAME_BOARD_TUTORIAL_H
#define _GAME_BOARD_TUTORIAL_H

#include "game/board/board.h"
#include "game/object.h"

void mbTutorialInit(void);
void mbTutorialMainFuncSet(void (*func)(void));
void mbTutorialCreate(void);
int mbTutorialCall(int scene);
void mbTutorialCallEnd(void);
void mbTutorialVSleep(void);
void mbTutorialCallWait(int scene);
void mbTutorialMultiCall(int scene, int count);
void mbTutorialCallResult(int scene, int result);
void mbTutorialMesCall(int scene, u32 mess);
void mbTutorialMesMasuCall(int scene, u32 mess, int dataNum);
void mbTutorialTurnCall(int turnNo);
void mbTutorialGuideCall(int result);
void mbTutorialCapsuleUseCall(int capsuleNo);
void mbTutorialResultSet(int result);
int mbTutorialSceneGet(void);
void mbTutorialModeSet(int mode);
int mbTutorialModeGet(void);
s16 mbTutorialViewSet(void);
void mbTutorialViewMasuSet(s16 masuId);
s16 mbTutorialSprCreate(int dataNum);
s16 mbTutorialSprDispOn(int dataNum);
void mbTutorialSprKill(s16 sprId);
void mbTutorialSprDispOff(s16 sprId);
void mbTutorialSprGrpSet(s16 grpId);
void mbTutorialSprGrpKill(s16 grpId);
s16 mbTutorialModelCreate(int dataNum, BOOL linkF);
void mbTutorialModelKill(s16 mdlId);
OMOBJ *mbTutorialGuideCreate(s8 *motTbl, BOOL screenF);
void mbTutorialGuideClose(OMOBJ *obj);
OMOBJ *mbTutorialGuideGet(void);
BOOL mbTutorialExitReqGet(void);
void mbTutorialExitSet(void);
void mbTutorialExitOnSet(BOOL flag);
BOOL mbTutorialWinWait(int winNo);
void mbTutorialWinClose(int winNo);
void mbTutorialWinMesExec(int mess);
void mbTutorialWinMesMasuExec(int mess, int dataNum);
int mbTutorialWinCreate(int mess);
void mbTutorialWinKeyWait(int winNo);
void mbTutorialMgCallInit(void);
void mbTutorialMgCallClose(void);
void mbTutorialMgCallExec(int type);

#endif
