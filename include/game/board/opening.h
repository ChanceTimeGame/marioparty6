#ifndef _GAME_BOARD_OPENING_H
#define _GAME_BOARD_OPENING_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbev_Opening(void);
void mbev_OpeningParty(void);
void mbOpeningViewSet(HuVecF *rot, HuVecF *pos, float zoom);
void mbev_OpeningSingle(void);
float mbOpeningZoomGet(void);
void mbOpeningRotGet(HuVecF *rot);
void mbOpeningPosGet(HuVecF *pos);
void mbOpeningInstHookSet(void *hook);
void mbOpeningStarInstHookSet(void *hook);
s16 mbOpeningGuideObjIdGet(void);
void mbOpeningGuidePosRestore(void);
void mbOpeningCameraPosRestore(void);

#endif
