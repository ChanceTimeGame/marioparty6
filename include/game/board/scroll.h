#ifndef _GAME_BOARD_SCROLL_H
#define _GAME_BOARD_SCROLL_H

#include "game/board/board.h"

typedef void (*MBSCROLLHOOK)(BOOL startF);
typedef void (*MBMAPHOOK)(BOOL startF);
typedef s16 (*MBSCROLLSTARFINDFUNC)(int playerNo);

void mbScrollInit(int dataNum);
void mbScrollClose(void);
void mbev_Scroll(int playerNo, BOOL mapF);
void mbev_StarScroll(HuVecF *startPos, HuVecF *endPos, s16 maxTime);
void mbScrollStarFindFuncSet(MBSCROLLSTARFINDFUNC func);
void mbMapCameraSet(HuVecF *rot, HuVecF *pos, float zoom);
void mbMapHookSet(MBMAPHOOK hook);
void mbScrollHookSet(MBSCROLLHOOK hook);
void mbMapSprAdd(int charNo, s16 masuId);
void mbev_ScrollCapsule(int playerNo);

#endif
