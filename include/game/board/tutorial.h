#ifndef _GAME_BOARD_TUTORIAL_H
#define _GAME_BOARD_TUTORIAL_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbTutorialInit(void);
void mbTutorialMainFuncSet(void *arg0);
void mbTutorialCreate(void);
int mbTutorialCall(int arg0);
void mbTutorialCallEnd(void);
void mbTutorialVSleep(void);
void mbTutorialCallWait(int arg0);
void mbTutorialMultiCall(int arg0, int arg1);
void mbTutorialCallResult(int arg0, int arg1);
void mbTutorialMesCall(int arg0, int arg1);
void mbTutorialMesMasuCall(int arg0, int arg1, int arg2);
void mbTutorialTurnCall(int arg0);
void mbTutorialGuideCall(int arg0);
void mbTutorialCapsuleUseCall(int arg0);
void mbTutorialResultSet(int arg0);
int mbTutorialSceneGet(void);
void mbTutorialModeSet(int arg0);
int mbTutorialModeGet(void);
s16 mbTutorialViewSet(void);
void mbTutorialViewMasuSet(int arg0);
int mbTutorialSprCreate(int arg0);
s16 mbTutorialSprDispOn(int dataNum);
void mbTutorialSprKill(s16 arg0);
void mbTutorialSprDispOff(s16 sprId);
void mbTutorialSprGrpSet(int arg0);
void mbTutorialSprGrpKill(s16 arg0);
int mbTutorialModelCreate(int arg0, int arg1);
void mbTutorialModelKill(s16 arg0);
int mbTutorialGuideCreate(void *arg0, int arg1);
void mbTutorialGuideClose(void *arg0);
int mbTutorialGuideGet(void);
int mbTutorialExitReqGet(void);
void mbTutorialExitSet(void);
void mbTutorialExitOnSet(int arg0);
int mbTutorialWinWait(s16 arg0);
void mbTutorialWinClose(s16 arg0);
void mbTutorialWinMesExec(int arg0);
void mbTutorialWinMesMasuExec(int arg0, int arg1);
int mbTutorialWinCreate(int arg0);
void mbTutorialWinKeyWait(s16 arg0);
void mbTutorialMgCallInit(void);
void mbTutorialMgCallClose(void);
void mbTutorialMgCallExec(int arg0);

#endif
