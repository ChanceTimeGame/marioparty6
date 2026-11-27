#ifndef _GAME_BOARD_SNPC_H
#define _GAME_BOARD_SNPC_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
/* The story NPC's save data (kept by the story driver across board loads). */
typedef struct SNpcSaveWork_s {
    /* 0x00 */ u8 koopaF : 1; /* night: Koopa instead of Donkey Kong */
    /* 0x01 */ u8 masuId;
    /* 0x02 */ u8 effectNum; /* turns since the last extra dice event */
} SNPCSAVEWORK;

void mbSNpcInit(void);
void mbSNpcCreate(SNPCSAVEWORK *saveWork, int level);
void mbSNpcKill(void);
u8 mbSNpcMasuGet(void);
void mbSNpcMotWinSet(void);
void mbSNpcMotIdleSet(void);
void mbSNpcDispSet(BOOL dispF);
void mbSNpcMotReset(void);
void mbSNpcPlayerWalkSet(int playerNo, int masuId);
void mbSNpcStarExec(int playerNo, int masuId);
int mbMasuChanceCreate(int arg0, int arg1, int arg2);
void mbMasuChanceKill(void *arg0);
void mbMasuChanceTypeSet(int arg0, u8 arg1, int arg2, int arg3);
void mbMasuChanceFlagSet(int arg0, u8 arg1, int arg2, int arg3, int arg4);
void mbMasuChancePlayerSet(int arg0, u8 arg1);
int mbMasuChanceSet(int arg0, s16 arg1);
int mbSNpcMasuStarNextGet(int masuId, int num, int *tbl, int level);
void mbObjStarTevStageSet(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *matP, u32 *tevStage, u32 *texCoord);
void mbObjFadeCreate(s16 objId, HuVecF *pos);
void mbObjFadeKill(s16 objId);
void mbObjFadeTexRotSet(s16 objId, HuVecF *pos, HuVecF *rot);
void mbObjFadeTexColorSet(s16 objId, u8 r, u8 g, u8 b, float scale);
void mbObjMetalCreate(s16 objId);
BOOL mbObjMetalKill(s16 objId);
void mbObjMetalTPLvlSet(s16 objId, float tpLvl);
void mbObjMetalColorSet(s16 objId, GXColor shadowColor, GXColor hiliteColor);
void mbObjBiriQCreate(s16 objId);
BOOL mbObjBiriQKill(s16 objId);
void mbObjBiriQColorSet(s16 objId, int mode, float alpha, GXColor color);

#endif
