#ifndef _GAME_BOARD_CONFIG_H
#define _GAME_BOARD_CONFIG_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
int mbConfigExec(int playerNo, s16 guideObjId);
void mbPauseDispCopyCreate(void);
void mbPauseDispCopyKill(void);
void mbPauseGuideCreate(void);
void mbPauseGuideKill(void);
s16 mbPausePanelCreate(int dataNum, u32 sprDataNum);
void mbPausePanelKill(s16 panelId);
void mbPausePanelPosSet(s16 panelId, float posX, float posY);
void mbPausePanelPosGet(s16 panelId, HuVecF *pos);
void mbPausePanelRotSet(s16 panelId, float rotX, float rotY, float rotZ);
void mbPausePanelScaleSet(s16 panelId, float scale);
float mbPausePanelScaleGet(s16 panelId);
void mbPausePanelBankSet(s16 panelId, int bank);
void mbPausePanelBatsuSet(s16 panelId, BOOL batsuF);
BOOL mbPausePanelFreezeGet(s16 panelId);
void mbPausePanelUnlockSet(s16 panelId);
void mbPausePanelSlideSet(s16 panelId, int time, HuVecF *pos);
void mbPausePanelSizeSet(s16 panelId, int time, float scale);
void mbPausePanelAnmNoSet(s16 panelId, int time, int bank);
void mbPausePanelGrowSet(s16 panelId, int time, int delay, float scale);
void mbPausePanelShrinkSet(s16 panelId, int time, int delay);
void mbPauseGuideMoveSet(s16 objId, int time, HuVecF *pos, HuVecF *normPos);
void mbPauseGuideTalkSet(s16 objId);
void mbConfigPadDisableSet(BOOL disableF);

#endif
