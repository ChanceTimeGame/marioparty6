#ifndef _GAME_BOARD_GUIDE_H
#define _GAME_BOARD_GUIDE_H

#include "game/board/board.h"

#define MB_GUIDE_ATTR_SCREEN (1 << 0)
#define MB_GUIDE_ATTR_FADEIN (1 << 1)
#define MB_GUIDE_ATTR_MOT1 (1 << 2)

void mbGuideInit(void);
OMOBJ *mbGuideCreate(int guideNo, HuVecF *pos, HuVecF *rot, s8 *motTbl, float scale, u32 attr);
OMOBJ *mbGuideCreateFlag(HuVecF *pos, s8 *motTbl, BOOL screenF, BOOL fadeInF, BOOL motF);
OMOBJ *mbGuideCreateIn(void);
void mbGuideKill(OMOBJ *obj);
void mbGuideEnd(OMOBJ *obj);
void mbGuideFadeIn(OMOBJ *obj);
void mbGuideFadeOut(OMOBJ *obj);
s16 mbGuideModelGet(OMOBJ *obj);
void mbGuideScreenSet(OMOBJ *obj, BOOL screenF);
BOOL mbGuideIdleCheck(OMOBJ *obj);
void mbGuideMotionNextSet(OMOBJ *obj, s16 motNo);
void mbGuideMotionSet(OMOBJ *obj, s16 motNo, BOOL shiftF);
void mbGuideMotionShiftSet(OMOBJ *obj, s16 motNo, BOOL shiftF);
void mbGuideMotionStop(OMOBJ *obj);
BOOL mbGuideMotionCheck(OMOBJ *obj);
int mbGuideNoGet(void);
int mbGuideSpeakerNoGet(void);

#endif
