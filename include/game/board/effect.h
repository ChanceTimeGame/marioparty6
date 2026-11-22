#ifndef _GAME_BOARD_EFFECT_H
#define _GAME_BOARD_EFFECT_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbEffInit(void);
void mbEffClose(void);
void mbEffFadeOutSet(s16 maxTime);
void mbEffFadeCreate(s16 maxTime, u8 alpha);
BOOL mbEffFadeDoneCheck(void);
void mbEffFadeCameraSet(u16 bit);
int mbEffFadeCheck(void);
void mbEffConfettiCreate(HuVecF *pos, s16 maxCnt, float width);
void mbEffConfettiKill(void);
void mbEffConfettiReset(void);
s16 mbParticleCreate(void *arg0, int arg1);
void mbParticleKill(s16 arg0);
void mbParticleColorCreate(s16 arg0);
void mbParticleHookSet(int arg0, void *arg1);
void mbParticleAttrSet(s16 arg0, int arg1);
void mbParticleAttrReset(s16 arg0, int arg1);
int mbParticleDataCreate(void *arg0);
int mbParticleUnkTotalGet(void *arg0, int arg1);
int mbParManCreate(void *arg0, int arg1, int arg2);
void mbParManKill(s16 arg0);
void mbParticleBlendModeSet(s16 arg0, u8 arg1);
void mbParManPosSet(s16 arg0);
void mbParManVecSet(s16 arg0, float f0, float f1, float f2);
void mbParManRotSet(s16 arg0);
void mbParManAttrSet(s16 arg0, int arg1);
void mbParManAttrReset(s16 arg0, int arg1);

#endif
