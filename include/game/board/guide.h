#ifndef _GAME_BOARD_GUIDE_H
#define _GAME_BOARD_GUIDE_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbGuideInit(void);
int mbGuideCreate(int arg0, HuVecF *arg1, HuVecF *arg2, void *arg3, int arg4, float f0);
int mbGuideCreateFlag(HuVecF *arg0, void *arg1, int arg2, int arg3, int arg4);
int mbGuideCreateIn(const HuVecF *pos, BOOL screenF, BOOL effInF, BOOL modelOffF);
void mbGuideKill(void *arg0);
void mbGuideEnd(void *arg0);
void mbGuideFadeIn(int arg0);
void mbGuideFadeOut(void *arg0);
s16 mbGuideModelGet(void *arg0);
void mbGuideScreenSet(void *arg0, int arg1);
int mbGuideIdleCheck(int arg0);
void mbGuideMotionNextSet(int arg0, int arg1);
void mbGuideMotionSet(void *arg0, s16 arg1, int arg2);
void mbGuideMotionShiftSet(void *arg0, s16 arg1, int arg2);
void mbGuideMotionStop(int arg0);
int mbGuideMotionCheck(int arg0);
int mbGuideNoGet(void);
int mbGuideSpeakerNoGet(void);

#endif
