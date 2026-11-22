#ifndef _GAME_BOARD_MASU_H
#define _GAME_BOARD_MASU_H

#include "game/board/board.h"

#define MASU_LINK_MAX 5
#define MASU_LAYER_MAX 2

typedef struct Masu_s {
    /* 0x00 */ u8 flag; /* bitfield (MP5: useMtxF, hasStarF) */
    /* 0x02 */ s16 unk2;
    /* 0x04 */ HuVecF pos;
    /* 0x10 */ u16 attr; /* mbMasuAttrGet */
    /* 0x14 */ u32 mAttr; /* mbMasuMAttrGet */
    /* 0x18 */ HuVecF scale;
    /* 0x24 */ HuVecF rot;
    /* 0x30 */ u16 type; /* mbMasuTypeGet */
    /* 0x32 */ s16 capsuleNo; /* mbMasuCapsuleGet */
    /* 0x34 */ Mtx matrix;
    /* 0x64 */ u16 linkNum; /* mbMasuLinkNumGet */
    /* 0x66 */ u16 linkTbl[MASU_LINK_MAX];
} MASU; /* size 0x70 (stride in masuData[layer]) */

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbMasuInit(int arg0);
void mbMasuClose(void);
BOOL mbMasuDataRead(int layer, int dataNum);
void mbMasuNextSet(s16 arg0);
void mbMasuNextDispSet(int arg0);
int mbev_MasuMove(int arg0, s16 arg1);
int mbev_MasuCapStop(int arg0, int arg1);
int mbev_MasuStop(int arg0, s16 arg1);
int mbev_MasuMasuStart(int arg0);
int mbev_MasuMasuEnd(s16 arg0);
int mbMasuNumGet(int layer);
s16 mbMasuRawNumGet(void);
MASU *mbMasuGet(s16 layer, int id);
void mbMasuLayerSet(int arg0);
int mbMasuLayerGet(void);
u16 mbMasuAttrGet(s16 arg0);
void mbMasuAttrSet(s16 arg0, int arg1);
int mbMasuMAttrGet(s16 arg0);
void mbMasuMAttrSet(s16 arg0, int arg1);
int mbev_MasuBitGet(int arg0, int arg1);
int mbev_MasuAttrGet(int arg0, int arg1);
int mbMasuTypeGet(s16 layer, int id);
void mbMasuTypeSet(s16 layer, u16 id, int type);
void mbMasuTypeChange(u16 oldType, u16 newType);
int mbMasuDispCheck(s16 arg0);
void mbMasuCapsuleReset(void);
int mbMasuCapsuleGet(s16 layer, int id);
void mbMasuCapsuleSet(s16 layer, s16 id, int capsuleNo);
void mbMasuPosGet(s16 arg0, HuVecF *arg1);
void mbMasuPosSet(s16 arg0, float f0, float f1, float f2);
void mbMasuPosSetV(s16 arg0, HuVecF *arg1);
void mbMasuCornerRotPosGet(s16 id, int cornerNo, HuVecF *pos);
void mbMasuCornerPosGet(s16 id, int cornerNo, HuVecF *pos);
void mbMasuRotGet(int arg0, HuVecF *arg1);
void mbMasuRotSet(s16 arg0, float f0, float f1, float f2);
void mbMasuRotSetV(s16 arg0, HuVecF *arg1);
void mbMasuMtxGet(int masuId, Mtx matrix);
void mbMasuMtxSet(int masuId, Mtx matrix);
BOOL mbMasuDispGet(void);
void mbMasuDispSet(BOOL dispF);
void mbMasuDispAttrSet(int arg0);
void mbMasuDispAttrReset(int arg0);
u16 mbMasuDispAttrGet(void);
void mbMasuDispMAttrSet(int arg0);
void mbMasuDispMAttrReset(int arg0);
int mbMasuDispMAttrGet(void);
void mbMasuModelDispSet(int arg0);
s16 mbMasuLinkGet(s16 arg0, int arg1);
s16 mbMasuLinkNumGet(s16 arg0);
int mbMasuLinkTblGet(int arg0, void *arg1);
int mbMasuLinkTblGet2(s16 arg0, int arg1, int arg2);
int mbMasuAttrFindLink(s16 arg0, u16 arg1);
int mbMasuAttrMatchFindLink(s16 arg0, u16 arg1, u16 arg2);
int mbMasuMAttrFindLink(s16 arg0, int arg1);
int mbMasuMAttrMatchFindLink(s16 arg0, int arg1, int arg2);
int mbMasuTypeFindLink(s16 arg0, int arg1);
s16 mbMasuLinkParentGet(s16 arg0, void *arg1);
s16 mbMasuFind_TypeStepGet(int arg0, int arg1);
s16 mbMasuFind_TypeStepGet2(int arg0, int arg1, int arg2, int arg3);
s16 mbMasuFind_AttrStepGet(int arg0, int arg1);
s16 mbMasuFind_AttrMatchStepGet(int arg0, int arg1, int arg2);
s16 mbMasuFind_AttrStepGet2(int arg0, int arg1, int arg2, int arg3, int arg4);
s16 mbMasuFind_MAttrStepGet(int arg0, int arg1);
s16 mbMasuFind_MAttrMatchStepGet(int arg0, int arg1, int arg2);
s16 mbMasuFind_MAttrStepGet2(int arg0, int arg1, int arg2, int arg3, int arg4);
s16 mbMasuFind_IdStepGet(int arg0, int arg1);
s16 mbMasuFind_IdStepGet2(int arg0, int arg1, int arg2, int arg3);
int mbMasuFind_TypeSearch(int arg0, int arg1);
int mbMasuFind_TypeIdGet(s16 arg0, s16 arg1, int arg2, int arg3);
int mbMasuFind_AttrIdGet(s16 arg0, u16 arg1);
int mbMasuFind_AttrMatchIdGet(int arg0, int arg1, int arg2);
int mbMasuFind_AttrMatchIdGet2(s16 arg0, u16 arg1, u16 arg2, int arg3, int arg4);
int mbMasuFind_MAttrIdGet(s16 arg0, int arg1);
int mbMasuFind_MAttrMatchIdGet(int arg0, int arg1, int arg2);
int mbMasuFind_MAttrMatchIdGet2(s16 arg0, int arg1, int arg2, int arg3, int arg4);
int mbMasuTypeListGet(s16 arg0, void *arg1);
int mbMasuAttrListGet(u16 arg0, int arg1);
int mbMasuAttrMatchListGet(u16 arg0, u16 arg1, int arg2);
int mbMasuMAttrListGet(int arg0, void *arg1);
int mbMasuMAttrMatchTblGet(int arg0, int arg1, int arg2);
s16 mbMasuFind_TypeListGet(int arg0, int arg1, void *arg2);
s16 mbMasuFind_TypeListGet2(int arg0, int arg1, int arg2, int arg3, void *arg4);
s16 mbMasuFind_AttrNumGet(int arg0, int arg1, int arg2);
s16 mbMasuFind_AttrMatchNumGet(int arg0, int arg1, int arg2, int arg3);
s16 mbMasuFind_AttrMatchNumGet2(int arg0, int arg1, int arg2, int arg3, int arg4);
int mbMasuFind_MAttrNumGet(s16 arg0, int arg1);
s16 mbMasuFind_MAttrMatchListGet(int arg0, int arg1, int arg2, void *arg3);
s16 mbMasuFind_MAttrMatchListGet2(int arg0, int arg1, int arg2, int arg3, int arg4, void *arg5);
s16 mbMasuFind_IdListGet(int arg0, int arg1, void *arg2);
s16 mbMasuFind_IdListGet2(int arg0, int arg1, int arg2, int arg3, void *arg4);
void mbev_MasuMoveEndSet(void *arg0);
void mbev_MasuMoveStartSet(void *arg0);
void mbev_MasuHatenaSet(void *arg0);
void mbev_MasuLinkTblHookSet(void *arg0);
int mbMasuPlayerCapMoveCheck(int arg0, int arg1);
int mbMasuPlayerCapStopCheck(int arg0, int arg1);
void mbMasuPlayerColorSet(int arg0);
void mbMasuPlayerDispSet(int arg0);
void mbMasuPlayerFadeSet(int arg0);
void mbMasuPlayerPrizeReset(int arg0);
int mbMasuPKinokoValueGet(int arg0, s16 arg1);
int mbMasuStub(void);

#endif
