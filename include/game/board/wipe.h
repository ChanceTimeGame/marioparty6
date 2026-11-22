#ifndef _GAME_BOARD_WIPE_H
#define _GAME_BOARD_WIPE_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbWipeCreate(int arg0, int arg1, int arg2);
void mbWipeWait(void);
void mbWipeFadeOut(void);
void mbWipeFadeIn(void);
void mbWipeWhiteFadeOut(void);
void mbWipeWhiteFadeIn(void);
void mbWipeDissolveFadeOut(void);
void mbWipeDissolveFadeIn(void);
void mbWipeFadeOutTime(s16 arg0);
void mbWipeFadeInTime(s16 arg0);
void mbWipeWhiteFadeOutTime(s16 arg0);
void mbWipeWhiteFadeInTime(s16 arg0);
void mbWipeDissolveFadeOutTime(s16 arg0);
void mbWipeDissolveFadeInTime(s16 arg0);
void mbWipeSpecialInit(void);
void mbWipeSpecialClose(void);
void mbWipeSpecialKill(void);
void mbWipeSpecialCreate(int arg0, s16 arg1, s16 arg2);
void mbWipeSpecialFadeOutCreate(s16 arg0, s16 arg1);
void mbWipeSpecialFadeInCreate(s16 arg0, s16 arg1);
int mbWipeSpecialCheck(void);
void mbWipeSpecialWait(void);
int mbWipeSpecialStatGet(void);

#endif
