#ifndef _GAME_BOARD_AUDIO_H
#define _GAME_BOARD_AUDIO_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbAudInit(void);
void mbMusBoardPlay(void);
void mbMusPlay(int chan, int id, s8 vol, u16 fadeSpeed);
void mbMusFadeOut(int chan, u16 speed);
void mbMusStop(int chan);
void mbMusFadeOutSpeed(int arg0, u16 arg1);
void mbMusStub(void);
void mbMusParamSet(int chan, s8 vol, u16 fadeSpeed);
void mbMusPauseFadeOut(int chan, BOOL pauseF, int speed);
s32 mbMusStatGet(int chan);
void mbAudClose(void);
BOOL mbMusCheck(int chan);
BOOL mbMusStopCheck(int chan);
int mbMusFadeCheck(int arg0);
BOOL mbMusEndCheck(int chan);
int mbMusJinglePlay(int arg0);
void mbMusJingleWait(int arg0);
s32 mbMusJingleStatGet(int streamNo);
int mbMusBoardFadeOut(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5);
int mbMusBoardFadeCheck(void);
void mbMusPauseSet(int arg0);
void mbAudStub1(void);
void mbAudStub2(void);
void mbAudFXObjSet(int arg0);
int mbAudFXObjCreate(int arg0, int arg1);
void mbAudFXObjVolSet(int arg0, int arg1);
void mbAudFXObjKill(int arg0);
void mbAudFXObjClose(void);
void mbAudFXObjEmitterSet(int arg0, HuVecF *arg1);
int mbAudFXObjEmitterCreate(int arg0, HuVecF *arg1, int arg2);
void mbAudFXObjEmitterUpdate(int arg0, HuVecF *arg1);
void mbAudFXObjEmiterPosSet(int arg0, HuVecF *arg1);
void mbAudFXObjRefSet(int arg0, int arg1);
int mbAudFXPlay(s16 seId);
void mbAudFXStop(int seNo);
void mbAudFXStopAll(int arg0);
int mbAudFXPosPlay(s16 seNo, Vec *pos);
u8 mbAudFXPosPanGet(Vec *pos);
int mbAudFXEmitterPlay(s16 arg0, HuVecF *arg1);
void mbAudFXVolSet(int seNo, s16 vol);
void mbAudFXPanning(int arg0, int arg1);
void mbAudFXPosPanning(int arg0, HuVecF *arg1);
void mbAudFXDelaySet(int arg0);
int mbAudGuidePlay(s16 arg0);

#endif
