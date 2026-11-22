#ifndef _GAME_BOARD_MASU_H
#define _GAME_BOARD_MASU_H

#include "game/board/board.h"

#define MASU_MAX 256
#define MASU_LINK_MAX 5
#define MASU_LAYER_MAX 2

typedef struct Masu_s {
    /* 0x00 */ u8 useMtxF : 1;
    /* 0x02 */ s16 id;
    /* 0x04 */ HuVecF pos;
    /* 0x10 */ u16 attr;
    /* 0x14 */ u32 mAttr;
    /* 0x18 */ HuVecF scale;
    /* 0x24 */ HuVecF rot;
    /* 0x30 */ u16 type;
    /* 0x32 */ s16 capsuleNo;
    /* 0x34 */ Mtx matrix;
    /* 0x64 */ u16 linkNum;
    /* 0x66 */ u16 linkTbl[MASU_LINK_MAX];
} MASU; /* size 0x70 */

typedef int (*MASUEVHOOK)(int playerNo, s16 masuId);
typedef int (*MASULINKTBLHOOK)(s16 masuId, u32 mAttr, s16 *linkTbl, int hookF);

void mbMasuInit(int dataNum);
void mbMasuClose(void);
BOOL mbMasuDataRead(int dataNum);
void mbMasuNextSet(s16 masuId);
void mbMasuNextDispSet(BOOL dispF);
int mbev_MasuMove(int playerNo, s16 masuId);
int mbev_MasuCapStop(int playerNo, s16 masuId);
int mbev_MasuStop(int playerNo, s16 masuId);
int mbev_MasuMasuStart(int playerNo);
int mbev_MasuMasuEnd(int masuId);
int mbMasuNumGet(void);
int mbMasuRawNumGet(void);
MASU *mbMasuGet(s16 id);
void mbMasuLayerSet(int layer);
int mbMasuLayerGet(void);
u16 mbMasuAttrGet(s16 id);
void mbMasuAttrSet(s16 id, u16 attr);
u32 mbMasuMAttrGet(s16 id);
void mbMasuMAttrSet(s16 id, u32 mAttr);
u32 mbev_MasuBitGet(u32 value, u32 mask);
u32 mbev_MasuAttrGet(int value, u32 mask);
int mbMasuTypeGet(s16 id);
void mbMasuTypeSet(s16 id, int type);
void mbMasuTypeChange(u16 oldType, u16 newType);
int mbMasuDispCheck(s16 id);
void mbMasuCapsuleReset(void);
s16 mbMasuCapsuleGet(s16 id);
void mbMasuCapsuleSet(s16 id, int capsuleNo);
void mbMasuPosGet(s16 id, HuVecF *pos);
void mbMasuPosSet(s16 id, float x, float y, float z);
void mbMasuPosSetV(s16 id, HuVecF *pos);
void mbMasuCornerRotPosGet(s16 id, int cornerNo, HuVecF *pos);
void mbMasuCornerPosGet(s16 id, int cornerNo, HuVecF *pos);
void mbMasuRotGet(s16 id, HuVecF *rot);
void mbMasuRotSet(s16 id, float x, float y, float z);
void mbMasuRotSetV(s16 id, HuVecF *rot);
void mbMasuMtxGet(s16 id, Mtx mtx);
void mbMasuMtxSet(s16 id, Mtx mtx);
BOOL mbMasuDispGet(void);
void mbMasuDispSet(BOOL dispF);
void mbMasuDispAttrSet(u16 attr);
void mbMasuDispAttrReset(int attr);
u16 mbMasuDispAttrGet(void);
void mbMasuDispMAttrSet(u32 mAttr);
void mbMasuDispMAttrReset(u32 mAttr);
u32 mbMasuDispMAttrGet(void);
void mbMasuModelDispSet(BOOL dispF);
s16 mbMasuLinkGet(s16 id, int no);
s16 mbMasuLinkNumGet(s16 id);
int mbMasuLinkTblGet(s16 id, s16 *linkTbl);
int mbMasuLinkTblGet2(s16 id, s16 *linkTbl, BOOL hookF);
s16 mbMasuAttrFindLink(s16 id, u16 attr);
s16 mbMasuAttrMatchFindLink(s16 id, u16 attr, u16 mask);
s16 mbMasuMAttrFindLink(s16 id, u32 mAttr);
s16 mbMasuMAttrMatchFindLink(s16 id, u32 mAttr, u32 mask);
s16 mbMasuTypeFindLink(s16 id, int type);
s16 mbMasuLinkParentGet(s16 id, s16 *linkTbl);
s16 mbMasuFind_TypeStepGet(s16 id, s16 type);
s16 mbMasuFind_TypeStepGet2(s16 id, s16 type, BOOL hookF, BOOL dispF);
s16 mbMasuFind_AttrStepGet(s16 id, u16 attr);
s16 mbMasuFind_AttrMatchStepGet(s16 id, u16 attr, u16 mask);
s16 mbMasuFind_AttrStepGet2(s16 id, u16 attr, u16 mask, BOOL hookF, BOOL dispF);
s16 mbMasuFind_MAttrStepGet(s16 id, u32 mAttr);
s16 mbMasuFind_MAttrMatchStepGet(s16 id, u32 mAttr, u32 mask);
s16 mbMasuFind_MAttrStepGet2(s16 id, u32 mAttr, u32 mask, BOOL hookF, BOOL dispF);
s16 mbMasuFind_IdStepGet(s16 id, s16 targetId);
s16 mbMasuFind_IdStepGet2(s16 id, s16 targetId, BOOL hookF, BOOL dispF);
s16 mbMasuFind_TypeSearch(s16 id, s16 type);
s16 mbMasuFind_TypeIdGet(s16 id, s16 type, BOOL hookF, BOOL dispF);
s16 mbMasuFind_AttrIdGet(s16 id, u16 attr);
s16 mbMasuFind_AttrMatchIdGet(s16 id, u16 attr, u16 mask);
s16 mbMasuFind_AttrMatchIdGet2(s16 id, u16 attr, u16 mask, BOOL hookF, BOOL dispF);
s16 mbMasuFind_MAttrIdGet(s16 id, u32 mAttr);
s16 mbMasuFind_MAttrMatchIdGet(s16 id, u32 mAttr, u32 mask);
s16 mbMasuFind_MAttrMatchIdGet2(s16 id, u32 mAttr, u32 mask, BOOL hookF, BOOL dispF);
int mbMasuTypeListGet(s16 type, s16 *list);
int mbMasuAttrListGet(u16 attr, s16 *list);
int mbMasuAttrMatchListGet(u16 attr, u16 mask, s16 *list);
int mbMasuMAttrListGet(u32 mAttr, s16 *list);
int mbMasuMAttrMatchTblGet(u32 mAttr, u32 mask, s16 *list);
s16 mbMasuFind_TypeListGet(s16 id, s16 type, s16 *list);
s16 mbMasuFind_TypeListGet2(s16 id, s16 type, BOOL hookF, BOOL dispF, s16 *list);
s16 mbMasuFind_AttrNumGet(s16 id, u16 attr, s16 *list);
s16 mbMasuFind_AttrMatchNumGet(s16 id, u16 attr, u16 mask, s16 *list);
s16 mbMasuFind_AttrMatchNumGet2(s16 id, u16 attr, u16 mask, BOOL hookF, BOOL dispF, s16 *list);
s16 mbMasuFind_MAttrNumGet(s16 id, u32 mAttr);
s16 mbMasuFind_MAttrMatchListGet(s16 id, u32 mAttr, u32 mask, s16 *list);
s16 mbMasuFind_MAttrMatchListGet2(s16 id, u32 mAttr, u32 mask, BOOL hookF, BOOL dispF, s16 *list);
s16 mbMasuFind_IdListGet(s16 id, s16 targetId, s16 *list);
s16 mbMasuFind_IdListGet2(s16 id, s16 targetId, BOOL hookF, BOOL dispF, s16 *list);
void mbev_MasuMoveEndSet(MASUEVHOOK func);
void mbev_MasuMoveStartSet(MASUEVHOOK func);
void mbev_MasuHatenaSet(MASUEVHOOK func);
void mbev_MasuLinkTblHookSet(MASULINKTBLHOOK func);
int mbMasuPlayerCapMoveCheck(int playerNo, s16 masuId);
int mbMasuPlayerCapStopCheck(int playerNo, s16 masuId);
void mbMasuPlayerColorSet(int playerNo);
void mbMasuPlayerDispSet(BOOL dispF);
void mbMasuPlayerFadeSet(BOOL fadeF);
void mbMasuPlayerPrizeReset(int playerNo);
int mbMasuPKinokoValueGet(int playerNo, s16 masuId);
int mbMasuStub(void);

#endif
