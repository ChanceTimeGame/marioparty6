#ifndef _GAME_BOARD_CONFIG_H
#define _GAME_BOARD_CONFIG_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
int mbConfigExec(int arg0, int arg1);
void mbPauseDispCopyCreate(void);
void mbPauseDispCopyKill(void);
void mbPauseGuideCreate(void);
void mbPauseGuideKill(void);
s16 mbPausePanelCreate(int arg0, int arg1);
void mbPausePanelKill(int arg0);
void mbPausePanelPosSet(s16 arg0, float f0, float f1);
void mbPausePanelPosGet(s16 arg0, HuVecF *arg1);
void mbPausePanelRotSet(int arg0, float f0, float f1, float f2);
void mbPausePanelScaleSet(s16 arg0, float f0);
float mbPausePanelScaleGet(s16 arg0);
void mbPausePanelBankSet(int arg0, s16 arg1);
void mbPausePanelBatsuSet(s16 arg0, int arg1);
int mbPausePanelFreezeGet(s16 arg0);
void mbPausePanelUnlockSet(s16 arg0);
void mbPausePanelSlideSet(s16 arg0, s16 arg1, HuVecF *arg2);
void mbPausePanelSizeSet(s16 arg0, s16 arg1, float f0);
void mbPausePanelAnmNoSet(int arg0, s16 arg1, s16 arg2);
void mbPausePanelGrowSet(int arg0, s16 arg1, int arg2, float f0);
void mbPausePanelShrinkSet(s16 arg0, s16 arg1, int arg2);
void mbPauseGuideMoveSet(int arg0, int arg1, HuVecF *arg2, HuVecF *arg3);
void mbPauseGuideTalkSet(int arg0);
void mbConfigPadDisableSet(int arg0);

#endif
