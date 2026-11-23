#ifndef _GAME_BOARD_WIPE_H
#define _GAME_BOARD_WIPE_H

#include "game/board/board.h"

void mbWipeCreate(s16 mode, s16 type, s16 maxTime);
void mbWipeWait(void);
void mbWipeFadeOut(void);
void mbWipeFadeIn(void);
void mbWipeWhiteFadeOut(void);
void mbWipeWhiteFadeIn(void);
void mbWipeDissolveFadeOut(void);
void mbWipeDissolveFadeIn(void);
void mbWipeFadeOutTime(int maxTime);
void mbWipeFadeInTime(int maxTime);
void mbWipeWhiteFadeOutTime(int maxTime);
void mbWipeWhiteFadeInTime(int maxTime);
void mbWipeDissolveFadeOutTime(int maxTime);
void mbWipeDissolveFadeInTime(int maxTime);
void mbWipeSpecialInit(void);
void mbWipeSpecialClose(void);
void mbWipeSpecialKill(void);
void mbWipeSpecialCreate(int mode, int type, int maxTime);
void mbWipeSpecialFadeOutCreate(int type, int maxTime);
void mbWipeSpecialFadeInCreate(int type, int maxTime);
int mbWipeSpecialCheck(void);
void mbWipeSpecialWait(void);
int mbWipeSpecialStatGet(void);

#endif
